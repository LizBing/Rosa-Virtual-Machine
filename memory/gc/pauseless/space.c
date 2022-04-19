#include "allocator.h"

extern size_t spaceSize;
extern void* fromSpace;
extern void* toSpace;
pthread_rwlock_t fsrwl = PTHREAD_RWLOCK_INITIALIZER;

void* brk_peak = NULL;

void exchange() {
    pthread_rwlock_wrlock(&fsrwl);
    void* temp = fromSpace;
    brk_peak = fromSpace = toSpace;
    toSpace = temp;
    pthread_rwlock_unlock(&fsrwl);
}

Ref_t plgc_sbrk(size_t size) {
    static pthread_mutex_t mtx = PTHREAD_RWLOCK_INITIALIZER;
    pthread_mutex_lock(&mtx);
    void* temp = brk_peak;
    brk_peak += sizeof(Obj_t) + size;
    pthread_mutex_unlock(&mtx);
    return temp;
}
