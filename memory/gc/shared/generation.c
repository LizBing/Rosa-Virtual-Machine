#include "generation.h"

int newRatio = 0;
int survivorRatio = 0;

// young generation
void* eden = NULL;
size_t edenSize = 0;
size_t edenUsed = 0;

void* survivor[2] = { NULL };
size_t survivorSize = 0;
size_t survivorUsed = 0;
void* toSpace = NULL, *fromSpace = NULL;

void* oldGen = NULL;
size_t oldGenSize = NULL;

void generationInit(int NewRatio, int SurvivorRatio, void* start, size_t size) {
    newRatio = NewRatio;
    survivorRatio = SurvivorRatio;

    size_t youngGenSize = size / NewRatio;
}

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

int gcState();