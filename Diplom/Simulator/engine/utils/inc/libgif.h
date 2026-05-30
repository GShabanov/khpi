/***************************************************************************************\
*   File:                                                                               *
*       libgif.h                                                                        *
*                                                                                       *
*   Abstract:                                                                           *
*       gif file parsing                                                                *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-May-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 59 $

#ifndef __LIBGIF_H__
#define __LIBGIF_H__

extern BOOL 
LoadGIFBuff(class IDataStream *pStream, struct _GFX_INFO *gfxInfo);


#endif // __LIBGIF_H__
