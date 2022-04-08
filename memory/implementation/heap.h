#ifndef DYNA_HEAP_H
#define DYNA_HEAP_H

#include "../../stdafx.h"
#include <pthread.h>
typedef struct MemBlock_t MemBlock_t;

typedef struct {
    MemBlock_t* memory;
    size_t size;
    size_t rest;
    MemBlock_t* start;
    MemBlock_t* end;
    pthread_mutex_t mtx;
} Heap_t;

struct MemBlock_t {
    size_t size;
    Heap_t* heap;
    byte_t data[0];
    MemBlock_t* prev;
    MemBlock_t* next;
};

Heap_t* newHeap(size_t size);
void deleteHeap(Heap_t* heap);
void* firstFit(Heap_t* heap, size_t size);
void* worstFit(Heap_t* heap, size_t size);
void release(void* ptr);


#endif // DYNA_HEAP_H