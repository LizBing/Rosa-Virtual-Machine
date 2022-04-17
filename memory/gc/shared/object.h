#ifndef GC_OBJECT_
#define GC_OBJECT_

#include "../stdafx.h"

struct GC_Object {
    uint8_t colour : 1;
    uint8_t age : 4;
    size_t refCount : 59;
    size_t size;
    GC_Object* start[0];
};

#endif