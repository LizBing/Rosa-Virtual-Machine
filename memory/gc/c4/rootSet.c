#include "impl.h"

rs_t* basicrs = NULL;

prs_t c4_getRootSet(size_t thrdID) {
    return basicrs + thrdID;
}

prs_t c4_pushRootSet(void* placement, prs_t prv, _Atomic(void*)* start, size_t refCount) {
    prs_t ret = placement;
    ret->lock = 0;
    
    ret->size = refCount;
    ret->start = start;

    ret->next = prv->next;
    atomic_store(&prv->next, ret);

    return ret;
}

void c4_popNextRootSet(prs_t prv, void(*release)()) {
    prs_t r = atomic_exchange(&prv->next, NULL);
    if(!r->start) return;
    while(atomic_load(&r->lock));
    if(release) release(r);
}

_Atomic(void*)* c4_loadRefAddr(_Atomic(void*)* obj, size_t idx) {
    return c4_impl_loadValueBarrier(obj)->start + idx;
}

void c4_storeRef(_Atomic(void*)* obj, size_t idx, _Atomic(void*)* value) {
    atomic_store(c4_loadRefAddr(obj, idx), c4_impl_loadValueBarrier(value));
}

void* c4_load(_Atomic(void*)* obj, size_t offs) {
    return *(void**)((byte_t*)c4_impl_loadValueBarrier(obj)->start + offs);
}

void c4_store(_Atomic(void*)* obj, size_t offs, void* value) {
    *(void**)((byte_t*)c4_impl_loadValueBarrier(obj)->start + offs) = value;
}