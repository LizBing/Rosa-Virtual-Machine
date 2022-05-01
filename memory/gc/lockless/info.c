#include "impl.h"

size_t gcThrdCount = 0;
size_t plgc_thrdCount() {}

int plgc_init(size_t GCThrdCount, size_t heapSize, void* heapStart) {
    gcThrdCount = GCThrdCount;

    rootSets = malloc(sizeof(rs_t) * getMaxThrdCount());
    if(!rootSets) return 1;

    gcSpaceSize = heapSize / 2;
    fromSpace = heapStart;
    toSpace = fromSpace + gcSpaceSize;

    return 0;
}