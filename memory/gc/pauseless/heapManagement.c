#include "impl.h"
#include <stdio.h>
#include <pthread.h>

extern atomic_bool gcCopyFlag;

atomic_ref_t heapPeak = null;
void* fromSpace = NULL,
    * toSpace = NULL;
size_t spaceSize = 0;

static ref_t edge = null;

static atomic_size_t gcAllocated = 0;
size_t pl_allocated() {
    return atomic_load(&gcAllocated);
}

inline void* loadPtr(ref_t ref) {
    return ref >> 1;
}

inline ref_t ptr2ref(void* value) {
    return ((uint64_t)value << 1) | expectedNMTValue;
}

inline int isExpected(ref_t ref) {
    return expectedNMTValue == (ref & 1);
}

static pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void pl_impl_heapLock() {
    pthread_rwlock_rdlock(&lock);
}

void pl_impl_heapUnlock() {
    pthread_rwlock_unlock(&lock);
}

ptrdiff_t pl_impl_exchangeSpace() {
    pthread_rwlock_wrlock(&lock);

    expectedNMTValue = !expectedNMTValue;
    ptrdiff_t ret = atomic_exchange(&heapPeak, fromSpace) - (uint64_t)toSpace;
    edge = fromSpace + spaceSize;

    atomic_store(&gcCopyFlag, 1);

    pthread_rwlock_unlock(&lock);
    
    void* temp = toSpace;
    toSpace = fromSpace;
    fromSpace = temp;

    return ret;
}

void* pl_impl_sbrk(size_t size) {
    void* ret = atomic_fetch_add(&heapPeak, size);
    if(ret + size >= edge) return NULL;

    return ret;
}

static ref_t pl_new_once(size_t size, size_t refCount) {
    size_t brkSize = size + sizeof(Object_t);
    pl_impl_heapLock();

    pObject_t obj = pl_impl_sbrk(brkSize);
    if(!obj) { pl_impl_heapUnlock(); return NULL; }
    ref_t ret = ptr2ref(obj);

    obj->size = size;
    obj->refCount = refCount;
    obj->status = 0;
    obj->newAddr = NULL;
    
    if(!atomic_load(&gcCopyFlag))
        pl_impl_pushWorkList(obj);
    pl_impl_heapUnlock();

    atomic_fetch_add(&gcAllocated, brkSize);

    return ret;
}

ref_t pl_new(size_t size, size_t refCount) {
    ref_t ret = pl_new_once(size, refCount);
    if(ret) return ret;

    int gcr = pl_routineCount();
    while(pl_routineCount() == gcr);

    return pl_new_once(size, refCount);
}