#include "../llgc.h"
#include "../../../../process/thread.h"



int main() {
    thrdPoolInit(1);
    llgc_init(2, 1 << 24, malloc(1 << 24));
    
    printf("%zu, %zu\n", llgc_allocated(), llgc_collected());
    

    return 0;
}