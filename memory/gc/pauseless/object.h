#ifndef PAUSELESS_GC_OBJECT_
#define PAUSELESS_GC_OBJECT_

#include "stdafx.h"


typedef struct Object_t {
    pthread_rwlock_t rwl;
    // pthread_mutex_t mtx;
    _Bool reachable : 1;
    // 0: hasn't copied. 1: is being copied. 2: already
    byte_t status : 2;
    size_t size : 61;
    struct Object_t* newLoc;
    size_t refCount;
    void* start[0];
} Obj_t, *Ref_t;

#endif