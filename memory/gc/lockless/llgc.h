#ifndef DYNA_LOCKLESS_GC_
#define DYNA_LOCKLESS_GC_

typedef struct RootSet_t rs_t, *rshdl_t;
#include "stdafx.h"

#ifdef __cplusplus
extern "C" {
#endif

int llgc_init(size_t GCThrdCount, size_t heapSize, void* heapStart);
size_t llgc_thrdCount();
size_t llgc_allocated();
size_t llgc_collected();

rshdl_t llgc_getRootSet(int thrdId);
rshdl_t llgc_pushRootSet(rshdl_t prv, void** start, size_t refCount);
void llgc_popNextRootSet(rshdl_t node);

void* llgc_malloc(size_t size, size_t refCount);
size_t llgc_allocated();
size_t llgc_collected();

void** llgc_loadRef(atomic_ptrdiff_t* hdl, size_t offs);
void* llgc_load(atomic_ptrdiff_t* hdl, size_t offs);
void llgc_store(atomic_ptrdiff_t* hdl, size_t offs, void* value);


#ifdef __cplusplus
}
#endif

#endif