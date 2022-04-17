#include <stdio.h>
#include "generational.h"

void test01() {
    genInit(2, 8, malloc(1024 * 1024), 1024 * 1024);
    printf("%zu, %zu, %zu\n", getEdenSize(), getSurvivorSize(), getOldGenSize());
}

int main() {
    test01();

    int* i = ysbrk(4);
    printf("%zu\n", getEdenUsed());

    return 0;
}