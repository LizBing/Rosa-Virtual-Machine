#include "generational.h"
#include <stdbool.h>

static void* heap = NULL;
static size_t heapSize = 0;

static struct Gen {
    void* start;
    size_t size;
    void* peak;
} 
eden = { 0 }, 
survivor = { 0 }, 
oldGen = { 0 };

static void* toSpace = NULL;

static size_t youngGenSize = 0;

static int newRatio = 0;
static int survivorRatio = 0;

static bool recalculateYoung = false;
static bool recalculateHeap = false;

void genInit(int NewRatio, int SurvivorRatio, void* start, size_t size) {
    newRatio = NewRatio;
    survivorRatio = SurvivorRatio;
    heap = start;
    heapSize = size;

    recalculateYoung = true;
    recalculateHeap = true;
    recalculate();
}

void setNewRatio(int r) {
    newRatio = r;
    recalculateHeap = true;
}

void setSurvivorRatio(int r) {
    survivorRatio = r;
    recalculateYoung = true;
}

int getNewRatio() { return newRatio; }
int getSurvivorRatio() { return survivorRatio; }

void recalculate() {
    if(recalculateHeap) {
        youngGenSize = heapSize / (newRatio + 1);
        oldGen.size = heapSize - youngGenSize;
        
        oldGen.peak = oldGen.start = heap + youngGenSize;

        recalculateHeap = false;
        recalculateYoung = true;
    }

    if(recalculateYoung) {
        eden.size = youngGenSize / (survivorRatio + 2) * survivorRatio;
        survivor.size = (youngGenSize - eden.size) / 2;

        eden.peak = eden.start = heap;

        void* temp1 = survivor.start, *temp2 = toSpace;
        survivor.start = heap + eden.size;
        toSpace = survivor.start + survivor.size;
        if(temp1 > temp2) swapSurvivor();

        recalculateYoung = false;
    }
}

size_t getEdenSize() { return eden.size; }
size_t getEdenUsed() { return (size_t)eden.peak - (size_t)eden.start; }
size_t getSurvivorSize() { return survivor.size; }
size_t getSurvivorUsed() { return (size_t)survivor.peak - (size_t)survivor.start; }
size_t getOldGenSize() { return oldGen.size; }
size_t getOldGenUsed() { return (size_t)oldGen.peak - (size_t)oldGen.start; }

static void* gsbrk(struct Gen* gen, pthread_mutex_t* pMtx, size_t size) {
    pthread_mutex_lock(pMtx);

    void* ret = gen->peak;
    if(gen->peak + size >= gen->start + gen->size) ret = NULL;
    else gen->peak = gen->peak + size;

    pthread_mutex_unlock(pMtx);
    return ret;
}

void* ysbrk(size_t size) {
    static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    return gsbrk(&eden, &mtx, size);
}

void* osbrk(size_t size) {
    static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    return gsbrk(&oldGen, &mtx, size);
}

void* ssbrk(size_t size) {
    static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    return gsbrk(&survivor, &mtx, size);
}

void* swapSurvivor() {
    void* temp = toSpace;
    toSpace = survivor.start;
    survivor.peak = survivor.start = toSpace;

    return toSpace;
}