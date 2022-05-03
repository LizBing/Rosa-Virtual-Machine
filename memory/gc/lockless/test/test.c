#include "../llgc.h"
#include "../../../../process/thread.h"

int main() {
    thrdPoolInit(1);
    llgc_init(1, 1 << 24, malloc(1 << 24));

    atomic_ptrdiff_t ptr = 0;
    rshdl_t rs = llgc_getRootSet(0);
    llgc_pushRootSet(rs, &ptr, 1);
    
    for(;;) {
        void* temp = llgc_malloc(128, 0);
        if(!temp) return 1;
        atomic_store(&ptr, temp);
        
    }
    
/*
    for(int i = 0; i < 1 << 23; i += 168) {
        atomic_store(&ptr, llgc_malloc(128, 0));
    }
*/
    /*
    atomic_store(&ptr, llgc_malloc(128, 10));
    for(int j = 0; j < 100; j++) {
        printf("round %d:\n", j);

        for(int i = 0; i < 10; i++) {
            printf("%p\n", llgc_load(&ptr, sizeof(void*) * i));
            llgc_store(&ptr, sizeof(void*) * i, llgc_malloc(128, 0));
        }
    }
    */
    
    while(!llgc_timeCount());
    printf("%zu, %zu\n", llgc_allocated(), llgc_collected());
    

    return 0;
}