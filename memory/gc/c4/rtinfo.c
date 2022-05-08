#include "impl.h"
#include "../../../process/thread.h"

size_t gcThrdCount = 0;
_Atomic(size_t) gcCollected = 0;

extern _Atomic(prs_t) gcRSIter;

int c4_init(size_t appThrdCount, size_t GCThrdCount, void* heap, size_t heapSize) {
    gcThrdCount = GCThrdCount;

    spaceSize = heapSize / 2;
    fromSpace = heap;
    brk = toSpace = fromSpace + spaceSize;

    size_t brsCount = appThrdCount;
    size_t brsSize = sizeof(rs_t) * brsCount;
    gcRSIter = basicrs = malloc(brsSize);
    if(!basicrs) return 1;
    for(int i = 0; i < brsCount; ++i) { 
        basicrs[i].next = basicrs + (i + 1);
        basicrs[i].lock = 0;
        basicrs[i].size = 0;
        basicrs[i].start = NULL;
    }
    basicrs[brsCount - 1].next = NULL;

    pthread_t t = { 0 };
    if(pthread_create(&t, NULL, c4_impl_phaseManager, NULL)) return 2;
    if(pthread_detach(t)) return 3;

    for(size_t i = 0; i < GCThrdCount; ++i) {
        if(pthread_create(&t, NULL, c4_impl_gcThrdFunc, NULL)) return 2;
        if(pthread_detach(t)) return 3;
    }

    return 0;
}

size_t c4_gcThrdCount() {
    return gcThrdCount;
}

size_t c4_allocated() {
    return atomic_load(&heapAllocated);
}

size_t c4_collected() {
    return atomic_load(&gcCollected);
}