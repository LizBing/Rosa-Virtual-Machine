#include "impl.h"

atomic_bool gcCopyFlag = 0;
static atomic_bool remapFlag = 0;

static _Atomic(size_t) completedCount = 0;

_Atomic(prs_t) gcRSIter = NULL;

static _Atomic(pObject_t) workListIter = NULL;
static pObject_t workListBegin = NULL;
static _Atomic(pObject_t) workListEnd = NULL;

static _Atomic(pObject_t) remapListIter = NULL;
// static pObject_t remapListBegin = NULL;
static _Atomic(pObject_t) remapListEnd = NULL;

static _Atomic(size_t) copied = 0;

static _Atomic(size_t) routine = 0;
size_t c4_gcRoutine() {
    return atomic_load(&routine);
}

void c4_impl_phaseManager() {
    while(1) {
        while(atomic_load(&completedCount) != gcThrdCount);
        atomic_store(&remapFlag, 0);
        completedCount = 0;

        atomic_fetch_add(&routine, 1);
        workListIter = workListBegin;
        gcRSIter = basicrs;
        size_t delta = c4_impl_exchangeSpace();
        
        // printf("copying phase start!\n");
        atomic_store(&gcCopyFlag, 1);
        while(atomic_load(&completedCount) != gcThrdCount);
        atomic_store(&gcCopyFlag, 0);
        completedCount = 0;
        // printf("copying phase finished!\n");
        
        atomic_fetch_add(&gcCollected, delta - copied);
        copied = 0;
        workListEnd = remapListEnd = NULL;

        // printf("remapping & marking phases start!\n");
        atomic_store(&remapFlag, 1);
        // while(atomic_load(&completedCount) != gcThrdCount);
    }
}

void c4_impl_pushWorkList(pObject_t obj) {
    printf("join %p to work list\n", obj); 
    if(atomic_fetch_or(&obj->status, 1)) return;
    pObject_t prv = atomic_exchange(&workListEnd, obj);
    if(!prv) { workListIter = workListBegin = obj; return; }
    prv->next = obj;
    obj->next = NULL;
}

static void pushRemapList(pObject_t obj) {
    if(atomic_fetch_or(&obj->status, 1)) return;
    pObject_t prv = atomic_exchange(&remapListEnd, obj);
    if(!prv) { remapListIter = obj; return; }
    prv->next = obj;
    obj->next = NULL;
}

pObject_t c4_impl_loadValueBarrier(_Atomic(pObject_t)* addr) {
    pObject_t obj = NULL, new = NULL;
    int tag = 0;
    do {
lable:
        if(!(obj = atomic_load(addr))) return NULL;
        if(atomic_load(&toSpaceID) == obj->spaceID) return obj;

        if(atomic_load(&gcCopyFlag)) {
            if(!atomic_fetch_and(&obj->status, 0)) {
                while(!(new = atomic_load(&obj->newAddr)));
                continue;
            }
            new = c4_impl_sbrk(obj->size, obj->refCount, 0);
            if(!new) exit(1);
            memcpy(new->start, obj->start, obj->size);
            atomic_fetch_add(&copied, obj->size + sizeof(Object_t));
            atomic_store(&obj->newAddr, new);
        } else
        if(atomic_load(&remapFlag)) {
            new = obj->newAddr;
            tag = 1;
        } else goto lable;
    } while(atomic_load(addr) != obj);
    if(tag) c4_impl_pushWorkList(new);

    return new;
}

static void markWorkList() {
    pObject_t obj = NULL, next = NULL;
    while(obj = atomic_load(&workListIter)) {
        next = atomic_load(&obj->next);
        if(atomic_load(&workListIter) == obj) {
            atomic_store(&workListIter, next);
            for(size_t i = 0; i < obj->refCount; ++i) {
                pObject_t sub = atomic_load(obj->start + i);
                if(sub) c4_impl_pushWorkList(sub);
            }
        }
    }
}

void mark() {
    printf("tag\n");
    prs_t rs = NULL, next = NULL;
    while(rs = atomic_load(&gcRSIter)) {
        atomic_store(&rs->lock, 1);
        
        next = atomic_load(&rs->next);
        
        if(atomic_load(&gcRSIter) == rs) {
            atomic_store(&gcRSIter, next);
            for(size_t i = 0; i < rs->size; ++i) {
                pObject_t obj = atomic_load(rs->start + i);
                if(obj) c4_impl_pushWorkList(obj);
            }
        } 
        atomic_store(&rs->lock, 0);
    }

    while(atomic_load(&workListIter)) markWorkList();
    printf("marking finished!\n");
    atomic_fetch_add(&completedCount, 1);
}

static void copy() {
    pObject_t obj = NULL, next = NULL;
    while(obj = atomic_load(&workListIter)) {
        next = obj->next;
        if(atomic_load(&workListIter) != obj) continue;
        atomic_store(&workListIter, next);
        if(!atomic_fetch_and(&obj->status, 0)) continue;
        
        pObject_t new = c4_impl_sbrk(obj->size, obj->refCount, 0);
        if(!new) exit(1);
        memcpy(new->start, obj->start, obj->size);
        atomic_fetch_add(&copied, obj->size + sizeof(Object_t));
        atomic_store(&obj->newAddr, new);
    }
    
    atomic_fetch_add(&completedCount, 1);
}

static void remap() {
    prs_t rs = NULL, next = NULL;
    while(rs = atomic_load(&gcRSIter)) {
        atomic_store(&rs->lock, 1);

        next = atomic_load(&rs->next);
        if(atomic_load(&gcRSIter) == rs) {
            atomic_store(&gcRSIter, next);
            for(size_t i = 0; i < rs->size; ++i) {
                pObject_t obj = atomic_load(rs->start + i);
                if(obj) {
                    if(toSpaceID == obj->spaceID) continue;
                    if(!obj->newAddr) i = *(int*)NULL;
                    if(atomic_load(rs->start + i) != obj) { --i; continue; }
                    atomic_store(rs->start + i, obj->newAddr);
                    pushRemapList(obj);
                }
            }
        }
        atomic_store(&rs->lock, 0);
    }

    pObject_t obj = NULL, _next = NULL;
    while(obj = atomic_load(&remapListIter)) {
        _next = obj->next;
        if(atomic_load(&remapListIter) == obj) {
            atomic_store(&remapListIter, _next);

            pObject_t new = obj->newAddr;
            for(size_t i = 0; i < new->refCount; ++i) {
                pObject_t sub = atomic_load(new->start + i);
                if(sub) {
                    if(!sub->newAddr) continue;
                    atomic_store(new->start + i, sub->newAddr);
                }
            }
        }
    }
}

void c4_impl_gcThrdFunc() {
    while(1) {
        mark();
        while(!atomic_load(&gcCopyFlag));
        copy();
        while(!atomic_load(&remapFlag));
        remap();
    }
}