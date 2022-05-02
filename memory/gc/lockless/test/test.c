#include "../llgc.h"
#include "../../../../process/thread.h"

int main() {
    thrdPoolInit(10);
    llgc_init(10, 102400, malloc(102400));

    atomic_ptrdiff_t ptr = 0;
    rshdl_t rs = llgc_getRootSet(0);
    llgc_pushRootSet(rs, &ptr, 1);
    /*
    for(int i = 0; i < 102400; i++) {
        atomic_store(&ptr, llgc_malloc(128, 0));
    }
    */
    while(1) {
        void* mem = llgc_malloc(128, 0);
        atomic_store(&ptr, mem);
    }

    return 0;
}