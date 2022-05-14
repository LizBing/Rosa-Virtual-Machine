#include "impl.h"

atomic_bool gcCopyFlag = 0;
static atomic_bool gcMarkAndRemapFlag = 0;

static atomic_int completedCount = 0;

_Atomic(pRootSet_t) gcRootSetIter = NULL;

static _Atomic(pObject_t) workListIter = NULL;
static pObject_t workListBegin = NULL;
static _Atomic(pObject_t) workListEnd = NULL;

static atomic_size_t copied = 0;
static atomic_size_t collected = 0;

static atomic_int routine = 0;
int pl_routineCount() {
    return atomic_load(&routine);
}

size_t pl_collected() {
    return atomic_load(&collected);
}

void pl_impl_pushWorkList(pObject_t obj) {
    if(atomic_exchange(&obj->status, 1)) return;
    // printf("pushed %p into work list\n", obj);
    obj->workListNext = NULL;
    pObject_t prv = atomic_exchange(&workListEnd, obj);
    if(!prv) workListIter = workListBegin = obj; 
    else prv->workListNext = obj;
}

pObject_t pl_impl_loadValueBarrier(atomic_ref_t* pRef) {
    pl_impl_heapLock();

    ref_t ref = atomic_load(pRef);
    if(!ref) { pl_impl_heapUnlock(); return NULL; }

    if(isExpected(ref)) {
        if(!atomic_load(&gcCopyFlag)) 
            pl_impl_pushWorkList(loadPtr(ref));

        pl_impl_heapUnlock();
        return loadPtr(ref);
    }
    
    pObject_t old = loadPtr(ref), new = NULL;
    if(!atomic_fetch_and(&old->status, 0)) {
        while(!(new = atomic_load(&old->newAddr)));
        goto ret;
    }

    size_t size = old->size + sizeof(Object_t);
    new = pl_impl_sbrk(size);
    if(!new) exit(1);

    new->newAddr = NULL;
    new->status = 0;
    memcpy(&new->size, &old->size, old->size + sizeof(size_t) * 2);
    atomic_store(&old->newAddr, new);
    atomic_fetch_add(&copied, size);
    
ret:
    if(!atomic_load(&gcCopyFlag))
        pl_impl_pushWorkList(new);
    
    if(atomic_load(pRef) == ref) { 
        atomic_store(pRef, ptr2ref(new));
    } else {
        pl_impl_heapUnlock();
        return pl_impl_loadValueBarrier(pRef);
    }

    pl_impl_heapUnlock();
    return new;
}

static void markAndRemapRootSet() {
    pRootSet_t rs = NULL, next = NULL;
    while(rs = atomic_load(&gcRootSetIter)) {
        next = atomic_load(&rs->next);
        if(atomic_load(&gcRootSetIter) == rs) 
            atomic_store(&gcRootSetIter, next);

        atomic_store(&rs->lock, 1);
        for(size_t i = 0; i < rs->refCount; ++i) {
            ref_t ref = atomic_load(rs->start + i);
            if(!ref) continue;

            if(isExpected(ref)) { 
                pl_impl_pushWorkList(loadPtr(ref)); 
                continue; 
            }

            pObject_t* new = NULL;
            while(!(new = atomic_load(&((pObject_t)loadPtr(ref))->newAddr)));
            if(!new) {
                int i = *(int*)NULL;
            }
            if(atomic_load(rs->start + i) == ref) { 
                atomic_store(rs->start + i, ptr2ref(new));
                pl_impl_pushWorkList(new);
            }
            else { --i; continue; }
        }
        atomic_store(&rs->lock, 0);
    }
}

static void markAndRemapObj() {
    pObject_t obj = NULL, next = NULL;
    while(obj = atomic_load(&workListIter)) {
        next = obj->workListNext;
        if(atomic_load(&workListIter) == obj) 
            atomic_store(&workListIter, next);

        for(size_t i = 0; i < obj->refCount; ++i) {
            atomic_ref_t* pRef = obj->ref + i;
            ref_t ref = atomic_load(pRef);
            if(!ref) continue;

            if(isExpected(ref)) { 
                pl_impl_pushWorkList(loadPtr(ref));
                continue; 
            }

            pObject_t new = NULL;
            while(!(new = atomic_load(&((pObject_t)loadPtr(ref))->newAddr)));
            if(atomic_load(pRef) == ref) { 
                pl_impl_pushWorkList(new);
                atomic_store(pRef, ptr2ref(new));
            }
            else { --i; continue; }
        }
    }
}

static void copy() {
    pObject_t obj = NULL, next = NULL;
    while(obj = atomic_load(&workListIter)) {
        // printf("%p\n", obj);
        next = obj->workListNext;
        if(atomic_load(&workListIter) == obj) 
            atomic_store(&workListIter, next);

        if(!atomic_fetch_and(&obj->status, 0)) continue;

        size_t size = obj->size + sizeof(Object_t); 
        pObject_t new = pl_impl_sbrk(size);
        if(!new) exit(1);

        new->newAddr = NULL;
        new->status = 0;
        memcpy(&new->size, &obj->size, obj->size + sizeof(size_t) * 2);
        atomic_store(&obj->newAddr, new);
        atomic_fetch_add(&copied, size);
    }
}

void pl_impl_manager() {
    while(1) {
        gcRootSetIter = basicRootSets;

        atomic_store(&gcMarkAndRemapFlag, 1);
        while(atomic_load(&completedCount) != gcThrdCount) // printf("mar rotating...\n")
        ;
        atomic_store(&gcMarkAndRemapFlag, 0);
        // printf("mark & remap phases finished.\n");
        completedCount = 0;

        workListIter = workListBegin;
        ptrdiff_t delta = pl_impl_exchangeSpace();
        atomic_fetch_add(&routine, 1);
        
        // printf("copy phase start.\n");
        while(atomic_load(&completedCount) != gcThrdCount) // printf("c rotating...\n")
        ;
        workListEnd = NULL;
        atomic_store(&gcCopyFlag, 0);
        completedCount = 0;

        atomic_fetch_add(&collected, delta - copied);
        copied = 0;
    }
}

void pl_impl_thrdFunc() {
    while(1) {
        while(!atomic_load(&gcMarkAndRemapFlag));
        markAndRemapRootSet();
        markAndRemapObj();
        atomic_fetch_add(&completedCount, 1);

        while(!atomic_load(&gcCopyFlag));
        copy();
        atomic_fetch_add(&completedCount, 1);
    }
}
