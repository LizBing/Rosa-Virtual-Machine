#include "impl.h"

size_t gcThrdCount = 0;
size_t llgc_thrdCount() {}

int llgc_init(size_t GCThrdCount, size_t heapSize, void* heapStart) {
    gcThrdCount = GCThrdCount;

    rootSets = malloc(sizeof(rs_t) * getMaxThrdCount());
    if(!rootSets) return 1;

    gcSpaceSize = heapSize / 2;
    source = fromSpace = heapStart;
    toSpace = fromSpace + gcSpaceSize;
    
    for(int i = 1; i < GCThrdCount; ++i) {
        pthread_t t = { 0 };
        pthread_create(&t, NULL, llgc_impl_gcThrdFunc, NULL);
        pthread_detach(t);
    }

    pthread_t t = { 0 };
    pthread_create(&t, NULL, llgc_impl_main, NULL);
    pthread_detach(t);

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