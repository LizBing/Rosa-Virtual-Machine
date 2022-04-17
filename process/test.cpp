#include "thread.h"
#include <unistd.h>

std::atomic_int i = 0;
void foo(void*) {
    auto id = pthread_self();
    std::cout << "Thread " << id << ": 启动" << std::endl;
    for(int j = 0; j < 10; j++) {
        std::cout << "Thread " << pthread_self() << ": 为变量i贡献了 1 " << std::endl;
        i.store(i.load() + 1);
    }
    std::cout << "Thread " << id << ": 完成任务" << std::endl;
}

int main() {
    thrd::thrdPoolInit(100);
    thrd::addTask(foo, NULL);
    thrd::addTask(foo, NULL);
    thrd::addTask(foo, NULL);

    return 0;
}