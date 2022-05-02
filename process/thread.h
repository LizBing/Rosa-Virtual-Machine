#ifndef DYNA_THRDPOOL_
#define DYNA_THRDPOOL_

#include "stdafx.h"

#ifdef __cplusplus
namespace thrd {
extern "C" {
#endif

void thrdPoolInit(size_t);

size_t getTaskCount();
size_t getCompletedTaskCount();
size_t getMaxThrdCount();
size_t getActiveCount();

void addTask(void(*)(int, void*), void*);

#ifdef __cplusplus
}}
#endif

#endif