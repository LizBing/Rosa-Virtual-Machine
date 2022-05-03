#include "impl.h"

size_t gcThrdCount = 0;
size_t llgc_thrdCount() {
    return gcThrdCount;
}

int llgc_init(size_t GCThrdCount, size_t heapSize, void* heapStart) {
    gcThrdCount = GCThrdCount;

    rootSets = malloc(sizeof(rs_t) * getMaxThrdCount());
    memset(rootSets, 0, sizeof(rs_t) * getMaxThrdCount());
    if(!rootSets) return 1;

    gcSpaceSize = heapSize / 2;
    fromSpace = heapStart;
    source = heapStart;
    toSpace = fromSpace + gcSpaceSize;
    
    llgc_impl_exchangeAlloc();
    llgc_impl_exchangeSpace();

    pthread_t t = { 0 };
    pthread_create(&t, NULL, llgc_impl_main, NULL);
    pthread_detach(t);
    usleep(500);
    
    for(int i = 0; i < GCThrdCount; ++i) {
        pthread_create(&t, NULL, llgc_impl_gcThrdFunc, NULL);
        pthread_detach(t);
    }

    return 0;
}

_Atomic size_t collected = 0;
_Atomic size_t allocated = 0;

size_t llgc_allocated() {
    return atomic_load(&allocated);
}

size_t llgc_collected() {
    return atomic_load(&collected);
}