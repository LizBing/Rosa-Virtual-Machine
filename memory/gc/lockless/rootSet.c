#include "impl.h"
// #include "../../../process/thread.h"

static rs_t* rootSets = NULL;

rshdl_t plgc_getRootSet(int thrdId) {
    return rootSets + thrdId;
}

rshdl_t plgc_pushRootSet(rshdl_t prv, void* start, size_t refCount) {
    rshdl_t new = malloc(sizeof(rs_t));
    if(!new) return NULL;

    new->next = NULL;
    new->refCount = refCount;
    new->start = start;

    atomic_store(&prv->next, new);
}

void plgc_popNextRootSet(rshdl_t node) {
    rshdl_t r = atomic_exchange_explicit(&node->next, NULL, memory_order_release);
    while(atomic_load(&r->lock)); free(r);
}

void* plgc_load(mbi_t** );