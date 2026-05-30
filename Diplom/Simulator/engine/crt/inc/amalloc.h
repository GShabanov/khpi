/*******************************************************************************
*   File:                                                                      *
*       amalloc.h                                                              *
*                                                                              *
*   Abstract:                                                                  *
*       allocator implementation                                               *
*                                                                              *
*   Author:                                                                    *
*       Gennady Shabanov (gennady.shabanov@syssafety.com) 13-Feb-2006          *
*                                                                              *
*   Revision History:                                                          *
*******************************************************************************/
// $Id: $
#ifndef __AMALLOC_H__
#define __AMALLOC_H__

extern size_t  zones_size;

extern BOOL
Alloc_Init();

extern VOID
Alloc_Finalyze();

extern PVOID __cdecl
Alloc(size_t size);

extern void __cdecl
Free(PVOID pointer);


#endif // __AMALLOC_H__
