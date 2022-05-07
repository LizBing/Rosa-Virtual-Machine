#include "../llgc.h"
#include "../../../../process/thread.h"

void test01() {
    thrdPoolInit(1);
    llgc_init(1, 1 << 24, malloc(1 << 24));
    rshdl_t rs = llgc_getRootSet(0);

    _Atomic(void*) arr[10] = { NULL };
    llgc_pushRootSet(rs, arr, 10);
    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            void* temp = llgc_malloc(128, 10);
            if(!temp) exit(1);
            atomic_store(arr + j, temp);
            for(int k = 0; k < 10; ++k) {
                temp = llgc_malloc(128, 0);
                if(!temp) exit(1);
                printf("arr[%d] = %p\n", j, atomic_load(arr + j));
                if(!atomic_load(arr + j)) printf("s\n");
                llgc_storeRef(arr + j, k, temp);
            }
        }
    }

    llgc_popNextRootSet(rs);
}

void test02() {
    thrdPoolInit(1);
    llgc_init(1, 1 << 24, malloc(1 << 24));
    rshdl_t rs = llgc_getRootSet(0);

    _Atomic(void*) arr[10] = { NULL };
    llgc_pushRootSet(rs, arr, 10);

    for(int i = 0; i < 10; ++i) {
        atomic_store(arr + i, llgc_malloc(128, 0));
    }

    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 10; ++j) {
            printf("arr[%d] = %p\n", j, atomic_load(arr + j));
        }
    }

    llgc_popNextRootSet(rs);
}

int main() {
    // test01();
    test02();
    
    printf("%zu, %zu\n", llgc_allocated(), llgc_collected());

    return 0;
}