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

void** llgc_loadHdl(atomic_ptrdiff_t* hdl, size_t offs) {
    return ((byte_t*)llgc_impl_loadBarrier(hdl)->start) + offs;
}

void* llgc_load(atomic_ptrdiff_t* hdl, size_t offs) {
    return *llgc_loadHdl(hdl, offs);
}

void llgc_store(atomic_ptrdiff_t* hdl, size_t offs, void* value) {
    *llgc_loadHdl(hdl, offs) = value;
}