#include "allocator.h"
#include "api.h"
#include "../../../process/thread.h"

static size_t heapSize = 0;
static void* heap = NULL;
size_t spaceSize = 0;
void* space[2] = { NULL };

static size_t thrdCount = 0;

extern RootSet_t* rootSets;
extern size_t rootSetsSize;

int gc_signal = 0;

void plgc_major();

int plgc_init(void* start, size_t size, size_t ThrdCount) {
    heap = start;
    heapSize = size;
    thrdCount = ThrdCount;

    spaceSize = size / 2;
    space[0] = heap;
    space[1] = heap + spaceSize;

    rootSetsSize = getMaxThrdCount - ThrdCount;
    rootSets = malloc(sizeof(RootSet_t) * rootSetsSize);
    if(!rootSets) {
        return 1;
    }

    for(int i = 0; i < rootSetsSize; ++i) {
        rootSets[i].begin = NULL;
        rootSets[i].next = NULL;
        rootSets[i].size = 0;
    }

    for(int i = 0; i < ThrdCount; ++i) {
        addTask(plgc_major, NULL);
    }
}

size_t plgc_thrdCount() {
    return thrdCount;
}

size_t plgc_heapSize() {
    return heapSize;
}