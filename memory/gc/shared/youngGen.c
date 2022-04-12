#include "../../stdafx.h"

// static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
void* youngGen = NULL;
size_t youngGenSize = 0;

int survivorRatio = 8;

struct {
    size_t size;
    void* start;
    void* peak;
} eden = { 0 }, survivor[2] = { { 0 } }, 
  *toArea = survivor, *fromArea = survivor + 1;

//