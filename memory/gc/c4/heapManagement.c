#include "impl.h"

extern atomic_bool gcCopyFlag;

atomic_short toSpaceID = 0;
size_t spaceSize = 0;
byte_t* fromSpace = NULL;
_Atomic(byte_t*) toSpace = NULL;
_Atomic(byte_t*) brk = NULL;

_Atomic(size_t) heapAllocated = 0;

static atomic_size_t lock = 0;

size_t c4_impl_exchangeSpace() {
    atomic_store(&toSpaceID, !toSpaceID);
    size_t ret = atomic_exchange(&brk, fromSpace) - toSpace;
    void* temp = toSpace;
    atomic_store(&toSpace, fromSpace);
    fromSpace = temp;

    return ret;
}

pObject_t c4_impl_sbrk(size_t size, size_t refCount, int countable) {
    size_t brkSize = size + sizeof(Object_t);

    pObject_t ret = atomic_fetch_add(&brk, brkSize);
    int isIllegal = (byte_t*)ret + brkSize >= atomic_load(&toSpace) + spaceSize;
    if(isIllegal) return NULL;

    if(countable) atomic_fetch_add(&heapAllocated, brkSize);

    ret->newAddr = NULL;
    ret->next = NULL;
    ret->refCount = refCount;
    ret->size = size;
    ret->status = 0;
    ret->spaceID = atomic_load(&toSpaceID);

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

    if(!atomic_load(&gcCopyFlag)) { c4_impl_pushWorkList(ret); }
    return ret;
}