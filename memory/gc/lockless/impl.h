#ifndef DYNA_LOCKLESS_IMPL_
#define DYNA_LOCKLESS_IMPL_

#include "stdafx.h"
#include "llgc.h"
#include "../../../process/thread.h"

typedef struct MemoryBlockInfo_t mbi_t;

struct MemoryBlockInfo_t {
    _Atomic(void*) newAddr;
    // bit 1: isMarked
    // bit 2: isCopying
    // bit 3: space ID
    atomic_int status;
    mbi_t* next;

    size_t size;
    size_t refCount;
    _Atomic(void*) start[0];
};

struct RootSet_t {
    _Atomic(void*) next;
    
    atomic_bool lock;
    size_t refCount;
    _Atomic(void*)* start;
};

extern size_t gcThrdCount;
extern rs_t* rootSets;
extern size_t gcSpaceSize;
extern _Atomic(void*) fromSpace, toSpace;
extern void* source;

extern _Atomic size_t collected;
extern _Atomic size_t allocated;

void llgc_impl_exchangeSpace();
size_t llgc_impl_exchangeAlloc();
int llgc_impl_testSpace(mbi_t*);
int llgc_impl_testSource(mbi_t*);

void* llgc_impl_sbrk(size_t size, size_t refCount, int countable);

void llgc_impl_pushWorkList(mbi_t* mbi);
mbi_t* llgc_impl_loadBarrier(_Atomic(void*)* hdl);
void* llgc_impl_gcThrdFunc(void* arg);
void llgc_impl_main();

#endif