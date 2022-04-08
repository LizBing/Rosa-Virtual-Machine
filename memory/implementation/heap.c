#include "heap.h"

size_t totalMemory = 0;
size_t restMemory = 0;

#define MINIMUM_BLOCKSIZE (sizeof(MemBlock_t*) * 2)
#define INFOSIZE (sizeof(MemBlock_t) - MINIMUM_BLOCKSIZE + sizeof(byte_t))
#define tag data[0]
#define ret data + 1

#define TAG_FREE 0

Heap_t* newHeap(size_t size) {
    Heap_t* heap = malloc(sizeof(Heap_t));
    if(heap == NULL) return NULL;
    
    heap->memory = malloc(size);
    if(heap->memory == NULL) {
        free(heap);
        return NULL;
    }
    
    totalMemory += size;
    heap->size = size;
    restMemory += heap->rest = size -= INFOSIZE;
    heap->start = heap->memory;
    heap->end = heap->memory;

    heap->memory->size = size;
    heap->memory->heap = heap;
    heap->memory->prev = 
    heap->memory->next = NULL;
    heap->memory->tag = TAG_FREE;

    pthread_mutex_init(&heap->mtx, NULL);

    return heap;
}

void deleteHeap(Heap_t* heap) {
    pthread_mutex_destroy(&heap->mtx);
    free(heap->memory);

    totalMemory -= heap->size;
    restMemory -= heap->rest;
    free(heap);
}

static void* treat(MemBlock_t* mb, size_t req, Heap_t* heap) {
    if(!mb) return NULL;
    if(mb->size - req > INFOSIZE + MINIMUM_BLOCKSIZE) {
        MemBlock_t* new = mb->ret + req;
        new->size = mb->size - req - INFOSIZE;
        new->heap = heap;
        new->prev = mb;
        new->next = mb->next;
        new->tag = TAG_FREE;

        if(mb->next) mb->next->prev = new;
        mb->next = new;
        mb->size = req;

        restMemory -= INFOSIZE;
        heap->rest -= INFOSIZE;
    }

    if(mb->prev) mb->prev->next = mb->next;
    if(mb->next) mb->next->prev = mb->prev;
    if(heap->start == mb) heap->start = mb->next;
    if(heap->end == mb) heap->end = mb->prev;

    restMemory -= mb->size;
    heap->rest -= mb->size;

    mb->tag = !TAG_FREE;
    return mb->ret;
}

void* firstFit(Heap_t* heap, size_t req) {
    if(req < MINIMUM_BLOCKSIZE) req = MINIMUM_BLOCKSIZE;

    pthread_mutex_lock(&heap->mtx);

    MemBlock_t* mb = heap->start;
    for(; mb->size < req; mb = mb->next);

    void* ret_val = treat(mb, req, heap);
    pthread_mutex_unlock(&heap->mtx);

    return ret_val;
}

void* worstFit(Heap_t* heap, size_t req) {
    if(req < MINIMUM_BLOCKSIZE) req = MINIMUM_BLOCKSIZE;

    pthread_mutex_lock(&heap->mtx);

    MemBlock_t* mb = heap->end;
    if(mb->size < req) {
        pthread_mutex_unlock(&heap->mtx);
        return NULL;
    }

    void* ret_val = treat(mb, req, heap);
    pthread_mutex_unlock(&heap->mtx);

    return ret_val;
}

static MemBlock_t* getHigher(MemBlock_t* l) {
    MemBlock_t* h = l->ret + l->size;
    if(h >= l->heap->memory + l->heap->size) return NULL;
    return h;
}

static MemBlock_t* getHigherFree(MemBlock_t* l) {
    MemBlock_t* h = getHigher(l);
    if(h == NULL) return NULL;
    if(h->tag == TAG_FREE) return h;
    return getHigherFree(h);
}

static void merge(MemBlock_t* l, MemBlock_t* h) {
    l->next = h->next;
    l->size += h->size + INFOSIZE;
    if(l->heap->end == h) l->heap->end = l;

    restMemory += INFOSIZE;
}

void release(void* p) {
    if(!p) return;
    MemBlock_t* mb = p - INFOSIZE;
    Heap_t* heap = mb->heap;
    size_t size = mb->size;

    pthread_mutex_lock(&heap->mtx);
    if(mb->tag == TAG_FREE) {
        pthread_mutex_unlock(&heap->mtx);
        return;
    }

    MemBlock_t* h = getHigherFree(mb);
    MemBlock_t* l = NULL;
    if(h) {
        l = h->prev;
        if(mb->ret + mb->size == h) merge(mb, h);
    } else {
        mb->prev = heap->end;
        if(heap->end) heap->end->next = mb;
        else heap->start = mb;
        heap->end = mb;
    }

    if(l) if(l->ret + l->size == mb) {
        merge(l, mb);
        mb = l;
    } else {
        mb->prev = NULL;
        mb->next = heap->start;
        if(heap->start) heap->start->prev = mb;
        heap->start = mb;
    }
    mb->tag = TAG_FREE;

    restMemory += size;
    heap->rest += size;

    pthread_mutex_unlock(&heap->mtx);
}