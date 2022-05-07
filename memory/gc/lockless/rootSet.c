#include "impl.h"
// #include "../../../process/thread.h"

rs_t* rootSets;

rshdl_t llgc_getRootSet(int thrdId) {
    return rootSets + thrdId;
}

rshdl_t llgc_pushRootSet(rshdl_t prv, void** start, size_t refCount) {
    rshdl_t new = malloc(sizeof(rs_t));
    if(!new) return NULL;

    new->next = NULL;
    new->refCount = refCount;
    new->start = start;

    atomic_store(&prv->next, new);
}

void llgc_popNextRootSet(rshdl_t node) {
    rshdl_t r = atomic_exchange_explicit(&node->next, NULL, memory_order_release);
    while(atomic_load(&r->lock)); free(r);
}

_Atomic(void*)* llgc_loadRef(_Atomic(void*)* hdl, size_t idx) {
    return llgc_impl_loadBarrier(hdl)->start + idx;
}

void llgc_storeRef(_Atomic(void*)* hdl, size_t idx, void* value) {
    mbi_t* mbi = NULL;
    if(atomic_load(hdl)) mbi = llgc_impl_loadBarrier(hdl);
    else printf("fuck\n");
    // printf("afer: %p, %p\n", atomic_load(hdl), mbi);
    atomic_store(mbi->start + idx, value);
}

void* llgc_load(_Atomic(void*)* hdl, size_t offs) {
    return (byte_t*)llgc_impl_loadBarrier(hdl)->start + offs;
}

void llgc_store(_Atomic(void*)* hdl, size_t offs, void* value) {
    *(void**)((byte_t*)llgc_impl_loadBarrier(hdl)->start + offs) = value;
}