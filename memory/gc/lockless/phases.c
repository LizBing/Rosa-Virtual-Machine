#include "impl.h"

static atomic_bool gcCopyFlag = 0;
static atomic_bool gcRemapFlag = 0;

static atomic_int completedCount = 0;
static atomic_int gcTrigger = 0;

static atomic_int markIdx = 0;
static atomic_int remapIdx = 0;
static atomic_ptrdiff_t toSpaceIter;

static atomic_ptrdiff_t workListIter;
static mbi_t* workListBegin;
static atomic_ptrdiff_t workListEnd;

static _Atomic size_t copied = 0;

void llgc_impl_main() {
    while(1) {
        llgc_impl_exchangeSpace();
        atomic_store(&gcTrigger, !gcTrigger);
        workListEnd = NULL;

        while(completedCount != gcThrdCount);
        size_t delta = llgc_impl_exchangeAlloc();
        completedCount = 0; 
        markIdx = 0; remapIdx = 0;
        workListIter = workListBegin;
        toSpaceIter = toSpace;

        atomic_store(&gcCopyFlag, gcTrigger);
        while(completedCount != gcThrdCount);
        completedCount = 0;
        atomic_fetch_add(&collected, delta - copied);
        atomic_store(&copied, 0);

        atomic_store(&gcRemapFlag, gcTrigger);
    }
}


void llgc_impl_pushWorkList(mbi_t* mbi) {
    if(atomic_fetch_or(&mbi->status, 1 << 1) >> 1) return;

    mbi_t* prv = atomic_exchange(&workListEnd, mbi);
    if(!prv) { workListIter = workListBegin = mbi; return; }

    prv->next = mbi;
    mbi->next = NULL;
}

static void objMark() {
    for(mbi_t* obj = atomic_load(&workListIter); obj; obj = atomic_load(&workListIter)) {
        if(!atomic_compare_exchange_weak(&workListIter, obj, ((mbi_t*)atomic_load(&workListIter))->next)) continue;
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
                if(llgc_impl_testSpace(mbi)) continue;
                llgc_impl_pushWorkList(mbi);
            }
            rshdl_t temp = iter;
            iter = atomic_load(&iter->next);

            temp->lock = 0;
        }   
    }

    objMark();
    atomic_fetch_add(&completedCount, 1);
}

static void copy() {
    for(mbi_t* obj = atomic_load(&workListIter); obj; obj = atomic_load(&workListIter)) {
        if(!atomic_compare_exchange_weak(&workListIter, obj, ((mbi_t*)atomic_load(&workListIter))->next)) continue;
        if(llgc_impl_testSpace(obj)) continue;
        if(obj->newAddr) continue;
        if(atomic_fetch_or(&obj->status, 1) & 0b01) continue;
        
        obj->newAddr = llgc_impl_sbrk(obj->size, obj->refCount, 0);
        memcpy(obj->newAddr->start, obj->start, obj->size);
        atomic_fetch_add(&copied, obj->size);
        obj->status = 0;
    }
lable:
    atomic_fetch_add(&completedCount, 1);
}

mbi_t* llgc_impl_loadBarrier(atomic_ptrdiff_t* hdl) {
    mbi_t* mbi = NULL;
    do {
        mbi_t* mbi = atomic_load(hdl);
        if(!mbi) return mbi;
        if(llgc_impl_testSource(mbi)) return mbi;
        if(mbi->newAddr) continue;

        while(atomic_fetch_or(&mbi->status, 0b01));
        if(mbi->newAddr) continue;

        mbi->newAddr = llgc_impl_sbrk(mbi->size, mbi->refCount, 0);
        memcpy(mbi->newAddr->start, mbi->start, mbi->size);
        atomic_fetch_add(&copied, mbi->size);
        mbi->status = 0;
    } while(!atomic_compare_exchange_weak(hdl, mbi, mbi->newAddr));

    return mbi->newAddr;
}

static void remap() {
    int legalmarkIdx = 0;
    while((legalmarkIdx = atomic_fetch_add(&remapIdx, 1)) < getMaxThrdCount()) {
        rshdl_t list = rootSets + legalmarkIdx;

        rshdl_t iter = atomic_load(&list->next);
        while(iter) {
            atomic_store(&iter->lock, 1);

            for(int i = 0; i < iter->refCount; ++i) 
                llgc_impl_loadBarrier(iter->start + i);
            
            rshdl_t temp = iter;
            iter = atomic_load(&iter->next);

            temp->lock = 0;
        }
    }

    mbi_t* mbi = NULL;
    while(llgc_impl_testSource(mbi = atomic_fetch_add(&toSpaceIter, ((mbi_t*)atomic_load(&toSpaceIter))->size + sizeof(mbi_t)))) {
        if(!mbi->size) return;
        for(int i = 0; i < mbi->refCount; ++i)
            llgc_impl_loadBarrier(mbi->start + i);
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