/***************************************************************************************\
*   File:                                                                               *
*       riffFile.cpp                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       RIFF file format parser implementation                                          *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 11-Feb-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 51 $
#include "stdafx.h"
#include "inc\waveFormat.h"
#include "inc\sound.h"
#include "inc\riffFile.h"
#include "..\fs\inc\fileStream.h"

BOOL
CRiffWaveSource::Init(IDataStream *pDataStream)
{
    if (pDataStream->Reference() == 0)
    {
        return FALSE;
    }

    SIZE_T dataSize = pDataStream->Length();
    this->m_MemSize = dataSize;

    this->m_pbData = (short *)malloc(dataSize);

    if (this->m_pbData == 0)
    {
        pDataStream->Dereference();
        return FALSE;
    }

    SIZE_T readed = pDataStream->Read(this->m_pbData, dataSize);

    pDataStream->Dereference();

    if (readed < dataSize)
    {
        free(this->m_pbData);
        return FALSE;
    }

    if (ReadMMIO() != 0)
    {
        free(this->m_pbData);
        // ReadMMIO will fail if its an not a wave file
        return FALSE;
    }

    //mfileTime = mhmmio->Timestamp();

    /*if (iResetFile() != S_OK)
    {
        //Close();
        return FALSE;
    }*/

    // After the reset, the size of the wav file is mck.cksize so store it now
    //m_dwSize = mck.cksize / sizeof(short);
    //mMemSize = mck.cksize;

    /*if (mck.cksize != 0xffffffff )
    {
        memcpy(&this->m_Wfx, (waveformatex *)&m_Wfx, sizeof(waveformatex));
        return 0;
    }*/

    return TRUE;

}

VOID
CRiffWaveSource::Free()
{
}

/*int
CRiffWaveSource::iResetFile()
{
    unsigned char *ptr = (unsigned char *)this->m_pbData + 12;
    mminfo        ckIn;
    int          _read = 0;
    // Search the input file for for the 'fmt ' chunk.

    do
    {

        _read = SafeGetData(ptr, &ckIn, 8);

        if (_read != 8)
        {
            return -1;
        }

        ptr += _read;

        ckIn.dwDataOffset += ckIn.cksize - 8;

    }
    while (ckIn.ckid != mmioFOURCC('d', 'a', 't', 'a'));

    return S_OK;
}*/


int
CRiffWaveSource::SafeGetData(BYTE *ptr, PVOID pData, SIZE_T size)
{
    if ( ((SIZE_T)((unsigned char *)ptr - (unsigned char *)this->m_pbData) > this->m_MemSize) || 
        ((SIZE_T)((unsigned char *)ptr - (unsigned char *)this->m_pbData + size) > this->m_MemSize))
    {
        return -1;
    }

    memcpy(pData, ptr, size);

    return size;
}

int
CRiffWaveSource::ReadMMIO(void)
//-----------------------------------------------------------------------------
// Name: idWaveFile::ReadMMIO()
// Desc: Support function for reading from a multimedia I/O stream.
//       mhmmio must be valid before calling.  This function uses it to
//       update mckRiff, and mpwfx. 
//-----------------------------------------------------------------------------
{
    waveformatex          waveFormat;   // Temp PCM structure to load in.
    mminfo                ckRiff;
    mminfo                ckIn;
    mminfo                ckData;

    unsigned char *ptr = (unsigned char *)this->m_pbData;

    memset(&m_Wfx, 0, sizeof(waveformatextensible));

    int _read = SafeGetData(ptr, &ckRiff, FIELD_OFFSET(mminfo, dwDataOffset));

    if (-1 == _read)
    {
        return -1;
    }

    ptr += _read;

    ckRiff.dwDataOffset = FIELD_OFFSET(mminfo, dwDataOffset);

    // Check to make sure this is a valid wave file
    if((ckRiff.ckid != fourcc_riff) || (ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
    {
        return -1;
    }

    // Search the input file for for the 'fmt ' chunk.

    do {

        _read = SafeGetData(ptr, &ckIn, FIELD_OFFSET(mminfo, fccType));

        if (_read != FIELD_OFFSET(mminfo, fccType))
        {
            return -1;
        }

        ptr += _read;

        ckIn.dwDataOffset += ckIn.cksize - FIELD_OFFSET(mminfo, fccType);

    }
    while (ckIn.ckid != mmioFOURCC('f', 'm', 't', ' '));

    // Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
    // if there are extra parameters at the end, we'll ignore them
    if (ckIn.cksize < sizeof(pcmwaveformat))
    {
        return -1;
    }

    // Read the 'fmt ' chunk into <pcmWaveFormat>.
    _read = SafeGetData(ptr, &waveFormat, ckIn.cksize);

    if ((DWORD)_read != ckIn.cksize)
    {
        return -1;
    }

    ptr += _read;

    // Copy the bytes from the pcm structure to the waveformatex_t structure
    memcpy(&m_Wfx, &waveFormat, ckIn.cksize);

    // Allocate the waveformatex_t, but if its not pcm format, read the next
    // word, and thats how many extra bytes to allocate.
    if (waveFormat.wFormatTag == WAVE_FORMAT_TAG_PCM)
    {
        m_Wfx.Format.cbSize = 0;
    }
    else
    {
#if 0
        // Read in length of extra bytes.
        word cbExtraBytes = 0L;
        if( mhmmio->Read( (char*)&cbExtraBytes, sizeof(word) ) != sizeof(word) )
            return -1;

        mpwfx.Format.cbSize = cbExtraBytes;

        // Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
        if (mhmmio->Read( (char*)(((byte*)&(mpwfx.Format.cbSize))+sizeof(word)), cbExtraBytes ) != cbExtraBytes ) {
            memset( &mpwfx, 0, sizeof( waveformatextensible_t ) );
            return -1;
        }
#endif
        return -1;  // we don't handle these (32 bit wavefiles, etc)
    }

    for (;;)
    {
        _read = SafeGetData(ptr, &ckData, FIELD_OFFSET(mminfo, fccType));

        if (_read != FIELD_OFFSET(mminfo, fccType))
        {
            return -1;
        }

        ptr += _read;

        switch (ckData.ckid)
        {
        case mmioFOURCC('d', 'a', 't', 'a'):
            this->m_pPcmData = ptr;
            this->m_tPcmDataSize = (SIZE_T)ckData.cksize;
            goto __riff_end;

        case mmioFOURCC('f', 'a', 'c', 't'):
        {
            //DWORD dwFactData = *ptr;

            ptr += sizeof(DWORD);
        }
            break;

        default:
            return -1;
        }

    }
__riff_end:

    this->m_MemSize = this->m_tPcmDataSize;
    this->m_dwSize = this->m_tPcmDataSize;

    return 0;
}
