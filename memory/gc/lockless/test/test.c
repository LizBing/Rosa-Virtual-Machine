#include "../llgc.h"
#include "../../../../process/thread.h"

int main() {
    thrdPoolInit(1);
    llgc_init(10, 102400, malloc(102400));

    atomic_ptrdiff_t ptr = 0;
    rshdl_t rs = llgc_getRootSet(0);
    llgc_pushRootSet(rs, &ptr, 1);
    /*
    for(int i = 0; i < 102400; i++) {
        atomic_store(&ptr, llgc_malloc(128, 0));
    }
    */
    atomic_store(&ptr, llgc_malloc(128, 10));
    for(int j = 0; j < 10; j++) {
        printf("round %d:\n", j);

        for(int i = 0; i < 10; i++) {
            printf("%p\n", llgc_load(&ptr, sizeof(void*) * i));
            llgc_store(&ptr, sizeof(void*) * i, llgc_malloc(128, 0));
        }
    }
    

    printf("%zu, %zu\n", llgc_allocated(), llgc_collected());

    return 0;
}