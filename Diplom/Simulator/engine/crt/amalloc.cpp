/*******************************************************************************
*   File:                                                                      *
*       crt.cpp                                                                *
*                                                                              *
*   Abstract:                                                                  *
*       base CRT routines implementation                                       *
*                                                                              *
*   Author:                                                                    *
*       Gennady Shabanov (gennady.shabanov@syssafety.com) 13-Feb-2006          *
*                                                                              *
*   Revision History:                                                          *
*******************************************************************************/
// $Id: $
#include "stdafx.h"
#include "..\crt\inc\amalloc.h"

size_t  zones_size;

#define MINFRAGMENT     64
#define CACHE_LINE      64

#ifndef _DEBUG
#define COOKIE          0x1d4a11
#else // _DEBUG
#define COOKIE          'ENOZ'
#endif // _DEBUG

#pragma pack (push, 1)

typedef enum _memtag_t {
    TAG_FREE    = 0,
    TAG_GENERAL,
    TAG_SMALL,
    TAG_STATIC
} memtag_t;

typedef struct _HEAP_BLOCK {
    struct _HEAP_BLOCK  *prev;
    struct _HEAP_BLOCK  *next;
    struct _HEAP_ZONE   *zone;     // zone ID
    size_t               size;     // including the header and possibly tiny fragments
    memtag_t             tag;      // a tag of 0 is a free block
} HEAP_BLOCK, *PHEAP_BLOCK;

typedef union _SMALL_HEAP_DESCRIPTOR {
    union {
        struct {
            unsigned long  bucket    : 22;
            unsigned long  rover     : 5;
            unsigned long  free      : 5;
        } bits;
        unsigned long dword;
    };
} SMALL_HEAP_DESCRIPTOR, *PSMALL_HEAP_DESCRIPTOR;

typedef struct _SMALL_HEAP {
    SMALL_HEAP_DESCRIPTOR descriptor;
    struct _SMALL_HEAP   *next;
} SMALL_HEAP, *PSMALL_HEAP;


typedef struct _HEAP_SMALL_BLOCK {
    struct _SMALL_HEAP  *sheap;     // small heap we belong to
    memtag_t             tag;      // a tag of 0 is a free block
} HEAP_SMALL_BLOCK, *PHEAP_SMALL_BLOCK;

typedef struct _HEAP_ZONE {
    size_t                  commited;      // total bytes malloced, including header
    size_t                  highwater;     // allocator highwater mark 
    size_t                  free;          // free memory in zone
    struct _SMALL_HEAP      *smallheaps;
    struct _HEAP_ZONE       *next;         // next zone
    struct _HEAP_BLOCK      *rover;
    HEAP_BLOCK              blocklist;     // start / end cap for linked list
} HEAP_ZONE, *PHEAP_ZONE;
#pragma pack(pop)

DECLSPEC_CACHEALIGN \
CRITICAL_SECTION     allocatorLock;
// main zone for all "dynamic" memory allocation
HEAP_ZONE           *_HeadZone;

HEAP_ZONE *
ZoneAlloc(size_t zoneSize);

void *SmallHeapAlloc(HEAP_ZONE *headZone);
void SmallHeapFree(void *pointer);

//#define SMALL_BLOCK_SIZE (ROUND_UP(((0x1000 - ROUND_UP(sizeof(HEAP_ZONE), CACHE_LINE)) / 22), CACHE_LINE))

#define SMALL_BLOCK_SIZE 0xB0

void *
SmallHeapAlloc(HEAP_ZONE *zone)
{
    register HEAP_SMALL_BLOCK *smallBlock = 0;

    SMALL_HEAP  *smallHeap = zone->smallheaps;

    do
    {
        SMALL_HEAP_DESCRIPTOR oldvalue = smallHeap->descriptor;

        while (oldvalue.bits.free != 0)
        {
            SMALL_HEAP_DESCRIPTOR  newvalue = oldvalue;

            newvalue.bits.free--;
            unsigned long index = (unsigned long)-1;
            
            if (!(newvalue.bits.bucket & (1 << newvalue.bits.rover)))
            {
                index = newvalue.bits.rover;
                newvalue.bits.rover++;

                if (newvalue.bits.rover > 21)
                    newvalue.bits.rover = 0;
            }
            else
            {
                register unsigned long val = 1;

                for (unsigned long i = 0; i < 22; i++)
                {
                    if (!(newvalue.bits.bucket & val))
                    {
                        index = i;

                        newvalue.bits.rover++;
                        if (newvalue.bits.rover > 21)
                            newvalue.bits.rover = 0;

                        break;
                    }

                    val <<= 1;
                }
            }

            if (index == (unsigned long)-1)
                break;

            newvalue.bits.bucket |= (1 << index);

            //
            // success
            //
            if (InterlockedCompareExchange((LONG *)&zone->smallheaps, (LONG)newvalue.dword, (LONG)oldvalue.dword) == (LONG)oldvalue.dword)
            {
                smallBlock = (HEAP_SMALL_BLOCK *)((UCHAR *)zone + ROUND_UP(sizeof(HEAP_ZONE), CACHE_LINE) + 
                    (SMALL_BLOCK_SIZE * index) - sizeof(HEAP_SMALL_BLOCK));

                smallBlock->tag = (memtag_t)(TAG_SMALL | (index << 16));
                smallBlock->sheap = zone->smallheaps;

                return (void *) ((UCHAR *)smallBlock + sizeof(HEAP_SMALL_BLOCK));
            }

        }

        smallHeap = smallHeap->next;
    }
    while (smallHeap != 0);


    return 0;
}

void SmallHeapFree(void *pointer)
{
    register HEAP_SMALL_BLOCK *smallBlock = (HEAP_SMALL_BLOCK *)((UCHAR *)pointer - sizeof(HEAP_SMALL_BLOCK));

    SMALL_HEAP *sheap = smallBlock->sheap;

    unsigned long index = (unsigned long)(smallBlock->tag >> 16);

#ifdef _DEBUG
    if (index > 21) {
        __debugbreak();
    }
#endif // _DEBUG
    SMALL_HEAP_DESCRIPTOR  oldvalue;
    SMALL_HEAP_DESCRIPTOR  newvalue;
    do 
    {
        oldvalue = sheap->descriptor;
        newvalue = oldvalue;

        newvalue.bits.rover = index;
        newvalue.bits.free++;
        newvalue.bits.bucket &= ~(1 << index);

        
    } while (!(InterlockedCompareExchange((LONG *)&sheap->descriptor, (LONG)newvalue.dword, (LONG)oldvalue.dword) == (LONG)oldvalue.dword));
}

PVOID __cdecl
Alloc(size_t size)
{
    register HEAP_BLOCK  *base = NULL;
    //
    // scan through the block list looking for the first free block
    // of sufficient size
    //
    size += sizeof(HEAP_BLOCK); // account for size of block header
    size += sizeof(int);        // space for memory trash tester
    //size = (size + 3) & ~3;   // align to 32 bit boundary
    size = ROUND_UP(size, CACHE_LINE); // align to cache line

    /*if (size < (SMALL_BLOCK_SIZE - sizeof(HEAP_SMALL_BLOCK)))
    {
        void *smallBlock = SmallHeapAlloc(_HeadZone);

        if (smallBlock != 0)
            return smallBlock;
    }*/

    EnterCriticalSection(&allocatorLock);

    HEAP_ZONE  *zone = _HeadZone;

    do
    {

        //
        // search for zone with free space
        //
        while (zone->free < size)
        {
            //
            // check if the next zone not created
            //
            if (zone->next == NULL)
            {
                register size_t  alloc_size = ((size) > (zone->commited * 2)) ? (size) : (zone->commited * 2);

                HEAP_ZONE  *new_zone = ZoneAlloc(alloc_size);

                if (new_zone == NULL)
                {
                    LeaveCriticalSection(&allocatorLock);
                    return NULL;
                }

                zone->next = new_zone;
            }

            zone = zone->next;
        }

        register HEAP_BLOCK  *rover = zone->rover;
        base = rover;
        register HEAP_BLOCK  *start = base->prev;


        do
        {
            //
            // switch zone or create new
            //
            if (rover == start)
            {
                if (zone->next == NULL)
                {
                    register size_t  alloc_size = ((size) > (zone->commited * 2)) ? (size) : (zone->commited * 2);

                    HEAP_ZONE  *new_zone = ZoneAlloc(alloc_size);

                    if (new_zone == NULL)
                    {
                        LeaveCriticalSection(&allocatorLock);
                        return NULL;
                    }

                    zone->next = new_zone;
                }

                zone = zone->next;
                
                //
                // reinialize variables
                //
                rover = zone->rover;
                base = rover;
                start = base->prev;
            }

            if (rover->tag != TAG_FREE)
            {
                base = rover->next;
            }

            rover = rover->next;

        }
        while (base->tag != TAG_FREE || base->size <= size);

    }
    while (base == NULL);

    if ((size_t)((UCHAR *)base - (UCHAR *)zone + size) > zone->highwater)
    {
        zone->highwater = (UCHAR *)base - (UCHAR *)zone + size;
    }

    zone->free -= size;

    //
    // found a block big enough
    //
    size_t extra = base->size - size;

    if (extra > MINFRAGMENT)
    {
        // there will be a free fragment after the allocated block
        HEAP_BLOCK  *free_block = (HEAP_BLOCK *)((UCHAR *)base + size);

        free_block->tag  = TAG_FREE;           // free block
        free_block->size = extra;
        free_block->zone = zone;
        free_block->prev = base;
        free_block->next = base->next;
        free_block->next->prev = free_block;

        base->next = free_block;
        base->size = size;
    }
        
    zone->rover = base->next;   // next allocation will start looking here

    base->tag  = TAG_GENERAL;            // no longer a free block
    base->zone = zone;

    LeaveCriticalSection(&allocatorLock);

    // marker for memory trash testing
    *(int *)((UCHAR *)base + base->size - sizeof(int)) = COOKIE;

    return (void *) ((UCHAR *)base + sizeof(HEAP_BLOCK));
}

void __cdecl
Free(PVOID pointer)
{
    memtag_t    tag = *(memtag_t *)((UCHAR *)pointer - sizeof(memtag_t));


#ifdef _DEBUG
    if ((tag & 0xFFFF) == TAG_FREE) {
        __debugbreak();
        // freed a freed pointer
    }
#endif // _DEBUG

    if ((tag & 0xFFFF) == TAG_SMALL) {
        return SmallHeapFree(pointer);
    }

    HEAP_BLOCK  *block = (HEAP_BLOCK *) ((UCHAR *)pointer - sizeof(HEAP_BLOCK));

#ifdef _DEBUG
    if (block->zone == NULL) {
        __debugbreak();
        // freed a pointer without ZONEID
    }

    // check the memory trash tester
    if (*(int *)((UCHAR *)block + block->size - sizeof(int) ) != COOKIE)
    {
        __debugbreak();
        // memory block wrote past end"
    }
#endif // _DEBUG

#ifndef _DEBUG
    //
    // sanitize block data for release version
    //
    register ULONG  ulRetAddressData = *(ULONG *)_ReturnAddress();
    register ULONG *cBlockData = (ULONG *)pointer;

    for (register size_t i = 0; i < ((block->size - sizeof(HEAP_BLOCK)) / sizeof(ULONG)); i++) {

        *cBlockData ^= ulRetAddressData;

        ulRetAddressData = *cBlockData;
        cBlockData++;
    }
#else
    memset(pointer, 0, block->size - sizeof(HEAP_BLOCK));
#endif // _DEBUG

    HEAP_ZONE  *zone = block->zone;

    EnterCriticalSection(&allocatorLock);

    if (((UCHAR *)block - (UCHAR *)zone + block->size) >= zone->highwater)
    {
        zone->highwater = ((UCHAR *)block - (UCHAR *)zone);
    }

    zone->free += block->size;
    block->tag = TAG_FREE;     // mark as free
    
    // merge with previous free block
    HEAP_BLOCK *other = block->prev;

    if (other->tag == TAG_FREE)
    {

        other->size += block->size;
        other->next = block->next;
        other->next->prev = other;

        if (block == zone->rover) {
            zone->rover = other;
        }

        block = other;
    }

    zone->rover = block;

    // merge the next free block onto the end
    other = block->next;

    if (other->tag == TAG_FREE)
    {
        block->size += other->size;
        block->next = other->next;
        block->next->prev = block;

        if (other == zone->rover) {
            zone->rover = block;
        }

        zone->free += sizeof(HEAP_BLOCK);
    }

    LeaveCriticalSection(&allocatorLock);
}

BOOL
Alloc_Init()
{
    InitializeCriticalSection(&allocatorLock);

    zones_size = 0;

    SIZE_T  zoneSize = 0x100000;

    HEAP_ZONE *zone = ZoneAlloc(zoneSize);

    if (zone == NULL)
        return FALSE;

    _HeadZone = zone;

    return TRUE;
}

VOID
Alloc_Finalyze()
{

    do
    {
        HEAP_ZONE *zone = _HeadZone;
        _HeadZone = zone->next;

        VirtualFree(zone, 0, MEM_RELEASE);

    }
    while (_HeadZone != NULL);

    DeleteCriticalSection(&allocatorLock);
}

HEAP_ZONE *
ZoneAlloc(size_t zoneSize)
{
    register HEAP_ZONE *zone = reinterpret_cast<HEAP_ZONE *>(
        VirtualAlloc(0, zoneSize, MEM_COMMIT, PAGE_READWRITE));

    if (zone == NULL)
        return NULL;

    zones_size += zoneSize;

    //HEAP_BLOCK  *block = (HEAP_BLOCK *)((byte *)_mainZone + sizeof(HEAP_ZONE));
    //-------------------------------------------------------------------
    // header of a block must be cache alligned by the body of block

    HEAP_BLOCK  *block = (HEAP_BLOCK *)
        (ROUND_UP((UCHAR *)zone + 0x1000 + 1, CACHE_LINE) - sizeof(HEAP_BLOCK));
    /*HEAP_BLOCK  *block = (HEAP_BLOCK *)
        (ROUND_UP((UCHAR *)zone + sizeof(HEAP_ZONE) + CACHE_LINE, CACHE_LINE) - sizeof(HEAP_BLOCK));*/

    block->prev = &zone->blocklist;
    block->next = &zone->blocklist;
    block->tag = TAG_FREE;         // free block
    block->zone = zone;
    block->size = zoneSize - ((UCHAR *)block - (UCHAR *)zone);

    // set the entire zone to one free block
    zone->blocklist.next = block;
    zone->blocklist.prev = block;
    zone->blocklist.tag = TAG_GENERAL;    // in use block
    zone->blocklist.zone = zone;
    zone->blocklist.size = 0;

    zone->smallheaps = 0;

    zone->next = NULL;
    zone->rover = block;
    zone->free = block->size;
    zone->highwater = ((UCHAR *)block - (UCHAR *)zone);
    zone->commited = zoneSize;

    return zone;
}
