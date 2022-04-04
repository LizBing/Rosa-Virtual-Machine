#include "heap.h"
#include "../../exception/defs.h"

#define FREETAG 0b10100000
#define USEDTAG 0b01010000
#define FUMASK  0b11110000

#define TAGSIZE sizeof(byte_t)
#define SSIZE sizeof(size_t)

#define FLINFOSIZE (HLFINFOSIZE * 2)
#define HLFINFOSIZE (TAGSIZE + SSIZE)
#define MINIMUM_BLOCKSIZE (sizeof(void*) * 2)

#define lTagRef(mb) (*(byte_t*)(mb))
#define rTagRef(mb) (*(byte_t*)((mb) + lSizeRef(mb) + HLFINFOSIZE + SSIZE))
#define lSizeRef(mb) (*(size_t*)((mb) + TAGSIZE))
#define rSizeRef(mb) (*(size_t*)((mb) + HLFINFOSIZE + lSizeRef(mb)))
#define prevRef(mb) (*(void**)((mb) + HLFINFOSIZE))
#define nextRef(mb) (*(void**)((mb) + TAGSIZE + lSizeRef(mb)))
#define setTag(mb, t) (lTagRef(mb) = rTagRef(mb) = (t))
#define setSize(mb, s) (rSizeRef(mb) = lSizeRef(mb) = (s))
#define newBlock(mb, s) setSize(mb, s); setTag(mb, FREETAG)

void* heap = NULL;
size_t heapSize = 0;
size_t restMemory = 0;

void* gen[3] = { NULL };
size_t genSize[3] = { 0 };

void* freeList[3] = { NULL };
void* flEnd[3] = { NULL };
pthread_mutex_t flMtx[3] = { NULL };
void* toSurv = NULL;

void memInit(size_t edenSize, size_t survSize, size_t oldSize) {
    restMemory = heapSize = edenSize + survSize * 2 + oldSize;
    heap = malloc(heapSize);
    if(!heap) { throwr(out_of_memory, "metaspace"); }

    genSize[EDEN] = edenSize - FLINFOSIZE;
    genSize[SURV] = survSize - FLINFOSIZE;
    genSize[OLDGEN] = oldSize - FLINFOSIZE;

    size_t setCount = 0;
    for(int i = 0; i < 3; setCount += genSize[i++] + FLINFOSIZE) {
        flMtx[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

        gen[i] = heap + setCount;
        newBlock(gen[i], genSize[i]);
        prevRef(gen[i]) = nextRef(gen[i]) = NULL;

        freeList[i] = flEnd[i] = gen[i];
    } toSurv = heap + setCount;
}

void* treat(void* mb, size_t need, int genNo) {
    void* newFMB = (byte_t*)mb + need;

    if(lSizeRef(mb) < need + MINIMUM_BLOCKSIZE) goto ret;
           
    newBlock(newFMB, lSizeRef(mb) - need);
    prevRef(newFMB) = prevRef(mb);

    newBlock(mb, need - FLINFOSIZE);

    if(mb == freeList[genNo]) freeList[genNo] = newFMB;
    if(mb == flEnd[genNo]) flEnd[genNo] = newFMB;

ret:
    setTag(mb, USEDTAG);
    restMemory -= lSizeRef(mb) + FLINFOSIZE;
    return mb; 
}

void* firstFit(size_t req, int genNo) {
    if(req < MINIMUM_BLOCKSIZE) req = MINIMUM_BLOCKSIZE;
    size_t need = req + FLINFOSIZE;

    pthread_mutex_lock(flMtx + genNo);

    void* iter = freeList[genNo];
    for(; iter; iter = nextRef(iter)) 
        if(lSizeRef(iter) >= req) break;
    
    void* ret_val = NULL;
    if(iter) ret_val = treat(iter, need, genNo) + HLFINFOSIZE;

    pthread_mutex_unlock(flMtx + genNo);
    return ret_val;
}

void* bestFit(size_t req, int genNo) {
    return firstFit(req, genNo);
}

void* worstFit(size_t req, int genNo) {
    if(req < MINIMUM_BLOCKSIZE) req = MINIMUM_BLOCKSIZE;
    size_t need = req + FLINFOSIZE;

    pthread_mutex_lock(flMtx + genNo);

    void* iter = flEnd[genNo];

    void* ret_val = NULL;
    if(iter) {
        if(lSizeRef(iter) >= req)
            ret_val = treat(iter, need, genNo) + HLFINFOSIZE;
    } else ret_val = NULL;

    pthread_mutex_unlock(flMtx + genNo);
    return ret_val;
}

int isLegal(void* mb, int genNo) {
    if(mb + FLINFOSIZE + lSizeRef(mb) > gen[genNo] + genSize[genNo]) return 0;
    if(lSizeRef(mb) != rSizeRef(mb)) return 0;
    if(lTagRef(mb) != rTagRef(mb)) return 0;
    if(USEDTAG != lTagRef(mb) & FUMASK && FREETAG != lTagRef(mb) & FUMASK) return 0;

    return 1;
}

void* getHigherOne(void* mb, int genNo) {
    void* higher = &rTagRef(mb) + sizeof(byte_t);
    if(higher + HLFINFOSIZE > gen[genNo] + genSize[genNo]) return NULL;
    if(!isLegal(higher, genNo)) return NULL;

    return higher;
}

void* getLowerOne(void* mb, int genNo) {
    size_t* psize = (size_t*)(mb - HLFINFOSIZE);
    if(psize < gen[genNo]) return NULL;

    void* lower = (byte_t*)psize - *psize - HLFINFOSIZE;
    if(lower < gen[genNo]) return NULL;

    if(!isLegal(lower, genNo)) return NULL;

    return lower;
}

void* getHigherFMB(void* mb, int genNo) {
    for(void* iter = getHigherOne(mb, genNo); iter; iter = getHigherOne(iter, genNo)) 
        if(FREETAG == lTagRef(iter)) return iter;
    return NULL;
}

void* getLowerFMB(void* mb, int genNo) {
    for(void* iter = getLowerOne(mb, genNo); iter; iter = getLowerOne(iter, genNo)) 
        if(FREETAG == lTagRef(iter)) return iter;
    return NULL;
}

void* merge(void* mb, int genNo) {
    void* lower = getLowerOne(mb, genNo);
    void* higher = getHigherOne(mb, genNo);

    if(higher) { 
        if((byte_t)FREETAG == lTagRef(higher)) {
lable:
            nextRef(higher) = nextRef(mb);
            setSize(mb, lSizeRef(mb) + lSizeRef(higher) + FLINFOSIZE);

            if(freeList[genNo] == higher) freeList[genNo] = mb;
            if(flEnd[genNo] == higher) flEnd[genNo] = mb;
            restMemory += FLINFOSIZE;
        }
    }

    if(lower) {
        higher = mb;
        mb = lower;
        lower = NULL;
        goto lable;
    }
   
    return mb;
}

void gcfree(void* p) {
    void* mb = p - HLFINFOSIZE;
    int no = 0;

    for(; no <= OLDGEN + 1; no++) {
        if(no > OLDGEN) return 0;
        if(gen[no] <= mb && mb <= gen[no] + genSize[no]) break;
    }
            
    if(!isLegal(mb, no) || USEDTAG != lTagRef(mb)) { throwr(illegal_memory_address, "%p", p); }
    pthread_mutex_lock(flMtx + no);

    setTag(mb, FREETAG);
    mb = merge(mb, no);

    void* hfb = getHigherFMB(mb, no);
    void* lfb = getLowerFMB(mb, no);

    if(hfb) {
        prevRef(mb) = prevRef(hfb);
        prevRef(hfb) = mb;
        nextRef(mb) = hfb;
        if(freeList[no] == hfb) freeList[no] = mb;
    } else nextRef(mb) = NULL;
    
    if(lfb) {
        nextRef(mb) = nextRef(lfb);
        nextRef(lfb) = mb;
        prevRef(mb) = lfb;
        if(flEnd[no] == lfb) flEnd[no] = mb;
    } else prevRef(mb) = NULL;

    restMemory += lSizeRef(mb);
            
    pthread_mutex_unlock(flMtx + no);
}

void* vmalloc(size_t size) {
    void* ret_val = NULL;
    if(size >= largeSize) ret_val = worstFit(size, EDEN);
    else if(size >= mediumSize) ret_val = bestFit(size, EDEN);
    else ret_val = firstFit(size, EDEN);

    if(!ret_val) throw(out_of_memory, "%zu", size);

    return ret_val;
}

void* gcalloc(size_t size) {
    void* ret_val = NULL;
    ret_val = vmalloc(size);
    extern void(*gc)();
    if(catche(out_of_memory, gc, NULL)) return gcalloc(size);

    return ret_val;
}