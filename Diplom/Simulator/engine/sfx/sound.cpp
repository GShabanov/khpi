/***************************************************************************************\
*   File:                                                                               *
*       sound.cpp                                                                       *
*                                                                                       *
*   Abstract:                                                                           *
*       base sound interface                                                            *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 43 $
#include "stdafx.h"
#include "inc\waveFormat.h"
#include "inc\sound.h"
#include "..\fs\inc\filestream.h"


BOOL
CSoundWaveFile::Init(IWaveSource *waveSource)
{
    if (waveSource->Reference() == 0)
        return FALSE;

    this->m_pWaveSource = waveSource;
    this->m_DataCursor = 0;

    return TRUE;
}
VOID 
CSoundWaveFile::Free()
{
    this->m_pWaveSource->Dereference();
}

int
CSoundWaveFile::ResetFile()
{
    return 0;
}

DWORD
CSoundWaveFile::Read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD *pdwSizeRead)
//-----------------------------------------------------------------------------
// Name: idWaveFile::ReadFile()
// Desc: Reads section of data from a wave file into pBuffer and returns 
//       how much read in pdwSizeRead, reading not more than dwSizeToRead.
//       This uses mck to determine where to start reading from.  So 
//       subsequent calls will be continue where the last left off unless 
//       Reset() is called.
//-----------------------------------------------------------------------------
{
    if (pdwSizeRead == 0)
        return 0;

    *pdwSizeRead = 0;

    void *data = this->m_pWaveSource->Reference();

    if (data == 0)
    {
        return 0;
    }

    SIZE_T waveSize = this->m_pWaveSource->GetMemorySize();

    waveSize -= m_DataCursor;

    if (dwSizeToRead > waveSize)
    {
        dwSizeToRead = (DWORD)waveSize;
    }

    memcpy(pBuffer, (unsigned char *)data + m_DataCursor, dwSizeToRead);

    if (pdwSizeRead != NULL)
    {
        *pdwSizeRead = dwSizeToRead;
    }

    m_DataCursor += dwSizeToRead;

    this->m_pWaveSource->Dereference();

    return dwSizeToRead;

}

//-----------------------------------------------------------------------------
// Name: idWaveFile::Close()
// Desc: Closes the wave file 
//-----------------------------------------------------------------------------
int
CSoundWaveFile::Close( void )
{
    return 0;
}

BOOL
CMemoryWaveSource::Init(IDataStream *dataStream)
{

    SIZE_T  dataSize = (SIZE_T)dataStream;

    this->m_MemSize = dataSize;

    BYTE *speakerData = (BYTE *)malloc(this->m_MemSize);

    if (speakerData == 0)
    {
        return FALSE;
    }

    memset(speakerData, 0, this->m_MemSize);

    m_pbData     = (short *)speakerData;
    m_dwSize     = this->m_MemSize / sizeof(short);

    return TRUE;
}

VOID
CMemoryWaveSource::Free()
{
    if (m_pbData)
    {
        free(m_pbData);
    }
}

VOID *
CMemoryWaveSource::Reference()
{
    InterlockedIncrement(&this->m_lRefCount);

    if (this->m_bRemove)
    {
        InterlockedDecrement(&this->m_lRefCount);
        return 0;
    }

    return (VOID *)this->m_pbData;
}
