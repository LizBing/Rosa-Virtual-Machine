#include "api.h"

RootSet_t* rootSets = NULL;
size_t rootSetsSize = 0;

RootSet_t* plgc_pushRootSet(RootSet_t* rs, Ref_t* begin, size_t size) {
    RootSet_t* new = malloc(sizeof(RootSet_t));
    if(!new) return NULL;

    new->begin = begin;
    new->size = size;
    new->next = NULL;
    pthread_rwlock_init(&new->rwl, NULL);

    pthread_rwlock_wrlock(&rs->rwl);
    rs->next = new;
    pthread_rwlock_wrunlock(&rs->rwl);

    return new;
}

void plgc_destroyRootSet(RootSet_t* prev) {
    RootSet_t* rb = prev->next;

    pthread_rwlock_wrlock(&prev->rwl);
    prev->next = NULL;
    pthread_rwlock_wrunlock(&prev->rwl);

    pthread_rwlock_destroy(&rb->rwl);
    free(rb);
}