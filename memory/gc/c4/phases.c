#include "impl.h"

atomic_bool gcCopyFlag = 0;
static atomic_bool remapFlag = 0;

static _Atomic(size_t) completedCount = 0;

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
        while(!atomic_compare_exchange_weak(&completedCount, &gcThrdCount, 0));
        size_t delta = c4_impl_exchangeSpace();
        atomic_fetch_add(&routine, 1);
        workListIter = workListBegin;

        atomic_store(&gcCopyFlag, 1);
        while(!atomic_compare_exchange_weak(&completedCount, &gcThrdCount, 0));
        atomic_store(&gcCopyFlag, 0);
        atomic_fetch_add(&gcCollected, delta - copied);
        copied = 0;

        workListEnd = remapListEnd = NULL;
        atomic_store(&remapFlag, 1);
        while(!atomic_compare_exchange_weak(&completedCount, &gcThrdCount, 0));
        atomic_store(&remapFlag, 0);
    }
}

void c4_impl_pushWorkList(pObject_t obj) {
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
            if(!obj->newAddr) return obj;
            new = obj->newAddr;
            tag = 1;
        } else goto lable;
    } while(!atomic_compare_exchange_weak(addr, &obj, new));
    if(tag) pushRemapList(new);

    return new;
}

static void markWorkList() {
    pObject_t obj = NULL, next = NULL;
    while(obj = atomic_load(&workListIter)) {
        next = atomic_load(&obj->next);
        if(atomic_compare_exchange_weak(&workListIter, &obj, next))
        for(size_t i = 0; i < obj->refCount; ++i) {
            pObject_t sub = atomic_load(obj->start + i);
            if(sub) c4_impl_pushWorkList(sub);
        }
    }
}

static _Atomic(void*) rsIter = NULL;
void mark() {
    prs_t rs = NULL, next = NULL;
    while(rs = atomic_load(&rsIter)) {
        atomic_store(&rs->lock, 1);
        next = atomic_load(&rs->next);
        if(atomic_compare_exchange_weak(&rsIter, &rs, next))
        for(size_t i = 0; i < rs->size; ++i) {
            pObject_t obj = atomic_load(rs->start + i);
            if(obj) c4_impl_pushWorkList(obj);
        }
        atomic_store(&rs->lock, 0);
    }

    markWorkList();
    atomic_fetch_add(&completedCount, 1);
}

static void copy() {
    pObject_t obj = NULL, next = NULL;
    while(obj = atomic_load(&workListIter)) {
        next = obj->next;
        if(!atomic_compare_exchange_weak(&workListIter, &obj, next)) continue;
        if(!atomic_fetch_and(&obj->status, 0)) continue;
        
        pObject_t new = c4_impl_sbrk(obj->size, obj->refCount, 0);
        if(!new) exit(1);
        memcpy(new->start, obj->start, obj->size);
        atomic_fetch_add(&copied, new->size + sizeof(Object_t));
        atomic_store(&obj->newAddr, new);
    }

    atomic_fetch_add(&completedCount, 1);
}

static void remap() {
    prs_t rs = NULL, next = NULL;
    while(rs = atomic_load(&rsIter)) {
        atomic_store(&rs->lock, 1);
        next = atomic_load(&rs->next);
        if(atomic_compare_exchange_weak(&rsIter, &rs, next))
        for(size_t i = 0; i < rs->size; ++i) {
            pObject_t obj = atomic_load(rs->start + i);
            if(obj) {
                if(!obj->newAddr) continue;
                if(!atomic_compare_exchange_weak(rs->start + i, &obj, obj->newAddr)) { --i; continue; }
                pushRemapList(obj);
            }
        }
        atomic_store(&rs->lock, 0);
    }
    atomic_fetch_add(&completedCount, 1);

    pObject_t obj = NULL, _next = NULL;
    while(obj = atomic_load(&remapListIter)) {
        _next = obj->next;
        if(atomic_compare_exchange_weak(&remapListIter, &obj, _next))
        for(size_t i = 0; i < obj->refCount; ++i) {
            pObject_t sub = atomic_load(obj->start + i);
            if(sub) {
                if(!sub->newAddr) continue;
                atomic_store(obj->start + i, sub->newAddr);
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