#include "impl.h"

extern atomic_bool gcCopyFlag;

size_t spaceSize = 0;
byte_t* fromSpace = NULL;
byte_t* toSpace = NULL;
_Atomic(byte_t*) brk = NULL;

_Atomic(size_t) heapAllocated = 0;

static atomic_size_t lock = 0;

size_t c4_impl_exchangeSpace() {
    const int zero = 0;
    while(!atomic_compare_exchange_weak(&lock, &zero, -1));
    size_t delta = brk - toSpace;

    void* temp = toSpace;
    brk = toSpace = fromSpace;
    fromSpace = toSpace;

    atomic_store(&lock, 0);
    return delta;
}

pObject_t c4_impl_sbrk(size_t size, size_t refCount, int countable) {
    size_t brkSize = size + sizeof(Object_t);

    while(atomic_load(&lock) == -1);
    atomic_fetch_add(&lock, 1);

    pObject_t ret = atomic_fetch_add(&brk, brkSize);
    int isIllegal = (byte_t*)ret + brkSize >= toSpace + spaceSize;

    atomic_fetch_add(&lock, -1);

    if(isIllegal) return NULL;

    if(countable) atomic_fetch_add(&heapAllocated, brkSize);

    ret->newAddr = NULL;
    ret->next = NULL;
    ret->refCount = refCount;
    ret->size = size;
    ret->status = 0;

    return ret;
}

void* c4_malloc(size_t size, size_t refCount) {
    int gcr = 0;
    pObject_t ret = c4_impl_sbrk(size, refCount, 1);
    if(!ret) {
        gcr = c4_gcRoutine();
        while(c4_gcRoutine() == gcr);
        if(!(ret = c4_impl_sbrk(size, refCount, 1))) return NULL;  
    }

    if(!atomic_load(&gcCopyFlag)) c4_impl_pushWorkList(ret);
    return ret;
}