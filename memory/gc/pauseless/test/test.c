#include "../pauseless.h"
#include <stdio.h>
#include <unistd.h>

void* loadPtr(ref_t ref);
void* ptr2ref(ref_t value);
int isExpected(ref_t ref);

void test01() {
    for(int i = 0; i < 10086000; ++i) {
        ref_t ref = pl_new(128, 0);
        printf("%p, %d\n", loadPtr(ref), isExpected(ref));
    }
}

void test1p5() {
    pRootSet_t b = pl_getRootSet(0);
    atomic_ref_t ref = NULL;
    pl_newRootSet(malloc(PL_GC_SIZEOF_ROOTSET_T), b, &ref, 1);
    atomic_store(&ref, pl_new(128, 0));

    for(int i = 0; i < 1000000000; ++i) {
        printf("%p, %d\n", loadPtr(atomic_load(&ref)), isExpected(atomic_load(&ref)));
    }

    pl_deleteRootSet(b, free);
}

void test1p7() {
    pRootSet_t b = pl_getRootSet(0);
    atomic_ref_t ptr = NULL;
    pl_newRootSet(malloc(PL_GC_SIZEOF_ROOTSET_T), b, &ptr, 1);
    atomic_store(&ptr, pl_new(128, 0));
    // int i = *(int*)NULL;
    pl_store(&ptr, 0, 10);

    for(int i = 0; i < 1000000; ++i) {
        printf("%p, %d\n", pl_load(&ptr, 0), i);
    }

    pl_deleteRootSet(b, free);
}

void test1p8() {
    pRootSet_t b = pl_getRootSet(0);
    atomic_ref_t arr[10] = { NULL };
    pl_newRootSet(malloc(PL_GC_SIZEOF_ROOTSET_T), b, arr, 10);

    for(int i = 0; i < 10; ++i) {
        atomic_store(arr + i, pl_new(128, 10));
    }

    for(int i = 0; i < 10086000; ++i) {
        for(int j = 0; j < 10; ++j) {
            printf("%p\n", atomic_load(arr + j));
        }
        putchar('\n');
    }

    pl_deleteRootSet(b, free);
}

void test1p9() {
    pRootSet_t b = pl_getRootSet(0);
    atomic_ref_t ref = null;
    pl_newRootSet(malloc(PL_GC_SIZEOF_ROOTSET_T), b, &ref, 1);

    atomic_store(&ref, pl_new(80, 0));
    for(int i = 0; i < 10; ++i) {
        // pl_store(&ref, 8 * i, i);
        atomic_ref_t temp = pl_new(16, 0);
        pl_storeObjRef(&ref, i, &temp);
    }

    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            printf("%p\n", *pl_loadObjRef(&ref, j));
        }
        putchar('\n');
    }

    pl_deleteRootSet(b, free);
}

void test02() {
    pRootSet_t b = pl_getRootSet(0);
    atomic_ref_t ref = null;
    pl_newRootSet(malloc(PL_GC_SIZEOF_ROOTSET_T), b, &ref, 1);

    atomic_store(&ref, pl_new(80, 10));
    for(int i = 0; i < 10; ++i) {
        atomic_ref_t temp = pl_new(128, 0);
        pl_storeObjRef(&ref, i, &temp);
    }

    for(int i = 0; i < 1000000; ++i) {
        for(int j = 0; j < 10; ++j) {
            printf("%p\n", pl_load(&ref, 8 * j));
        }
        putchar('\n');
    }
}

void test2p1() {
    pRootSet_t b = pl_getRootSet(0);
    atomic_ref_t arr[10] = { NULL };
    pl_newRootSet(malloc(PL_GC_SIZEOF_ROOTSET_T), b, arr, 10);

    for(int i = 0; i < 1; ++i) {
        for(int j = 0; j < 10; ++j) {
            atomic_store(arr + j, pl_new(128, 10));
            for(int k = 0; k < 10; ++k) {
                atomic_ref_t temp = pl_new(128, 0);
                pl_storeObjRef(arr + j, k, &temp);
            }
        }
    }

    pl_deleteRootSet(b, free);
}

int main(void) {
    if(pl_init(1, 2, malloc(1 << 24), 1 << 24)) return 2;

    // test01();

    // test1p5();
    // test1p7();
    // test1p8();
    // test1p9();
    // test02();
    test2p1();

    usleep(500);
    printf("allocated: %zu, collected: %zu \nexecuted routine: %d\n", pl_allocated(), pl_collected(), pl_routineCount());

    return 0;
}