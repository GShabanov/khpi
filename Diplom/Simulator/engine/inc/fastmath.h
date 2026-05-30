/***************************************************************************************\
*   File:                                                                               *
*       fastmath.h                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       fast mathemetic equations                                                       *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 26-Dec-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __FASTMATH_H__
#define __FASTMATH_H__

extern BOOL InitFastMath();
extern VOID FinalizeFastMath();

extern BYTE AlphaBlendValues(const BYTE a, const BYTE b);

#endif // __FASTMATH_H__