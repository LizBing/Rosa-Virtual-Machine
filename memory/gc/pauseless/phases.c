#include "api.h"
#include "allocator.h"

extern size_t spaceSize;
extern void* fromSpace;
extern void* toSpace;

extern RootSet_t* rootSets;
extern size_t rootSetsSize;

int gc_signal = 0;

size_t collectedMemory = 0;

size_t plgc_collectedMemory() {
    return collectedMemory;
}

void gc_start() {
    gc_signal = 1;
}

#define synchronize(b) \
    pthread_mutex_lock(&sychMtx); \
    if(++sychCount == plgc_thrdCount()) { \
        b = 1; \
        sychCount = 0; \
    } \
    pthread_mutex_unlock(&sychMtx); \
    while(!b); 

void mark(RootSet_t* h);
void remap(RootSet_t* t);
Ref_t compact(Ref_t ref);

void plgc_major() {
    static volatile size_t sychCount = 0;

    static int lable = 0;
    static pthread_mutex_t sychMtx = PTHREAD_MUTEX_INITIALIZER;

    static size_t idx = 0;
    static pthread_mutex_t idxMtx = PTHREAD_MUTEX_INITIALIZER;
    volatile static int isMarking = 0;
    volatile static int isRemapping = 0;

    volatile static int isCompacting = 0;
    volatile static Ref_t fpeak = NULL;
    static pthread_mutex_t fpMtx = PTHREAD_MUTEX_INITIALIZER;
start:
    pthread_mutex_lock(&sychMtx);
    if(!lable) { 
        lable = 1; 
        exchange(); 
        fpeak = toSpace;
    }
    pthread_mutex_unlock(&sychMtx);
    synchronize(isMarking);
    pthread_mutex_lock(&sychMtx);
    if(lable) lable = 0;
    pthread_mutex_unlock(&sychMtx);

    while(!gc_signal);
    do {
        pthread_mutex_lock(&idxMtx);
        if(idx == rootSetsSize) {
            idx = 0;
            isMarking = 0;
            pthread_mutex_unlock(&idxMtx);
            break;
        } 
        size_t temp = idx++;
        pthread_mutex_unlock(&idxMtx);
        mark(rootSets + temp);
    } while(isMarking);

    synchronize(isCompacting);
    do {
        pthread_mutex_lock(&fpMtx);
        if(!fpeak->size || fpeak >= fromSpace + spaceSize) {
            pthread_mutex_unlock(&fpMtx);
            isCompacting = 0;
        } else {
            Ref_t r = fpeak;
            fpeak = (byte_t*)fpeak->start + fpeak->size;
            pthread_mutex_unlock(&fpMtx);

            r = compact(r);
            for(size_t i = 0; i < r->refCount; ++i) {
                Ref_t* pSub = r->start + i;
                if(!*pSub) continue;
                if(fromSpace <= *pSub && *pSub < fromSpace + spaceSize) continue;
                *pSub = compact(*pSub);
            }
        }
    } while(isCompacting);

    synchronize(isRemapping);
    do {
        pthread_mutex_lock(&idxMtx);
        if(idx == rootSetsSize) {
            idx = 0;
            isMarking = 0;
            pthread_mutex_unlock(&idxMtx);
            break;
        } 
        size_t temp = idx++;
        pthread_mutex_unlock(&idxMtx);
        remap(rootSets + temp);
    } while(isRemapping);

    goto start;
}

void remap(RootSet_t* t) {
    if(!t) return;
    pthread_rwlock_wrlock(&t->rwl);
    for(size_t i = 0; i < t->size; ++i) {
        if(!t->begin[i]) return;
        t->begin[i] = t->begin[i]->newLoc;
    }
    pthread_rwlock_unlock(&t->rwl);
    remap(t->next);
}

Ref_t compact(Ref_t ref) {
    if(!ref) return NULL;

    pthread_rwlock_rdlock(&ref->rwl);
    if(0b01 & ref->status) {
        pthread_rwlock_unlock(&ref->rwl);
        while(1 == ref->status);
        return ref->newLoc;
    }
    if(0b10  & ref->status) {
        pthread_rwlock_unlock(&ref->rwl);
        return ref->newLoc;
    } pthread_rwlock_unlock(&ref->rwl);

    pthread_rwlock_wrlock(&ref->rwl);
    ref->status &= 0b01;
    pthread_rwlock_unlock(&ref->rwl);
    
    ref->newLoc = plgc_sbrk(ref->size, ref->refCount);
    memcpy(ref->newLoc->start, ref->start, ref->size);
    collectedMemory += ref->size + sizeof(Obj_t);
    ref->status &= 0b10;

    return ref->newLoc;
}

void objMarking(Ref_t ref) {
    if(!ref) return;
    if(fromSpace <= ref && ref < fromSpace + spaceSize) return;

    pthread_rwlock_rdlock(&ref->rwl);
    if(ref->reachable) { pthread_rwlock_unlock(&ref->rwl); return; }
    for(size_t i = 0; i < ref->refCount; ++i) {
        objMarking(ref->start[i]);
    }
    pthread_rwlock_unlock(&ref->rwl);

    pthread_rwlock_wrlock(&ref->rwl);
    ref->reachable = 1;
    pthread_rwlock_unlock(&ref->rwl);
}

void mark(RootSet_t* h) {
    if(!h) return;
    pthread_rwlock_rdlock(&h->rwl);
    for(size_t i = 0; i < h->size; ++i) {
        objMarking(h->begin[i]);
    }
    pthread_rwlock_unlock(&h->rwl);
    mark(h->next);
}