/* ================================================================ *
 * Copyright (c) 2022 Relight Team - Lizbing. All rights reserved.  *
 * Designed and implemented by Lizbing                              *
 * email: lizbing@relight.team                                      *
 * QQ: 2541666786                                                   *
 *                                                                  *
 * Zero Garbage Collector - A pauseless garbage collector.          *
 * Version: Beta 0.11                                               *
 * Generated on 24/6/2022                                           *
 * ================================================================ */

#ifndef ROSA_VIRTUAL_MACHINE_ZERO_GC_
#define ROSA_VIRTUAL_MACHINE_ZERO_GC_

#include <stdlib.h>

/*
Struct 'zero_ref_t' is the type of reference in zero gc.
*/
typedef struct zero_ref_t {
    long : 64;
} zero_ref_t;

/*
Struct 'RootSet_t' is the data structure which is used to store the references' information in roots.
Clearly, there's only a declaration in the headfile, 'RootSet_t' isn't available.
Its pointer form 'pRootSet_t' is what you need.
*/
struct RootSet_t;
typedef struct RootSet_t RootSet_t, *pRootSet_t;

/*
A null value for zero references.
*/
#define zero_null ((zero_ref_t){ 0 })

/*
It's necessary to allocate memory for a 'RootSet_t' object. However,  we cannot use 'sizeof' operator to get its size.
To solve this problem, I provided the macro 'ZERO_SIZEOF_ROOTSET_T' represented the value of 32.
*/
#define ZERO_SIZEOF_ROOTSET_T 32

// to get gc version
float zero_version();
// to get the right announcement
char* zero_rightInfo();
// to get gc's formal name
char* zero_stdName();

/*
Function 'zero_init' should be called at the very first.
It intializes all the gc information. It will also detach gc threads.

It returns 0 if succeeded. 
The return value of 1 is for failing to allocate the basic root sets;
    2 is for failing to detach gc threads.

@appThrdCount: the number of application threads.
@gcThrdCount: the expected number of gc threads.
@heapStart: the address of the gc-managing heap.
@heapSize: the size of the heap.
*/
int zero_init(size_t appThrdCount, size_t gcThrdCount, void* heapStart, size_t heapSize);

// to get the number of gc threads.
size_t zero_thrdCount();
// to get the total size of allocated memories.
size_t zero_allocated();
// to get the total size of collected memories.
size_t zero_collected();
// to get the number of times of gc routine.
int zero_routine();

/*
Function 'zero_getBasicRootSet' is used to get the basic root set by application thread number.
For an example, please see below.

Returns a pointer to the basic 'RootSet_t' object if success; returns NULL if failed.

@thrdID: the ID number n{ n belongs to N and n < @appThrdCount } of a application thread(Please allocated an ID number for each application thread).
*/
pRootSet_t zero_getBasicRootSet(int thrdID);

/*
Function 'zero_newRootSet' news a 'RootSet_t' object o. o will be linked to the previous 'RootSet_t' object p's back.
For an example, please see below.

Returns a pointer to a new 'RootSet_t' object if success; returns NULL if failed.

@addr: the placement of o.
@prv: the pointer of p.
@start: the addr of managed root.
@refCount: the number of references in the root.
*/
pRootSet_t zero_newRootSet(void* addr, pRootSet_t prv, zero_ref_t* start, size_t refCount);

/*
Function 'zero_deleteRootSet', as its name suggests, it is called to delete and release a 'RootSet_t' object o.
For an example, please see below.

@prv: the pointer of p.
@release: a function pointer that points to the release function of o's memory.
*/
void zero_deleteRootSet(pRootSet_t prv, void(*release)(void*));

/*
example 1:
    // in app thread 0(No. 0):

    zero_ref_t roots1[10] = { zero_null };
    zero_ref_t roots2[16] = { zero_null };

    pRootSet_t b = zero_getBasicRootSet(0);
    pRootSet_t rs1 = zero_newRootSet(alloca(ZERO_SIZEOF_ROOTSET_T), b, roots1, 10);
    ...
    pRootSet_t rs2 = zero_newRootSet(malloc(ZERO_SIZEOF_ROOTSET_T), b, roots2, 16);
    ...
    zero_deleteRootSet(rs1, free);      // delete rs2
    ...
    zero_deleteRootSet(b, NULL);        // delete rs1
*/

/*
Function 'zero_new' allocates a new object managed by zero gc.
Please reference the example below.

Returns 1 if succeeded; 2 if failed.

@pRef: the pointer to a reference.
@size: the size of new object.
@refCount: the number of references in the new object

I predict that all the references in an object gather at the top of this object. It will be like this:

top -> ------------
       |references|
       ------------
       |          |
       |   datas  |
       |          |
       ------------
*/
int zero_new(zero_ref_t* pRef, size_t size, size_t refCount);

/*
Function 'zero_load' loads a 64-bit data d from an object.
Please reference the example below.

Returns d if succeeded.

@pRef: the pointer to a reference.
@offs: the offset of the data in the object.
*/
void* zero_load(zero_ref_t* pRef, size_t offs);

/*
Function 'zero_store' stores a 64-bit data d into an object.
Please reference the example below.

@pDst: the pointer to a reference.
@offs: the offset of the placement in the object which is used to store this data.
@value: the 64-bit data.
*/
void zero_store(zero_ref_t* pDst, size_t offs, void* value);

/*
Function 'zero_storeRef' stores a reference to an object.
Please reference the example below.

@pDst: the pointer to a reference to the target object.
@idx: the index value used to locate the reference placement in the object.
@pRef: the pointer to a reference to the stored object.
*/
void zero_storeRef(zero_ref_t* pDst, size_t idx, zero_ref_t* pRef);

/*
Function 'zero_loadRef' loads a reference from an object.
Please reference the example below.

@pDst: the pointer to a reference which is required to store the object.
@pSrc: the pointer to a reference to the object.
@idx: the index value used to locate the reference in the object.
*/
void zero_loadRef(zero_ref_t* pDst, zero_ref_t* pSrc, size_t idx);

/*
Function 'zero_assign' moves the reference pointed by pRef to pDst

@pDst: the pointer to the destination.
@pRef: the pointer to a reference.
*/
void zero_move(zero_ref_t* pDst, zero_ref_t* pRef);

/*
example 2:
    // 3 available reference r1, r2， r3
    if(!zero_new(&r1, 1024, 10) || !zero_new(&r2, 128, 0))
        exit(1);
    
    zero_store(&r2, 16, 30);    // store 30 to r2 + 16
    zero_storeRef(&r1, 0, &r2); // store r2 to r1 + 0

    int i = zero_load(&r2, 16); // i == 30
    zero_loadRef(&r3, &r1, 0);  // r3 == r2
    zero_move(&r3, &r1);        // r3 == r1

*/

#endif //ROSA_VIRTUAL_MACHINE_ZERO_GC_
