#ifndef YOUNG_GENERATION_
#define YOUNG_GENERATION_

#include "../../stdafx.h"

void generationInit(int, int, void*, size_t);

size_t getEdenSize();
size_t getSurvivorSize();
size_t getEdenGenUsed();
size_t getSurvivorUsed();

size_t getOldGenSize();
size_t getOldGenUsed();

int getSurvivorRatio();
int getNewRatio();

size_t getMinorGCTimes();
clock_t getMinorGCCost();

size_t getMajorGCTimes();
clock_t getMajorGCCost();

#endif