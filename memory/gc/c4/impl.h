#ifndef C4_IMPL_
#define C4_IMPL_

#include "c4.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Object Object_t, *pObject_t;
struct Object {
    /* @info
     * bool isPushed : 1;
     * bool lock : 1;
     * int spaceID : 1;
     */ 
    // atomic_int info;
    atomic_bool status;
    _Atomic(pObject_t) newAddr;
    _Atomic(pObject_t) next;
    size_t size;
    size_t refCount;
    _Atomic(pObject_t) start[0];
};

extern rs_t* basicrs;
struct RootSet_t {
    atomic_bool lock;

    _Atomic(pObject_t)* start;
    size_t size;
    _Atomic(prs_t) next;
};

extern byte_t* fromSpace, *toSpace;
extern size_t spaceSize;
extern _Atomic(byte_t*) brk;

extern size_t gcThrdCount;
extern _Atomic(size_t) gcCollected;
extern _Atomic(size_t) heapAllocated;

size_t c4_impl_exchangeSpace();

void c4_impl_pushWorkList(pObject_t);

pObject_t c4_impl_sbrk(size_t size, size_t refCount, int countable);

void c4_impl_phaseManager();
void c4_impl_gcThrdFunc();
pObject_t c4_impl_loadValueBarrier(_Atomic(pObject_t)*);

#ifdef __cplusplus
}
#endif

#endif