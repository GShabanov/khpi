/***************************************************************************************\
*   File:                                                                               *
*       texture.cpp                                                                     *
*                                                                                       *
*   Abstract:                                                                           *
*       sprite renderer                                                                 *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 26-Dec-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 63 $
#include "stdafx.h"
#include "inc\texture.h"
#include "..\utils\inc\utilites.h"
#include "..\fs\inc\fileStream.h"

BOOL
CTexture::Init(IDataStream *pStream)
{
    if (pStream->Reference() == 0)
    {
        return FALSE;
    }

    this->m_pComressedStream = pStream;

    this->m_pBufferRaw = 0;
    this->m_lRefCount = 0;

    return TRUE;
}

void
CTexture::Free()
{
    this->m_bRemove = TRUE;
    if (InterlockedDecrement(&this->m_lRefCount) <= 0)
    {
        free(this->m_pBufferRaw);
    }

    this->m_pComressedStream->Dereference();
}

void *
CTexture::Reference()
{
    if (InterlockedIncrement(&this->m_lRefCount) == 1)
    {
        GFX_INFO      gfxInfo;

        if ((
            Load_Picture(
            this->m_pComressedStream,
            &gfxInfo) == FALSE) || (gfxInfo.pic == 0))
        {

            InterlockedDecrement(&this->m_lRefCount);
            return NULL;
        }

        this->m_pBufferRaw = gfxInfo.pic;
        this->m_Width = gfxInfo.width;
        this->m_Height = gfxInfo.height;
        this->m_bgColor = gfxInfo.bgColor;
    }

    return this->m_pBufferRaw;
}

BOOL
CTexture::Dereference()
{
    if (InterlockedDecrement(&this->m_lRefCount) <= 0)
    {
        free(this->m_pBufferRaw);

        this->m_pBufferRaw = 0;
        return TRUE;
    }

    return FALSE;
}

VOID
CTexture::WaitDereference(HANDLE hWaitObject)
{
    UNREFERENCED_PARAMETER(hWaitObject);
    InterlockedDecrement(&this->m_lRefCount);

}
