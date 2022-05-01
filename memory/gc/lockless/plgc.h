#ifndef DYNA_PAUSELESS_GC_
#define DYNA_PAUSELESS_GC_

typedef struct RootSet_t rs_t, *rshdl_t;

#ifdef __cplusplus
extern "C" {
#endif

void* plgc_malloc(size_t size, size_t refCount);


#ifdef __cplusplus
}
#endif

#endif