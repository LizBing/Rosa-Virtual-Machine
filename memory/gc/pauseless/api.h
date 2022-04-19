#ifndef PAUSELESS_GC_INFO_
#define PAUSELESS_GC_INFO_

#include "stdafx.h"
#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

int plgc_init(void*, size_t, size_t);
size_t plgc_thrdCount();
size_t plgc_heapSize();

size_t plgc_usedMemory();
size_t plgc_collectedMemory();
size_t plgc_allocatedMemory();
// clock_t plgc_timeCost();
void* plgc_load(Ref_t*, size_t);
void plgc_store(Ref_t*, size_t, void*);
Ref_t* plgc_loadRef(Ref_t*, size_t);
void plgc_storeRef(Ref_t*, size_t, Ref_t);

typedef struct RootSet_t RootSet_t;
struct RootSet_t {
    Ref_t* begin;
    size_t size;
    pthread_rwlock_t rwl;
    RootSet_t* next;
};

RootSet_t* plgc_pushRootSet(RootSet_t*, Ref_t* begin, size_t size);
void plgc_destroyRootSet(RootSet_t*);

#ifdef __cplusplus
}
#endif

#endif