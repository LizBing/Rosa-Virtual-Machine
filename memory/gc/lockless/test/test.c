#include "../llgc.h"
#include "../../../../process/thread.h"

int main() {
    thrdPoolInit(10);
    llgc_init(2, 102400, malloc(102400));

    clock_t c1 = clock();
    while(clock() < c1 + 20000);

    return 0;
}