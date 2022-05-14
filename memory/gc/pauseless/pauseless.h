#ifndef DYNA_GC_PAUSELESS_
#define DYNA_GC_PAUSELESS_

/* ================================================ *
 * Designed and implemented by Lizbing.             *
 * A pause-less garbage collector                   *
 * Copyright (c) 2022 LizBing. All rights reserved. *
 * Licensed under the BSD 3                         *
 *                                                  *
 * Version: Beta 0.0.1                              *
 * Generated date: 14/5/2022                        *
 * ================================================ */

/*
 * Introduction:
 * ...
 * 
 * Abstract:
 * ...
 * I consider that every reference in an object should be gather at the start of the object, so that the gc phases can find the location of reachable references easily.
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <time.h>
#include <stdatomic.h>

typedef char byte_t;
typedef uint64_t ref_t, ptr_t;

/*
 * Data type 'atomic_ref_t' is used to declare variables which are to store an object reference.
 * Every load or store operation in root sets should be finished by using 'atomic_load' or 'atomic_store' in order to ensure the data synchronization.
 * Example 1:
 *      (there's an 'atomic_ref_t' typed variable 'ref' which has already been in a root set)
 *      atomic_store(&ref, pl_new(128, 6));
 *      printf("ref's value is %p\n", atomic_load(&ref));
 */
typedef _Atomic(ref_t) atomic_ref_t;
typedef _Atomic(byte_t) atomic_byte_t;

/*
 * Structure 'RootSet_t' is used to record root set reference information. Clearly, Type 'pRootSet_t' is its pointer form.
 */
typedef struct RootSet RootSet_t, *pRootSet_t;

/*
 * Function 'pl_init' initializes the basic gc information.
 * This function should be called atomically before using any functions in this library.
 * 
 * Parameters:
 *      @appThrdCount: the number of application threads.
 *      @gcThrdCount: the number of gc threads.
 *      @heapStart: the address of gc heap.
 *      @heapSize: the size of gc heap.
 * 
 * Return values:
 *      Returns 0 if succeed. 
 *      It will return 1 if 'malloc' can not allocate enough memory for the basic root sets. 
 *      If gc threads can't be detached, it will return 2.
 */
int pl_init(int appThrdCount, int gcThrdCount, void* heapStart, size_t heapSize);

// To get the number of gc thread(s).
int pl_thrdCount();

// To get the size of allocated memory by 'pl_new'.
size_t pl_allocated();

// To get the size of collected memory by gc threads.
size_t pl_collected();

// To get the number of routines that gc threads have go through.
int pl_routineCount();

// To get the summary time application threads have paused(exactly waited).
// WARNING: 'pl_appThrdPausedTime' doesn't have an implementation for the time being.
clock_t pl_appThrdPausedTime();     // us

// The size of structure 'RootSet_t'
#define PL_GC_SIZEOF_ROOTSET_T 32
// A null value for a reference.
#define null 0

/*
 * I consider that each application thread has a basic root set.
 * 'pl_getRootSet' uses the ID value(0, 1, 2, 3...) of a thread to get the basic root set object of it.
 * 
 * Parameter:
 *      @thrdID: the ID value of the requiring thread.
 * 
 * Return values:
 *      If succeed, return the pointer of the root set object, Otherwise, returns NULL.
 */
pRootSet_t pl_getRootSet(int thrdID);

/*
 * 'pl_newRootSet' requires some necessary data to provide gc threads with root set information.
 * 
 * Parameters:
 *      @placement: the address of a new 'RootSet_t' typed data.
 *      @prv: the pointer of the previous 'RootSet_t' object.
 *      @start: the start address of a reference array.
 *      @refCount: the size of the reference array.
 * 
 * Return value:
 *      Returns 'pRootSet_t' typed @placement.
 */
pRootSet_t pl_newRootSet(void* placement, pRootSet_t prv, atomic_ref_t* start, size_t refCount);

/*
 * This function deletes a 'RootSet_t' object.
 * WARNING: calling this function may cause a short pause of waiting the finish of the analyzing phase run by the gc threads
 * 
 * Parameters:
 *      @prv: the pointer of the previous 'RootSet_t' object.
 *      @dtor: a function pointer to the destructor function of the target object, nullable;
 * 
 */
void pl_deleteRootSet(pRootSet_t prv, void(*dtor)());

/*
 * 'pl_new' allocate an object from gc heap.
 * WARNING: this function may also cause a pause in order to wait for a new routine of gc if the first time allocation gets null.
 *
 * Parameters:
 *      @size: the size of the allocating object.
 *      @refCount: the number of the reference array in the object.
 * 
 * Return values:
 *      Returns an legal reference if succeed.
 *      Similar to 'malloc', returns null if there's no enough memory for a new object.
 */
ref_t pl_new(size_t size, size_t refCount);

/*
 * 'pl_loadObjRef' tries to heal the reference pointed by 'pRef' and get the pointer to a reference's address inside the object.
 * WARNING: the following 4 functions may alse cause a pause in order to wait for the finish of gc phases
 * 
 * Parameters:
 *      @pRef: the pointer to a reference of the object.
 *      @idx: the index value of the reference array inside the object which can be used to refer to the address of the reference inside the object.
 * 
 * Return value:
 *      The address of the requiring reference inside the object.
 */
atomic_ref_t* pl_loadObjRef(atomic_ref_t* pRef, size_t idx);

/*
 * 'pl_storeObjRef' tries to heal the reference pointed by 'pRef' and store the reference pointed by 'pValue' to this object
 * 
 * Parameters:
 *      @pRef: the pointer to a reference of the object.
 *      @idx: the index value of the reference array inside the object which can be used to refer to the address of the reference inside the object.
 *      @pValue: the pointer to a reference which needs storing into the object. If it equals null, the target will be set to be null.
 */
void pl_storeObjRef(atomic_ref_t* pRef, size_t idx, atomic_ref_t* pValue);

/*
 * 'pl_load' tries to heal the reference pointed by 'pRef' and is called to load the value represented by the location of the offset target.
 *
 * Parameters:
 *      @pRef: the pointer to the reference of the object.
 *      @offs: the offset value which is used to reach the target's address.
 * 
 * Return values:
 *      The pointed value by the target offset of the object.
 */
void* pl_load(atomic_ref_t* pRef, size_t offs);

/*
 * 'pl_store' 'pl_load' tries to heal the reference pointed by 'pRef' and stores 'value' into target location.
 *
 * Parameters:
 *      @pRef: the pointer to the reference of the object.
 *      @offs: the offset value which is used to reach the target's address.
 *      @value: the expected value
 * 
 */
void pl_store(atomic_ref_t* pRef, size_t offs, void* value);

#ifdef __cplusplus
}
#endif 


#endif