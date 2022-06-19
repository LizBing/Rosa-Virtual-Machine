#include <stdio.h>
#include "include/zero.h"
#include <pthread.h>

#define APPTHRD 3
#define GCTHRD 3
#define HEAPSIZE (1 << 24)

atomic_int finished = 0;

void thrd0() {
    pRootSet_t b = zero_getBasicRootSet(0);
    zero_root_t arr1[11] = { zero_null };
    pRootSet_t rs = zero_newRootSet(alloca(ZERO_SIZEOF_ROOTSET_T), b, arr1, 11);

    for(int i = 0; i < 10000; ++i) {
        printf("thrd0: %d\n", i);
        for(int j = 0; j < 10; ++j) {
            zero_new(arr1 + j, 128, 10);

            for(int k = 0; k < 10; ++k) {
                zero_new(arr1 + 10, 80, 0);
                zero_storeRefToObj(arr1 + j, k, arr1 + 10);
            }
        }
    }

    zero_root_t arr2[10] = { zero_null };
    void* placement = malloc(ZERO_SIZEOF_ROOTSET_T);
    if(!placement) exit(10);
    zero_newRootSet(malloc(ZERO_SIZEOF_ROOTSET_T), rs, arr2, 10);

    for(int i = 0; i < 10; ++i) {
        zero_new(arr2 + i, 80, 0);

        for(int j = 0; j < 10; ++j) {
            zero_store(arr2 + i, sizeof(uint64_t) * j, j);
        }

    }

    printf("\nprint thrd0 arr2");
    for(int i = 0; i < 10; ++i) {
        printf("arr2[%d]: ", i);
        for(int j = 0; j < 10; ++j)
            printf("\t[%d]: %d\n", j, zero_load(arr2 + i, sizeof(uint64_t) * j));
        putchar('\n');
    }

    zero_deleteRootSet(rs, free);

    printf("\nprint thrd0 arr1");
    for(int i = 0; i < 10; ++i) {
        printf("arr1[%d]: %p", i, ((zero_ref_t*)(arr1 + i))->addr);
        for(int j = 0; j < 10; ++j) {
            zero_ref_t ref = zero_loadRef(arr1 + i, j);
            printf("\t[%d]: %p\n", j, ref.addr);
        }
        putchar('\n');
    }

    zero_deleteRootSet(b, NULL);

    ++finished;
}

void thrd1() {
    pRootSet_t b = zero_getBasicRootSet(1);
    zero_root_t foo = zero_null;
    zero_newRootSet(alloca(ZERO_SIZEOF_ROOTSET_T), b, &foo, 1);
    while(!finished) {
        zero_new(&foo, 128, 0);
    }

    zero_deleteRootSet(b, NULL);
    ++finished;
}

void thrd2() {
    zero_root_t arr[10] = { zero_null };
    pRootSet_t b = zero_getBasicRootSet(2);
    void* placement = malloc(ZERO_SIZEOF_ROOTSET_T);
    if(!placement) exit(10);
    zero_newRootSet(placement, b, arr, 10);

    for(int i = 0; i < 10; ++i) {
        zero_new(arr + i, 80, 0);
        zero_store(arr + i, 16, 114514);
    }

    while(!finished) {
        for(int i = 0; i < 10; ++i) {
            if(zero_load(arr + i, 16) != 114514)
                exit(11);
        }
    }

    zero_deleteRootSet(b, free);

    ++finished;
}

void test01() {
    pthread_t t0 = { 0 }, t1 = { 0 }, t2 = { 0 };
    pthread_create(&t0, NULL, thrd0, NULL);
    pthread_create(&t1, NULL, thrd1, NULL);
    pthread_create(&t2, NULL, thrd2, NULL);

    printf("start test01\n\n");

    pthread_detach(t0);
    pthread_detach(t1);
    pthread_detach(t2);

    while(2 != finished);
}

int main() {
    char* gcname = zero_stdName();
    if(!gcname) exit(1);

    void* heap = malloc(HEAPSIZE);
    if(!heap) exit(1);

    int err = zero_init(3, 3, heap, HEAPSIZE);
    if(err) exit(err);
    printf(
            "Collector: %s\n"
            "Heap Size: %zu\n"
            "Application thread count: %d\n"
            "GC thread count: %d\n"
            "\n"
            , gcname, HEAPSIZE, APPTHRD, GCTHRD);

    clock_t c = clock();
    test01();
    c = clock() - c;

    printf(
            "\nFinished testing in %zu ns!\n"
            "completed gc routine: %d\n"
            "allocated memory: %zu bytes\n"
            "collected memory: %zu bytes\n"
            "\n",
            c, zero_routine(), zero_allocated(), zero_collected()
    );


    return 0;
}
