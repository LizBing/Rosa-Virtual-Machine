#include "thread.h"
#include <deque>
#include <shared_mutex>
#include <thread>
#include <mutex>
#include <condition_variable>

struct Task_t {
    void(*func)(void*);
    void* arg;
};

size_t maxThrdCount;
std::atomic<size_t> taskCount;
std::atomic<size_t> completedTaskCount;
std::atomic<size_t> activeCount;

static std::mutex mtx;
static std::deque<Task_t> taskQueue;

static void manager() {
    while(1) {
        mtx.lock();
        Task_t task = { 0 };
        if(taskQueue.size()) {
            task = *taskQueue.begin();
            taskQueue.pop_front();
        }
        mtx.unlock();
        if(task.func) {
            activeCount.store(activeCount.load() + 1);
            task.func(task.arg);
            activeCount.store(activeCount.load() - 1);
            taskCount.store(taskCount.load() - 1);
            completedTaskCount.store(completedTaskCount.load() + 1);
        }
    }
}

namespace thrd {
extern "C" {

void thrdPoolInit(size_t size) {
    maxThrdCount = size;
    taskCount.store(0);
    completedTaskCount.store(0);
    activeCount.store(0);

    for(int i = 0; i < size; ++i) {
        std::thread(manager).detach();
    }
}

size_t getTaskCount() {
    return taskCount.load();
}

size_t getCompletedTaskCount() {
    return completedTaskCount.load();
}

size_t getMaxThrdCount() {
    return maxThrdCount;
}

size_t getActiveCount() {
    return activeCount.load();
}

void addTask(void(*task)(void*), void* arg) {
    std::lock_guard<std::mutex> l(mtx);
    taskQueue.push_back((Task_t){ task, arg });
}

}}