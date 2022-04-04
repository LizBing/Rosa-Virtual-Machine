#ifndef KAORI_HEAP_
#define KAORI_HEAP_

#include "../../stdafx.h"
#include <pthread.h>

enum {
    EDEN = 0,
    SURV = 1,
    OLDGEN = 2,
};


#ifdef __cplusplus
extern "C" {
#endif 


void memInit(size_t, size_t, size_t);
void* vmalloc(size_t);
void* gcalloc(size_t);
void gcfree(void*);
int isLegal(void* mb, int genNo);

extern void* heap;
extern size_t heapSize;
extern size_t restMemory;

extern size_t mediumSize;
extern size_t largeSize;

extern void* gen[3];
extern size_t genSize[3];

extern void* freeList[3];
extern void* flEnd[3];
extern pthread_mutex_t flMtx[3];
extern void* toSurv;

#ifdef __cpluscplus
}
#endif

#endif