#include "heap.h"

extern size_t restMemory;

int main() {
    Heap_t* heap = newHeap(117);
    printf("%p\n", heap);
    printf("%zu\n", restMemory);

    int* ptr = firstFit(heap, 17);
    printf("restMemory: %zu\n", restMemory);
    void* ptr0 = firstFit(heap, 19);
    printf("restMemory: %zu\n", restMemory);
    void* ptr1 = firstFit(heap, 13);
    printf("restMemory: %zu\n", restMemory);

    release(ptr0);
    printf("restMemory: %zu\n", restMemory);
    release(ptr);
    printf("restMemory: %zu\n", restMemory);
    release(ptr1);
    printf("restMemory: %zu\n", restMemory);

    deleteHeap(heap);
    return 0;
}