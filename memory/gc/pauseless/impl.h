#ifndef DYNA_GC_PAUSELESS_IMPL_
#define DYNA_GC_PAUSELESS_IMPL_

#include "pauseless.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct Object Object_t, *pObject_t;
struct Object {
    atomic_bool status;
    atomic_ref_t newAddr;
    pObject_t workListNext;

    size_t size;
    size_t refCount;

    atomic_ref_t ref[0];
    byte_t start[0];
};

extern byte_t expectedNMTValue;

void* loadPtr(ref_t ref);
ref_t ptr2ref(void* value);
int isExpected(ref_t ref);

struct RootSet {
    atomic_ref_t* start;
    size_t refCount;

    atomic_bool lock;
    _Atomic(pRootSet_t) next;
};

extern int gcThrdCount;

extern RootSet_t* basicRootSets;

ptrdiff_t pl_impl_exchangeSpace();
void* pl_impl_sbrk(size_t size);
void pl_impl_heapLock();
void pl_impl_heapUnlock();

void pl_impl_pushWorkList(pObject_t);
pObject_t pl_impl_loadValueBarrier(atomic_ref_t* pRef);
void pl_impl_manager();
void pl_impl_thrdFunc();

#endif