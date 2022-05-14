#include "impl.h"

RootSet_t* basicRootSets = NULL;

pRootSet_t pl_getRootSet(int thrdID) {
    return basicRootSets + thrdID;
}

pRootSet_t pl_newRootSet(void* placement, pRootSet_t prv, atomic_ref_t* start, size_t refCount) {
    pRootSet_t ret = placement;
    ret->start = start;
    ret->refCount = refCount;
    ret->next = prv->next;

    atomic_store(&prv->next, ret);

    return ret;
}

void pl_deleteRootSet(pRootSet_t prv, void(*dtor)()) {
    pRootSet_t n = atomic_exchange(&prv->next, NULL);
    if(!n) return;
    if(!n->start) return;

    while(atomic_load(&n->lock));
    if(dtor) dtor(n);
}

atomic_ref_t* pl_loadObjRef(atomic_ref_t* p, size_t idx) {
    return pl_impl_loadValueBarrier(p)->ref + idx;
}

void pl_storeObjRef(atomic_ref_t* p, size_t idx, atomic_ref_t* pValue) {
    ref_t value = null;
    if(pValue) value = ptr2ref(pl_impl_loadValueBarrier(pValue));
    atomic_store(pl_loadObjRef(p, idx), value);
}

void* pl_load(atomic_ref_t* p, size_t offs) {
    return *(void**)(pl_impl_loadValueBarrier(p)->start + offs);
}

void pl_store(atomic_ref_t* p, size_t offs, void* value) {
    *(void**)(pl_impl_loadValueBarrier(p)->start + offs) = value;
}