/***************************************************************************************\
*   File:                                                                               *
*       soundOutput.cpp                                                                   *
*                                                                                       *
*   Abstract:                                                                           *
*       access to dirrect sound buffer                                                  *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 33 $
#include "stdafx.h"
#include "soundOutput.h"

#define DIRECTSOUND_VERSION 0x0800

#include <WinError.h>
#include <dsound.h>

#define SECONDARY_BUFFER_SIZE   0x10000

#undef DEFINE_GUID

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

// DirectSound Component GUID {47D4D946-62E8-11CF-93BC-444553540000}
DEFINE_GUID(CLSID_DirectSound, 0x47d4d946, 0x62e8, 0x11cf, 0x93, 0xbc, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0);
DEFINE_GUID(IID_IDirectSound, 0x279AFA83, 0x4981, 0x11CE, 0xA5, 0x21, 0x00, 0x20, 0xAF, 0x0B, 0xE5, 0x60);
// DirectSound 8.0 Component GUID {3901CC3F-84B5-4FA4-BA35-AA8172B8A09B}
DEFINE_GUID(CLSID_DirectSound8, 0x3901cc3f, 0x84b5, 0x4fa4, 0xba, 0x35, 0xaa, 0x81, 0x72, 0xb8, 0xa0, 0x9b);
DEFINE_GUID(IID_IDirectSound8, 0xC50A7E93, 0xF395, 0x4834, 0x9E, 0xF6, 0x7F, 0xA9, 0x9D, 0xE5, 0x09, 0x66);

BOOL
CDirrectSound::iInit(HWND hWindow)
{
    LPDIRECTSOUND pDS = NULL;
    bool          bUse8 = true;
    HRESULT       hResult = 0;
    WAVEFORMATEX  format;
    DSBUFFERDESC  dsbuf;
    DSBCAPS       dsbcaps;


    LPDIRECTSOUNDBUFFER pDSBuf;

    CoInitialize(NULL);

    //
    // Create IDirectSound using the primary sound device
    //
    hResult = CoCreateInstance(CLSID_DirectSound8, NULL, CLSCTX_INPROC_SERVER, IID_IDirectSound8, (void **)&pDS);

    if (hResult != S_OK) 
    {
        bUse8 = false;

        hResult = CoCreateInstance(CLSID_DirectSound, NULL, CLSCTX_INPROC_SERVER, IID_IDirectSound, (void **)&pDS);

        if (hResult != S_OK)
        {
            goto __release_com;
        }
    }

    if (pDS->Initialize(NULL) != S_OK)
    {
        goto __release_dirrect_sound;
    }

    pDS->SetCooperativeLevel(hWindow, DSSCL_PRIORITY);

    //
    // WAVE format descriptor
    //
    memset(&format, 0, sizeof(format));

    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.wBitsPerSample = 16;
    format.nSamplesPerSec = 22050;
    format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;
    format.cbSize = 0;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

    //
    // SOUND buffer descriptor
    //
    memset(&dsbuf, 0, sizeof(dsbuf));
    dsbuf.dwSize = sizeof(DSBUFFERDESC);
    dsbuf.dwFlags = DSBCAPS_LOCHARDWARE;

    if (bUse8)
    {
        dsbuf.dwFlags |= DSBCAPS_GETCURRENTPOSITION2;
    }

    dsbuf.dwBufferBytes = SECONDARY_BUFFER_SIZE;
    dsbuf.lpwfxFormat = &format;


    if (DS_OK != pDS->CreateSoundBuffer(&dsbuf, &pDSBuf, NULL))
    {
        // Couldn't get hardware, fallback to software.
        dsbuf.dwFlags = DSBCAPS_LOCSOFTWARE;

        if (bUse8)
        {
            dsbuf.dwFlags |= DSBCAPS_GETCURRENTPOSITION2;
        }

        if (DS_OK != pDS->CreateSoundBuffer(&dsbuf, &pDSBuf, NULL))
        {
            goto __release_dirrect_sound;
        }

    }

    // Make sure mixer is active
    if (DS_OK != pDSBuf->Play(0, 0, DSBPLAY_LOOPING))
    {
        goto __release_dirrect_buffer;
    }

    memset(&dsbcaps, 0, sizeof(dsbcaps));

    dsbcaps.dwSize = sizeof(dsbcaps);

    // get the returned buffer size
    if (DS_OK != pDSBuf->GetCaps(&dsbcaps))
    {
        goto __release_dirrect_buffer;
    }

    this->m_nChannels = format.nChannels;
    this->m_wBitsPerSample = format.wBitsPerSample;
    this->m_nSamplesPerSec = format.nSamplesPerSec;
    this->m_SoundBufferSize = dsbcaps.dwBufferBytes;

    this->m_b16Bit = (format.wBitsPerSample >> 3) - 1;
    this->m_SampleSize = format.nChannels * (format.wBitsPerSample >> 3);

    this->m_dwSamples = dsbcaps.dwBufferBytes / this->m_SampleSize;

    this->m_pBuffer = pDSBuf;
    this->m_pDirrectSound = pDS;

    return TRUE;

__release_dirrect_buffer:
    pDSBuf->Release();

__release_dirrect_sound:
    pDS->Release();

__release_com:
    CoUninitialize();

    return FALSE;
}

void
CDirrectSound::iFree()
{

    this->m_pDirrectSound->Release();

    CoUninitialize();
}

DWORD
CDirrectSound::iGetCurrentSample()
{
    DWORD   dwPlayCursor;
    DWORD   dwWriteCursor;
    DWORD   dwSample;
    if (this->m_pBuffer->GetCurrentPosition(&dwPlayCursor, &dwWriteCursor) != S_OK)
    {
        return 0;
    }

    dwSample = dwWriteCursor;

    dwSample >>= this->m_b16Bit;

    dwSample &= (this->m_dwSamples - 1);

    return dwSample;
}

void
CDirrectSound::iCopyData(DWORD offset, PVOID data, SIZE_T size)
{
    PVOID   BufferPos1 = 0;
    DWORD   dwBufferBytes1 = 0;
    PVOID   BufferPos2 = 0;
    DWORD   dwBufferBytes2 = 0;

    HRESULT  hResult = 0;

    DWORD  dwStatus = 0;

    int repeat = 0;

    this->m_pBuffer->GetStatus(&dwStatus);

    if (dwStatus & DSBSTATUS_BUFFERLOST)
        this->m_pBuffer->Restore();
    
    if (!(dwStatus & DSBSTATUS_PLAYING))
        this->m_pBuffer->Play(0, 0, DSBPLAY_LOOPING);

    while ((hResult = this->m_pBuffer->Lock(offset, size, &BufferPos1, &dwBufferBytes1,
            &BufferPos2, &dwBufferBytes2, 0)) != DS_OK)
    {
        if (hResult != DSERR_BUFFERLOST)
        {
            return;
        }
        else
        {
            this->m_pBuffer->Restore();
        }

        if (++repeat > 2)
            return;
    }

    if (size <= dwBufferBytes1)
    {
        memcpy(BufferPos1, data, size);
        dwBufferBytes1 = size;
        dwBufferBytes2 = 0;
    }
    else
    {
        memcpy(BufferPos1, data, dwBufferBytes1);

        size -= dwBufferBytes1;

        if (size <= dwBufferBytes2)
        {
            memcpy(BufferPos2, (unsigned char *)data + dwBufferBytes1, size);
            dwBufferBytes2 = size;
        }
        else
        {
            memcpy(BufferPos2, (unsigned char *)data + dwBufferBytes1, dwBufferBytes2);
        }
    }

    //memcpy(BufferPos1, data, dataSize);

    this->m_pBuffer->Unlock(BufferPos1, dwBufferBytes1, BufferPos2, dwBufferBytes2);

}