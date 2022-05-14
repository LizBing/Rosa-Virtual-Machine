#include "impl.h"
#include <pthread.h>

extern _Atomic(ptr_t) heapPeak;
extern void* fromSpace, * toSpace;
extern size_t spaceSize;

int gcThrdCount = 0;
byte_t expectedNMTValue = 0;

int pl_init(int appThrdCount, int _gcThrdCount, void* heapStart, size_t heapSize) {
    gcThrdCount = _gcThrdCount;

    spaceSize = heapSize / 2;
    fromSpace = heapStart;
    heapPeak = toSpace = fromSpace + spaceSize;

    size_t basicRootSetsSize = sizeof(RootSet_t) * appThrdCount;
    basicRootSets = malloc(basicRootSetsSize);
    if(!basicRootSets) return 1;
    memset(basicRootSets, 0, basicRootSetsSize);

    for(int i = 0; i < appThrdCount; ++i) 
        basicRootSets[i].next = basicRootSets + (i + 1);
    basicRootSets[appThrdCount - 1].next = NULL;

    pthread_t t = { 0 };
    if(pthread_create(&t, NULL, pl_impl_manager, NULL)) return 2;
    pthread_detach(t);
    
    for(int i = 0; i < _gcThrdCount; ++i) {
        if(pthread_create(&t, NULL, pl_impl_thrdFunc, NULL)) return 2;
        pthread_detach(t);
    }

    return 0;
}

int pl_thrdCount() {
    return gcThrdCount;
}