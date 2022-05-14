#ifndef DYNA_GC_PAUSELESS_
#define DYNA_GC_PAUSELESS_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <time.h>
#include <stdatomic.h>

typedef char byte_t;
typedef uint64_t ref_t, ptr_t;
typedef _Atomic(ref_t) atomic_ref_t;
typedef _Atomic(byte_t) atomic_byte_t;
typedef struct RootSet RootSet_t, *pRootSet_t;

int pl_init(int appThrdCount, int gcThrdCount, void* heapStart, size_t heapSize);
int pl_thrdCount();
size_t pl_allocated();
size_t pl_collected();
int pl_routineCount();
clock_t pl_appThrdPausedTime();     // us

#define PL_GC_SIZEOF_ROOTSET_T 32
#define null 0
pRootSet_t pl_getRootSet(int thrdID);
pRootSet_t pl_newRootSet(void* placement, pRootSet_t prv, atomic_ref_t* start, size_t refCount);
void pl_deleteRootSet(pRootSet_t prv, void(*dtor)());

ref_t pl_new(size_t, size_t refCount);

atomic_ref_t* pl_loadObjRef(atomic_ref_t*, size_t idx);
void pl_storeObjRef(atomic_ref_t*, size_t idx, atomic_ref_t* pValue);
void* pl_load(atomic_ref_t*, size_t offs);
void pl_store(atomic_ref_t*, size_t offs, void* value);

#ifdef __cplusplus
}
#endif 


#endif