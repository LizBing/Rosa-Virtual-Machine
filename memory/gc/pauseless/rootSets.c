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

extern size_t spaceSize;
extern void* fromSpace;
extern pthread_rwlock_t fsrwl;

Ref_t compact(Ref_t ref);

Ref_t plgc_loadBarrier(Ref_t* pRef) {
    Ref_t ref = *pRef;
    if(!ref) return NULL;
    pthread_rwlock_rdlock(&fsrwl);
    if(fromSpace <= ref && ref <= fromSpace + spaceSize) {
        pthread_rwlock_unlock(&fsrwl);
        pthread_mutex_lock(&ref->mtx);
        return ref->start;
    } pthread_rwlock_unlock(&fsrwl);

    *pRef = compact(ref);
    pthread_mutex_lock(&pRef[0]->mtx);
    return *pRef;
}

void* plgc_load(Ref_t* pRef, size_t offs) {
    Ref_t ref = plgc_loadBarrier(pRef);
    void** loc = (void*)ref->start + offs;
    pthread_mutex_unlock(&ref->mtx);
    return *loc;
}

void plgc_store(Ref_t* pRef, size_t offs, void* value) {
    Ref_t ref = plgc_loadBarrier(pRef);
    void** loc = (void*)ref->start + offs;
    *loc = value;
    pthread_mutex_unlock(&ref->mtx);
}

Ref_t* plgc_loadRef(Ref_t* pRef, size_t idx) {
    Ref_t ref = plgc_loadBarrier(pRef);
    Ref_t* ret = ref->start + idx;
    pthread_mutex_unlock(&ref->mtx);
    return ret;
}
void plgc_storeRef(Ref_t* pRef, size_t idx, Ref_t value) {
    Ref_t ref = plgc_loadBarrier(pRef);
    ref->start[idx] = value;
    pthread_mutex_unlock(&ref->mtx);
}