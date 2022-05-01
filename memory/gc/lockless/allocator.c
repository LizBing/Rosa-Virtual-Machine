#include "impl.h"

size_t gcSpaceSize = 0;
volatile atomic_ptrdiff_t fromSpace, toSpace;
static volatile atomic_ptrdiff_t peak;

void plgc_impl_exchangeSpace() {
    void* temp = fromSpace;
    atomic_store(&fromSpace, toSpace);
    atomic_store(&toSpace, temp);
    atomic_store(&peak, toSpace);
}

void* plgc_malloc(size_t size, size_t refCount) {
    size_t brkSize = size + sizeof(mbi_t);
    mbi_t* addr = atomic_fetch_add(&peak, brkSize);
    if(addr + size >= atomic_load(&toSpace) + gcSpaceSize) return NULL;

    addr->refCount = refCount;
    addr->next = NULL;
    addr->newAddr = NULL;
    addr->size = size;
    addr->status = 0;

    return addr;
}

// returns 0 when p is in fromSpace, and 1 in toSpace. 
// p is a legal pointer which means it isn't out of heap
int plgc_impl_testSpace(mbi_t* p) {
    void* base = atomic_load(&fromSpace);
    if(base <= p && p < base + gcSpaceSize) return 0;
    return 1;
}