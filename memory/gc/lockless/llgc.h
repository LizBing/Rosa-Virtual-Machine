#ifndef DYNA_LOCKLESS_GC_
#define DYNA_LOCKLESS_GC_

typedef struct RootSet_t rs_t, *rshdl_t;

#ifdef __cplusplus
extern "C" {
#endif

rshdl_t llgc_getRootSet(int thrdId);
rshdl_t llgc_pushRootSet(rshdl_t prv, void* start, size_t refCount);
void llgc_popNextRootSet(rshdl_t node);

void* llgc_malloc(size_t size, size_t refCount);
size_t llgc_allocated();
size_t llgc_collected();


#ifdef __cplusplus
}
#endif

#endif