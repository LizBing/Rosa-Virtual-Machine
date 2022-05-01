#include "allocator.h"

extern size_t spaceSize;
extern void* fromSpace;
extern void* toSpace;
pthread_rwlock_t fsrwl = PTHREAD_RWLOCK_INITIALIZER;

static size_t allocatedMemory = 0;
void* brk_peak = NULL;

void exchange() {
    pthread_rwlock_wrlock(&fsrwl);
    void* temp = fromSpace;
    brk_peak = fromSpace = toSpace;
    toSpace = temp;
    pthread_rwlock_unlock(&fsrwl);
}

Ref_t plgc_sbrk(size_t size, size_t refCount) {
    static pthread_mutex_t mtx = PTHREAD_RWLOCK_INITIALIZER;

    pthread_mutex_lock(&mtx);

    size_t alloc_size = sizeof(Obj_t) + size;
    Ref_t temp = brk_peak;
    brk_peak += alloc_size;
    allocatedMemory += alloc_size;
    
    pthread_mutex_unlock(&mtx);

    temp->refCount = refCount;
    return temp;
}

size_t plgc_usedMemory() {
    return brk_peak - fromSpace;
}

size_t plgc_allocatedMemory() {
    return allocatedMemory;
}
