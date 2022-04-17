#ifndef GC_GENERATIONAL_
#define GC_GENERATIONAL_

#include "../stdafx.h"

#ifdef __cplusplus
namespace gc {
extern "C" {
#endif

void genInit(int, int, void*, size_t);

void setNewRatio(int);
void setSurvivorRatio(int);
int getNewRatio();
int getSurvivorRatio();
void recalculate();

size_t getEdenSize();
size_t getEdenUsed();
size_t getSurvivorSize();
size_t getSurvivorUsed();
size_t getOldGenSize();
size_t getOldGenUsed();

void* swapSurvivor();

void* ysbrk(size_t);
void* osbrk(size_t);
void* ssbrk(size_t);

#ifdef __cplusplus
}}
#endif

#endif