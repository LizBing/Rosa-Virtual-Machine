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
int llgc_timeCount();

_Atomic(void*)* llgc_loadRef(_Atomic(void*)* hdl, size_t idx);
void llgc_storeRef(_Atomic(void*)* hdl, size_t idx, void* value);
void* llgc_load(_Atomic(void*)* hdl, size_t offs);
void llgc_store(_Atomic(void*)* hdl, size_t offs, void* value);


#ifdef __cplusplus
}
#endif

#endif