#ifndef PAUSELESS_GC_ALLOCATOR_
#define PAUSELESS_GC_ALLOCATOR_

#include "stdafx.h"
#include "object.h"

Obj_t* plgc_sbrk(size_t, size_t);
void exchange();

#endif