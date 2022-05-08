#include "../c4.h"
#include <unistd.h>

void test01() {
    if(c4_init(1, 10, malloc(1 << 24), 1 << 24)) exit(2);

    for(int i = 0; i < 10000; i++) {
        printf("%p\n", c4_malloc(128, 0));
    }
}

void test02() {
    if(c4_init(1, 2, malloc(1 << 24), 1 << 24)) exit(2);

    prs_t b = c4_getRootSet(0);
    _Atomic(void*) arr[10] = { NULL };
    prs_t rs = c4_pushRootSet(malloc(C4_ROOTSET_SIZE), b, arr, 10);
    
    for(int i = 0; i < 10000; i++) {
        for(int j = 0; j < 10; j++) {
            _Atomic(void*)* ptr = arr + j;
            atomic_store(ptr, c4_malloc(128, 10));
            for(int k = 0; k < 10; k++) {
                // printf("I love you!\n");
                atomic_store(c4_loadRefAddr(ptr, k), c4_malloc(128, 0));
                // c4_malloc(128, 0);
            }
        }
    }

    c4_popNextRootSet(b, free);
}

int main(void) {
    // test01();
    test02();

    usleep(500);
    printf("\nallocated: %zu, collected: %zu\ncompleted routine: %zu\n", c4_allocated(), c4_collected(), c4_gcRoutine());

    return 0;
}