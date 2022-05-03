#ifndef DYNA_LOCKLESS_IMPL_
#define DYNA_LOCKLESS_IMPL_

#include "stdafx.h"
#include "llgc.h"
#include "../../../process/thread.h"

typedef struct MemoryBlockInfo_t mbi_t;

struct MemoryBlockInfo_t {
    atomic_ptrdiff_t newAddr;
    atomic_int status;
    mbi_t* next;

    size_t size;
    size_t refCount;
    atomic_ptrdiff_t start[0];
};

struct RootSet_t {
    atomic_ptrdiff_t next;
    
    atomic_bool lock;
    size_t refCount;
    atomic_ptrdiff_t* start;
};

extern size_t gcThrdCount;
extern rs_t* rootSets;
extern size_t gcSpaceSize;
extern atomic_ptrdiff_t fromSpace, toSpace;
extern void* source;

extern _Atomic size_t collected;
extern _Atomic size_t allocated;

void llgc_impl_exchangeSpace();
size_t llgc_impl_exchangeAlloc();
int llgc_impl_testSpace(mbi_t*);
int llgc_impl_testSource(mbi_t*);

void* llgc_impl_sbrk(size_t size, size_t refCount, int countable);

void llgc_impl_pushWorkList(mbi_t* mbi);
mbi_t* llgc_impl_loadBarrier(atomic_ptrdiff_t* hdl);
void* llgc_impl_gcThrdFunc(void* arg);
void llgc_impl_main();

#endif