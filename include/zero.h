/* ================================================================ *
 * Copyright (c) 2022 Relight Team - Lizbing. All rights reserved.  *
 * Designed and implemented by Lizbing                              *
 * email: lizbing@relight.team                                      *
 * QQ: 2541666786                                                   *
 *                                                                  *
 * Zero Garbage Collector - A pauseless garbage collector.          *
 * Version: Beta 0.1                                                *
 * Generated on 19/6/2022                                           *
 * ================================================================ */

#ifndef ROSA_VIRTUAL_MACHINE_ZERO_GC_
#define ROSA_VIRTUAL_MACHINE_ZERO_GC_

#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <time.h>

typedef char byte_t;
typedef uint64_t ptr_t;
typedef _Atomic(ptr_t) atomic_ptr_t;

typedef struct zero_ref_t {
    uint8_t isReached : 1;
    ptr_t addr : 63;
} zero_ref_t;
typedef _Atomic(zero_ref_t) zero_root_t;

struct RootSet_t;
typedef struct RootSet_t RootSet_t, *pRootSet_t;

#define zero_null ((zero_ref_t){ 0 })
#define ZERO_SIZEOF_ROOTSET_T 32

float zero_version();
char* zero_rightInfo();
char* zero_stdName();

int zero_init(size_t appThrdCount, size_t gcThrdCount, void* heapStart, size_t heapSize);
size_t zero_thrdCount();
size_t zero_allocated();
size_t zero_collected();
int zero_routine();

pRootSet_t zero_getBasicRootSet(int thrdID);
pRootSet_t zero_newRootSet(void* addr, pRootSet_t prv, zero_root_t* start, size_t refCount);
void zero_deleteRootSet(pRootSet_t prv, void(*release)(void*));

zero_ref_t zero_new(zero_root_t* pRef, size_t size, size_t refCount);

void* zero_load(zero_root_t* ref, size_t offs);
zero_ref_t zero_loadRef(zero_root_t* ref, size_t idx);
void zero_store(zero_root_t* dst, size_t offs, void* value);
void zero_storeRefToObj(zero_root_t* dst, size_t idx, zero_root_t* src);
void zero_storeRefToRoot(zero_root_t* dst, zero_root_t* src, size_t idx);

#endif //ROSA_VIRTUAL_MACHINE_ZERO_GC_
