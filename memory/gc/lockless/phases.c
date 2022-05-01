#include "impl.h"

static atomic_bool gcMarkFlag = 0;
static atomic_bool gcCopyFlag = 0;
static atomic_bool gcRemapFlag = 0;

static atomic_int completedCount = 0;

static atomic_int idx = 0;
static atomic_ptrdiff_t workListIter = NULL;

void plgc_impl_main() {
    while(1) {
        plgc_impl_exchangeSapce();
        //

        atomic_store(&gcMarkFlag, !gcMarkFlag);
        while(completedCount != gcThrdCount);
        completedCount = 0; idx = 0;
        workListIter = workListBegin;

        atomic_store(&gcCopyFlag, !gcCopyFlag);
        while(completedCount != gcThrdCount);
        completedCount = 0;

        atomic_store(&gcRemapFlag, !gcRemapFlag);
    }
}

static mbi_t* workListBegin = NULL;
static atomic_ptrdiff_t workListEnd = NULL;

void plgc_impl_pushWorkList(mbi_t* mbi) {
    if(atomic_fetch_or(&mbi->status, 1 << 1) >> 1) return;

    mbi_t* prv = atomic_exchange(&workListEnd, mbi);
    if(!prv) { workListIter = workListBegin = mbi; return; }

    prv->next = mbi;
    mbi->next = NULL;
}

static void objMark() {
    mbi_t* obj = NULL;
    while(obj = atomic_exchange(&workListIter, ((mbi_t*)atomic_load(&workListIter))->next)) {
        for(int i = 0; i < obj->refCount; ++i) {
            mbi_t* sub = obj->start[i];
            if(plgc_impl_testSpace(sub)) continue;
            plgc_impl_pushWorkList(sub);
        }
    }
}

static void mark() {
    int legalIdx = 0;
    while((legalIdx = atomic_fetch_add(&idx, 1)) < getMaxThrdCount()) {
        rshdl_t list = rootSets + legalIdx;

        for(rshdl_t iter = atomic_load(&list->next); iter; ) {
            atomic_store(&iter->lock, 1);

            for(int i = 0; i < iter->refCount; ++i) {
                mbi_t* mbi = iter->start[i];
                if(plgc_impl_testSpace(mbi)) continue;
                plgc_impl_pushWorkList(mbi);
            }
            iter = atomic_load(&iter->next);

            iter->lock = 0;
        }   
    }
}

static void copy() {
    mbi_t* obj = NULL;
    while(obj = atomic_exchange(&workListIter, ((mbi_t*)atomic_load(&workListIter))->next)) {
        if(plgc_impl_testSpace(obj)) continue;
        if(obj->newAddr) continue;
        if(atomic_fetch_or(&obj->status, 1) & 0b01) continue;
        
        obj->newAddr = plgc_malloc(obj->size, obj->refCount);
        memcpy(obj->newAddr->start, obj->start, obj->size);
        obj->status = 0;
    }
}

mbi_t* plgc_impl_loadBarrier(atomic_ptrdiff_t* hdl) {
    mbi_t* mbi = NULL;
    do {
        mbi_t* mbi = atomic_load(hdl);
        if(plgc_impl_testSpace(mbi)) return mbi;
        if(mbi->newAddr) continue;

        while(atomic_fetch_or(&mbi->status, 0b01));
        if(mbi->newAddr) continue;

        mbi->newAddr = plgc_malloc(mbi->size, mbi->refCount);
        memcpy(mbi->newAddr->start, mbi->start, mbi->size);
        mbi->status = 0;
    } while(!atomic_compare_exchange_weak(hdl, mbi, mbi->newAddr));

    return mbi->newAddr;
}

static void remap() {
    int legalIdx = 0;
    while((legalIdx = atomic_fetch_add(&idx, 1)) < getMaxThrdCount()) {
        rshdl_t list = rootSets + legalIdx;

        rshdl_t iter = atomic_load(&list->next);
        while(iter) {
            atomic_store(&iter->lock, 1);

            for(int i = 0; i < iter->refCount; ++i) {
                // if(plgc_impl_testSpace(iter->start[i])) continue;
                plgc_impl_loadBarrier(iter->start + i);
            }

            iter->lock = 0;
        }
    }
}