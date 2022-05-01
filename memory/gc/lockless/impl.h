#ifndef DYNA_PAUSELESS_IMPL_
#define DYNA_PAUSELESS_IMPL_

#include "stdafx.h"
#include "plgc.h"
#include "../../../process/thread.h"

typedef struct MemoryBlockInfo_t mbi_t;

struct MemoryBlockInfo_t {
    mbi_t* newAddr;
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
extern volatile atomic_ptrdiff_t fromSpace, toSpace;

void plgc_impl_exchangeSpace();
plgc_impl_testSpace(mbi_t*);

#endif