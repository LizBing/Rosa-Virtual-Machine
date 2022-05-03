#include "impl.h"

size_t gcSpaceSize = 0;
atomic_ptrdiff_t fromSpace, toSpace;
atomic_ptrdiff_t source;
static atomic_ptrdiff_t peak;

void llgc_impl_exchangeSpace() {
    void* temp = fromSpace;
    atomic_store(&fromSpace, toSpace);
    atomic_store(&toSpace, temp);
}

size_t llgc_impl_exchangeAlloc() {
    return atomic_exchange(&peak, toSpace) - atomic_exchange(&source, toSpace);
}

void* llgc_impl_sbrk(size_t size, size_t refCount, int countable) {
    size_t brkSize = size + sizeof(mbi_t);
    mbi_t* addr = atomic_fetch_add(&peak, brkSize);
    if(addr + brkSize >= atomic_load(&source) + gcSpaceSize) return NULL;

    addr->refCount = refCount;
    addr->next = NULL;
    addr->newAddr = NULL;
    addr->size = size;
    addr->status = 0;

    if(!llgc_impl_testSpace(addr)) llgc_impl_pushWorkList(addr);
    if(countable) atomic_fetch_add(&allocated, brkSize);

    return addr;
}

void* llgc_malloc(size_t size, size_t refCount) {
    return llgc_impl_sbrk(size, refCount, 1);
}

// returns 0 when p is in fromSpace, and 1 in toSpace. 
// p is a legal pointer which means it isn't out of heap
int llgc_impl_testSpace(mbi_t* p) {
    void* base = atomic_load(&fromSpace);
    if(base <= p && p < base + gcSpaceSize) return 0;
    return 1;
}

int llgc_impl_testSource(mbi_t* p) {
    void* base = atomic_load(&source);
    if(base <= p && p < base + gcSpaceSize) return 1;
    return 0;
}