/***************************************************************************************\
*   File:                                                                               *
*       fastmath.cpp                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       fast mathematics eqquations                                                     *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 26-Dec-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#include "stdafx.h"
#include "fastmath.h"

static BYTE  *g_bAlphaTable = 0;
BOOL
InitFastMath()
{
    BYTE *bFastMath = (BYTE *)
        VirtualAlloc(0, 256 * 256 * sizeof(BYTE), MEM_COMMIT, PAGE_READWRITE);

    if (bFastMath == 0)
        return FALSE;

    for (DWORD i = 0; i < 256; i++)
    {
        for (DWORD j = 0; j < 256; j++)
        {
            bFastMath[i * 256 + j] = (BYTE)((i * j) >> 8);
        }
    }

    g_bAlphaTable = bFastMath;

    return TRUE;
}
VOID
FinalizeFastMath()
{
    VirtualFree(g_bAlphaTable, 0, MEM_RELEASE);

    g_bAlphaTable = 0;
}

BYTE
AlphaBlendValues(const BYTE a, const BYTE b)
{
    return *(g_bAlphaTable + (a << 8) + b);
}
