#include "impl.h"

static atomic_bool gcCopyFlag = 0;
static atomic_bool gcRemapFlag = 0;

static atomic_int completedCount = 0;
static atomic_int gcTrigger = 0;

static atomic_int markIdx = 0;
static atomic_int remapIdx = 0;

static _Atomic(void*) workListIter = NULL;
static mbi_t* workListBegin = NULL;
static _Atomic(void*) workListEnd = NULL;

static _Atomic(void*) remapListIter = NULL;
static mbi_t* remapListBegin = NULL;
static _Atomic(void*) remapListEnd = NULL;

static _Atomic size_t copied = 0;

atomic_int gcTimeCount = 0;
int llgc_timeCount() { return atomic_load(&gcTimeCount); }

void llgc_impl_main() {
    while(1) {
        llgc_impl_exchangeSpace();
        atomic_store(&gcTrigger, !gcTrigger);

        while(!atomic_compare_exchange_weak(&completedCount, &gcThrdCount, 0));
        size_t delta = llgc_impl_exchangeAlloc();
        atomic_fetch_add(&gcTimeCount, 1);
        markIdx = 0; remapIdx = 0;
        workListIter = workListBegin;

        remapListEnd = NULL;
        atomic_store(&gcCopyFlag, gcTrigger);
        while(!atomic_compare_exchange_weak(&completedCount, &gcThrdCount, 0));
        atomic_fetch_add(&collected, delta - copied);
        atomic_store(&copied, 0);
        
        workListEnd = NULL;
        atomic_store(&gcRemapFlag, gcTrigger);
        while(!atomic_compare_exchange_weak(&completedCount, &gcThrdCount, 0));
    }
}

void llgc_impl_pushWorkList(mbi_t* mbi) {
    printf("pushWorkList is invoked\n");
    if(atomic_fetch_or(&mbi->status, 0b10) & 0b10) return;

    mbi_t* prv = atomic_exchange(&workListEnd, mbi);
    if(!prv) { workListBegin = mbi; atomic_store(&remapListIter, mbi); return; }

    prv->next = mbi;
    mbi->next = NULL;
}

static void pushRemapList(mbi_t* mbi) {
    printf("pushRemapList is invoked\n");
    if(!atomic_load(&mbi->newAddr)) printf("why\n");
    mbi_t* prv = atomic_exchange(&remapListEnd, mbi);
    if(!prv) { remapListBegin = mbi; atomic_store(&remapListIter, mbi); return; }
    prv->next = mbi;
    mbi->next = NULL;
}

static void objMark() {
    mbi_t* obj = NULL, *next = NULL;
    while(obj = atomic_load(&workListIter)) {
        next = obj->next;
        if(atomic_compare_exchange_weak(&workListIter, &obj, next))
        for(int i = 0; i < obj->refCount; ++i) {
            mbi_t* sub = obj->start[i];
            if(llgc_impl_testSpace(sub)) continue;
            llgc_impl_pushWorkList(sub);
        }
    }
}

static void mark() {
    int legalmarkIdx = 0;
    while((legalmarkIdx = atomic_fetch_add(&markIdx, 1)) < getMaxThrdCount()) {
        rshdl_t list = rootSets + legalmarkIdx;
        for(rshdl_t iter = atomic_load(&list->next); iter; ) {
            atomic_store(&iter->lock, 1);
            for(int i = 0; i < iter->refCount; ++i) {
                mbi_t* mbi = atomic_load(iter->start + i);
                if(!mbi) continue;
                if(llgc_impl_testSpace(mbi)) continue;
                llgc_impl_pushWorkList(mbi);
            }
            rshdl_t temp = iter;
            iter = atomic_load(&iter->next);

            atomic_store(&temp->lock, 0);
        }   
    }

    objMark();
    atomic_fetch_add(&completedCount, 1);
}

static void copy() {
    mbi_t* obj = NULL, *next = NULL;
    mbi_t* temp = NULL;
    while(obj = atomic_load(&workListIter)) {
        next = obj->next;
        if(!atomic_compare_exchange_weak(&workListIter, &obj, next)) continue;
        if(llgc_impl_testSpace(obj)) continue;
        if(atomic_load(&obj->newAddr)) continue;
        if(atomic_fetch_or(&obj->status, 1) & 1) continue;
        
        temp = llgc_impl_sbrk(obj->size, obj->refCount, 0);
        if(!temp) continue;
        memcpy(temp->start, obj->start, obj->size);
        atomic_store(&obj->newAddr, temp);
        printf("%p\n", atomic_load(&obj->newAddr));
        pushRemapList(temp);

        atomic_fetch_add(&copied, obj->size + sizeof(mbi_t));
        atomic_fetch_and(&obj->status, 0b10);
    }
    atomic_fetch_add(&completedCount, 1);
}

mbi_t* llgc_impl_loadBarrier(_Atomic(void*)* hdl) {
    mbi_t* mbi = NULL;
    mbi_t* temp = NULL;
    do {
        mbi = atomic_load(hdl);
        if(!mbi) return NULL; 
        if(llgc_impl_testSource(mbi)) return mbi; 
        if(atomic_load(&mbi->newAddr)) continue;
        if(atomic_fetch_or(&mbi->status, 1) & 1) { while(atomic_load(&mbi->status) & 0b01); continue; }

        temp = llgc_impl_sbrk(mbi->size, mbi->refCount, 0);
        if(!temp) goto lable;
        memcpy(temp->start, mbi->start, mbi->size);
        atomic_store(&mbi->newAddr, temp);
        pushRemapList(temp);

        atomic_fetch_add(&copied, mbi->size + sizeof(mbi_t));
lable:
        atomic_fetch_and(&mbi->status, 0b10);
    } while(!atomic_compare_exchange_weak(hdl, &mbi, atomic_load(&mbi->newAddr)));

    return atomic_load(&mbi->newAddr);
}

static void remap() {
    atomic_fetch_add(&completedCount, 1);
    int legalmarkIdx = 0;
    while((legalmarkIdx = atomic_fetch_add(&remapIdx, 1)) < getMaxThrdCount()) {
        rshdl_t list = rootSets + legalmarkIdx;

        rshdl_t iter = atomic_load(&list->next);
        while(iter) {
            atomic_store(&iter->lock, 1);

            for(int i = 0; i < iter->refCount; ++i) {
                mbi_t* root = atomic_load(iter->start + i);
                if(llgc_impl_testSource(root)) continue;
                else if(root) {
                    printf("root->newAddr = %p\n", atomic_load(&root->newAddr));
                    if(!atomic_compare_exchange_weak(iter->start + i, &root, root->newAddr)) --i;
                }
            }
            
            rshdl_t temp = iter;
            iter = atomic_load(&iter->next);

            temp->lock = 0;
        }
    }

    mbi_t* mbi = NULL, *next = NULL;
    while(mbi = atomic_load(&remapListIter)) {
        next = mbi->next;
        if(!atomic_compare_exchange_weak(&remapListIter, &mbi, next)) continue;
        for(int i = 0; i < mbi->refCount; ++i) {
            mbi_t* sub = atomic_load(mbi->start + i);
            if(llgc_impl_testSource(sub)) continue;
            else if(sub) if(!atomic_compare_exchange_weak(mbi->start + i, &sub, sub->newAddr)) --i;
        }
    }
}

void* llgc_impl_gcThrdFunc(void* arg) {
    while(1) {
        mark();
        while(atomic_load(&gcCopyFlag) != atomic_load(&gcTrigger));
        copy();
        while(atomic_load(&gcRemapFlag) != atomic_load(&gcTrigger));
        remap();
    }
}