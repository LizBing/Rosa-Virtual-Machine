#include "../../../../process/thread.h"
#include "../c4.h"

void test01() {
    thrdPoolInit(1);
    if(c4_init(2, malloc(1 << 24), 1 << 24)) exit(1);
    prs_t rs = c4_getRootSet(0);

    _Atomic(void*) ptr = NULL;
    c4_pushRootSet(malloc(C4_ROOTSET_SIZE), rs, &ptr, 1);

    atomic_store(&ptr, c4_malloc(128, 0));
    while(1) {
        printf("%p\n", c4_loadRefAddr(&ptr, 0));
    }
}

int main(void) {
    test01();

    return 0;
}