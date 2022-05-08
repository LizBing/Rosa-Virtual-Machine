#ifndef DYNA_C4_
#define DYNA_C4_

#ifdef __cplusplus
extern "C" {
#endif

typedef char byte_t;
typedef struct RootSet_t rs_t, *prs_t;
#define C4_ROOTSET_SIZE 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

int c4_init(size_t appThrdCount, size_t GCThrdCount, void* heap, size_t heapSize);
size_t c4_gcThrdCount();
size_t c4_allocated();
size_t c4_collected();

prs_t c4_getRootSet(size_t thrdID);
prs_t c4_pushRootSet(void* placement, prs_t prv, _Atomic(void*)* start, size_t refCount);
void c4_popNextRootSet(prs_t prv, void(*release)());

void* c4_malloc(size_t, size_t refCount);
size_t c4_gcRoutine();

_Atomic(void*)* c4_loadRefAddr(_Atomic(void*)* obj, size_t idx);
void c4_storeRef(_Atomic(void*)* obj, size_t idx, _Atomic(void*)* value);
void* c4_load(_Atomic(void*)* obj, size_t offs);
void c4_store(_Atomic(void*)* obj, size_t offs, void* value);

#ifdef __cplusplus
}
#endif

#endif