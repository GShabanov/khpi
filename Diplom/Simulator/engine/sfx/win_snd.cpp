/***************************************************************************************\
*   File:                                                                               *
*       win_snd.cpp                                                                     *
*                                                                                       *
*   Abstract:                                                                           *
*       access to dirrect sound buffer                                                  *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 43 $
#include "stdafx.h"
#pragma hdrstop

#include <dsound.h>
// DirectX SDK
#include <ks.h>
#include <ksmedia.h>

#include "inc\waveFormat.h"
#include "inc\sound.h"
#include "inc\win_snd.h"
#include "..\fs\inc\filestream.h"


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

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
/*
================
CAudioHardwareWIN32::iFree()
================
*/
void
CAudioHardwareWIN32::Free()
{
    m_pSpeakers->Free();
    SAFE_DELETE(m_pSpeakers);
    SAFE_RELEASE(m_pDSBPrimary);
    SAFE_RELEASE(m_pDS); 
}

BOOL
CAudioHardwareWIN32::CreateDirrectSoundInstance()
{
    CoInitialize(NULL);

    LPDIRECTSOUND pDS = NULL;
    //
    // Create IDirectSound using the primary sound device
    //
    HRESULT hResult = CoCreateInstance(CLSID_DirectSound8, NULL, CLSCTX_INPROC_SERVER, IID_IDirectSound8, (void **)&pDS);

    if (hResult != S_OK) 
    {
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

    this->m_pDS = pDS;

    return TRUE;

__release_dirrect_sound:
    pDS->Release();

__release_com:
    CoUninitialize();

    return FALSE;
}

/*
===============
CAudioHardwareWIN32::Init
===============
*/
BOOL
CAudioHardwareWIN32::Init(HWND hwnd)
{
    CoInitialize(NULL);

    this->m_BufferSize = 0;
    this->m_iNumSpeakers = 0;
    this->m_iBlockAlign = 0;

    SAFE_RELEASE(m_pDS);

    // Create IDirectSound using the primary sound device
    if (CreateDirrectSoundInstance() != TRUE)
    {
        return FALSE;
    }

    // Set primary buffer format
    this->SetPrimaryBufferFormat(hwnd, PRIMARYFREQ, 16, 2);

    return TRUE;
}

void
CAudioHardwareWIN32::SetPrimaryBufferFormat(
    HWND  hWnd,
    DWORD dwPrimaryFreq,
    DWORD dwPrimaryBitRate,
    DWORD dwSpeakers )
/*
===============
CAudioHardwareWIN32::SetPrimaryBufferFormat
Set primary buffer to a specified format 
For example, to set the primary buffer format to 22kHz stereo, 16-bit
then:   dwPrimaryChannels = 2
        dwPrimaryFreq     = 22050, 
        dwPrimaryBitRate  = 16
===============
*/
{
    if (m_pDS == NULL)
    {
        return;
    }

    ULONG cfgSpeakers;
    m_pDS->GetSpeakerConfig(&cfgSpeakers);

    DSCAPS dscaps; 
    dscaps.dwSize = sizeof(DSCAPS); 
    m_pDS->GetCaps(&dscaps); 

    if (dscaps.dwFlags & DSCAPS_EMULDRIVER)
    {
        return;
    }

    // Get the primary buffer 
    DSBUFFERDESC dsbd;
    memset(&dsbd, 0, sizeof(DSBUFFERDESC));

    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
    // Obtain write-primary cooperative level. 
    HRESULT hr = m_pDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

    if (hr != S_OK)
    {
        return;
    }

    if (FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &this->m_pDSBPrimary, NULL)))
    {
        return;
    }

    if (dwSpeakers == 6 && (cfgSpeakers == DSSPEAKER_5POINT1 || cfgSpeakers == DSSPEAKER_SURROUND))
    {
        WAVEFORMATEXTENSIBLE    waveFormatPCMEx;

        memset(&waveFormatPCMEx, 0, sizeof(WAVEFORMATEXTENSIBLE)); 

        waveFormatPCMEx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
        waveFormatPCMEx.Format.nChannels = 6;
        waveFormatPCMEx.Format.nSamplesPerSec = dwPrimaryFreq;
        waveFormatPCMEx.Format.wBitsPerSample  = (WORD) dwPrimaryBitRate; 
        waveFormatPCMEx.Format.nBlockAlign = waveFormatPCMEx.Format.wBitsPerSample / 8 * waveFormatPCMEx.Format.nChannels;
        waveFormatPCMEx.Format.nAvgBytesPerSec = waveFormatPCMEx.Format.nSamplesPerSec * waveFormatPCMEx.Format.nBlockAlign;
        waveFormatPCMEx.dwChannelMask = KSAUDIO_SPEAKER_5POINT1;
                                     // SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
                                     // SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY |
                                     // SPEAKER_BACK_LEFT  | SPEAKER_BACK_RIGHT
        waveFormatPCMEx.SubFormat =  KSDATAFORMAT_SUBTYPE_PCM;  // Specify PCM
        waveFormatPCMEx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE);
        waveFormatPCMEx.Samples.wValidBitsPerSample = 16;

        if (FAILED(hr = m_pDSBPrimary->SetFormat((WAVEFORMATEX*)&waveFormatPCMEx)))
        {
            return;
        }
        this->m_iNumSpeakers = 6;        // force it to think 5.1
        this->m_iBlockAlign = waveFormatPCMEx.Format.nBlockAlign;
    }
    else
    {

        if (dwSpeakers == 6) {
        }
        WAVEFORMATEX wfx;
        memset(&wfx, 0, sizeof(WAVEFORMATEX)); 

        wfx.wFormatTag      = WAVE_FORMAT_PCM; 
        wfx.nChannels       = 2; 
        wfx.nSamplesPerSec  = dwPrimaryFreq; 
        wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate; 
        wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
        wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
        wfx.cbSize = sizeof(WAVEFORMATEX);

        if (FAILED(hr = m_pDSBPrimary->SetFormat(&wfx)))
        {
            return;
        }

        this->m_iNumSpeakers = 2;        // force it to think stereo
        this->m_iBlockAlign = wfx.nBlockAlign;
    }

    this->m_BufferSize = MIXBUFFER_SAMPLES * sizeof(WORD) * this->m_iNumSpeakers * ROOM_SLICES_IN_BUFFER;

    InitializeSpeakers(
        this->m_BufferSize,
        dwPrimaryFreq,
        dwPrimaryBitRate,
        this->m_iNumSpeakers);
}

bool
CAudioHardwareWIN32::InitializeSpeakers(
    SIZE_T bufferSize,
    DWORD dwPrimaryFreq,
    DWORD dwPrimaryBitRate, 
    DWORD dwSpeakers
    )
/*
===============
idAudioHardwareWIN32::InitializeSpeakers
===============
*/
{
    if (dwSpeakers == 2)
    {
        WAVEFORMATEXTENSIBLE wfx;
        memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE)); 
        wfx.Format.wFormatTag      = WAVE_FORMAT_PCM; 
        wfx.Format.nChannels       = 2; 
        wfx.Format.nSamplesPerSec  = dwPrimaryFreq;
        wfx.Format.wBitsPerSample  = (WORD)dwPrimaryBitRate; 
        wfx.Format.nBlockAlign     = wfx.Format.wBitsPerSample / 8 * wfx.Format.nChannels;
        wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;
        wfx.Format.cbSize = sizeof(WAVEFORMATEX);

        CreateFromMemory(&m_pSpeakers, bufferSize, (waveformatextensible *)&wfx);

    }
    else
    {
        WAVEFORMATEXTENSIBLE    waveFormatPCMEx;
        memset(&waveFormatPCMEx, 0, sizeof(WAVEFORMATEXTENSIBLE)); 

        waveFormatPCMEx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
        waveFormatPCMEx.Format.nChannels = 6;
        waveFormatPCMEx.Format.nSamplesPerSec = dwPrimaryFreq;
        waveFormatPCMEx.Format.wBitsPerSample  = (WORD)dwPrimaryBitRate;
        waveFormatPCMEx.Format.nBlockAlign = waveFormatPCMEx.Format.wBitsPerSample / 8 * waveFormatPCMEx.Format.nChannels;
        waveFormatPCMEx.Format.nAvgBytesPerSec = waveFormatPCMEx.Format.nSamplesPerSec * waveFormatPCMEx.Format.nBlockAlign;
        waveFormatPCMEx.dwChannelMask = KSAUDIO_SPEAKER_5POINT1;
                                     // SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT |
                                     // SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY |
                                     // SPEAKER_BACK_LEFT  | SPEAKER_BACK_RIGHT
        waveFormatPCMEx.SubFormat =  KSDATAFORMAT_SUBTYPE_PCM;  // Specify PCM
        waveFormatPCMEx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE);
        waveFormatPCMEx.Samples.wValidBitsPerSample = 16;

        CreateFromMemory(&m_pSpeakers, bufferSize, (waveformatextensible *)&waveFormatPCMEx );

    }

    if (!m_pSpeakers)
    {
        return false;
    }

    m_pSpeakers->Play(0, DSBPLAY_LOOPING);

    return true;
}

int
CAudioHardwareWIN32::CreateFromMemory(
    IAudioBuffer** ppSound,
    SIZE_T  dataSize,
    waveformatextensible* pwfx,
    DWORD dwCreationFlags)
//-----------------------------------------------------------------------------
// Name: idAudioHardwareWIN32::CreateFromMemory()
// Desc: 
//-----------------------------------------------------------------------------
{
    HRESULT    hr = S_OK;
    LPDIRECTSOUNDBUFFER     apDSBuffer     = NULL;
    DWORD                   dwDSBufferSize = NULL;

    if (m_pDS == NULL)
        return -1;

    if (ppSound == NULL)
        return -1;

    IWaveSource *pWaveSource = new CMemoryWaveSource(pwfx);

    if (pWaveSource == 0)
    {
        return -1;
    }

    if (pWaveSource->Init((IDataStream *)dataSize) != TRUE)
    {
        goto __delete_source;
    }

    ISound  *pWaveFile = new CSoundWaveFile();

    if (pWaveFile == 0)
    {
        goto __free_source;
    }

    if (pWaveFile->Init(pWaveSource) != TRUE)
    {
        goto __delete_wav_file;
    }

    // Make the DirectSound buffer the same size as the wav file
    dwDSBufferSize = dataSize;

    // Create the direct sound buffer, and only request the flags needed
    // since each requires some overhead and limits if the buffer can 
    // be hardware accelerated
    DSBUFFERDESC dsbd;
    memset( &dsbd, 0, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = dwCreationFlags | DSBCAPS_GETCURRENTPOSITION2;
    dsbd.dwBufferBytes   = dwDSBufferSize;
    dsbd.guid3DAlgorithm = GUID_NULL;
    dsbd.lpwfxFormat     = (WAVEFORMATEX *)pwfx;

    if (FAILED(hr = this->m_pDS->CreateSoundBuffer(&dsbd, &apDSBuffer, NULL)))
    {
        goto __free_wave_file;
    }

    // Create the sound

    IAudioBuffer *pAudioBuffer = new CAudioBufferWIN32();

    if (pAudioBuffer == 0)
    {
        goto __release_ds_buffer;
    }

    if (pAudioBuffer->Init(apDSBuffer, dwDSBufferSize, pWaveFile) != TRUE)
    {
        goto __delete_audio_buffer;
    }

    *ppSound = pAudioBuffer;

    return S_OK;

__delete_audio_buffer:
    delete pAudioBuffer;

__release_ds_buffer:
    apDSBuffer->Release();

__free_wave_file:
    pWaveFile->Free();
    goto __end;       // break to end

__delete_wav_file:
    delete pWaveFile;

__free_source:
    pWaveSource->Free();

__delete_source:        
    delete pWaveSource;

__end:

    return -1;
}

int
CAudioHardwareWIN32::Create(
    IAudioBuffer** ppSound, 
    const char* strWaveFileName, 
    DWORD dwCreationFlags)
/*
===============
idAudioHardwareWIN32::Create
===============
*/
{
    UNREFERENCED_PARAMETER(ppSound); 
    UNREFERENCED_PARAMETER(strWaveFileName);
    UNREFERENCED_PARAMETER(dwCreationFlags);
#if 0

    int hr;
    LPDIRECTSOUNDBUFFER   apDSBuffer     = NULL;
    DWORD                 dwDSBufferSize = NULL;
    idWaveFile*          pWaveFile      = NULL;

    if (m_pDS == NULL)
        return -1;

    if (strWaveFileName == NULL || ppSound == NULL)
        return -1;

    pWaveFile = new idWaveFile();

    pWaveFile->Open(strWaveFileName, NULL);

    if (pWaveFile->GetOutputSize() == 0)
    {
        // Wave is blank, so don't create it.
        hr = E_FAIL;
        goto LFail;
    }

    // Make the DirectSound buffer the same size as the wav file
    dwDSBufferSize = pWaveFile->GetOutputSize();

    // Create the direct sound buffer, and only request the flags needed
    // since each requires some overhead and limits if the buffer can 
    // be hardware accelerated
    DSBUFFERDESC dsbd;
    memset(&dsbd, 0, sizeof(DSBUFFERDESC));
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = dwCreationFlags;
    dsbd.dwBufferBytes   = dwDSBufferSize;
    dsbd.guid3DAlgorithm = GUID_NULL;
    dsbd.lpwfxFormat     = (WAVEFORMATEX*)&pWaveFile->mpwfx;

    // DirectSound is only guarenteed to play PCM data.  Other
    // formats may or may not work depending the sound card driver.
    if (FAILED( hr = m_pDS->CreateSoundBuffer(&dsbd, &apDSBuffer, NULL)) )
        return -1;

    // Create the sound
    //*ppSound = new CAudioBufferWIN32(apDSBuffer, dwDSBufferSize, pWaveFile);
    
    pWaveFile->Close();

    return 0;

LFail:
    // Cleanup
    SAFE_DELETE( pWaveFile );
#endif // if 0

    return -1;
}

int
CAudioHardwareWIN32::Create(
    CWaveFile* pWaveFile,
    IAudioBuffer** ppiab)
/*
===============
idAudioHardwareWIN32::Create
===============
*/
{
    UNREFERENCED_PARAMETER(pWaveFile); 
    UNREFERENCED_PARAMETER(ppiab);

#if 0
    int hr;
    LPDIRECTSOUNDBUFFER   apDSBuffer     = NULL;
    DWORD                dwDSBufferSize = NULL;

    if (m_pDS == NULL)
        return -1;

    if (pWaveFile == NULL)
        return -1;

    *ppiab = NULL;

    if (pWaveFile->GetOutputSize() == 0 )
    {
        // Wave is blank, so don't create it.
        hr = E_FAIL;
        goto LFail;
    }

    // Make the DirectSound buffer the same size as the wav file
    dwDSBufferSize = pWaveFile->GetOutputSize();

    // Create the direct sound buffer, and only request the flags needed
    // since each requires some overhead and limits if the buffer can 
    // be hardware accelerated
    DSBUFFERDESC dsbd;
    memset( &dsbd, 0, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = 0;
    dsbd.dwBufferBytes   = dwDSBufferSize;
    dsbd.guid3DAlgorithm = GUID_NULL;
    dsbd.lpwfxFormat     = (WAVEFORMATEX*)&pWaveFile->mpwfx;

    // DirectSound is only guarenteed to play PCM data.  Other
    // formats may or may not work depending the sound card driver.
    if (FAILED( hr = m_pDS->CreateSoundBuffer(&dsbd, &apDSBuffer, NULL)))
        return -1;

    // Create the sound
    //*ppiab = new CAudioBufferWIN32(apDSBuffer, dwDSBufferSize, pWaveFile);
    
    return 0;

LFail:
    // Cleanup
    SAFE_DELETE( pWaveFile );
#endif // if 0

    return -1;
}


bool
CAudioHardwareWIN32::Lock(
    void **pDSLockedBuffer,
    ULONG *dwDSLockedBufferSize)
/*
===============
idAudioHardwareWIN32::Lock
===============
*/
{
    if (this->m_pSpeakers) {
        return this->m_pSpeakers->Lock(pDSLockedBuffer, dwDSLockedBufferSize);
    }

    return false;
}

bool
CAudioHardwareWIN32::Unlock(
    void *pDSLockedBuffer,
    DWORD dwDSLockedBufferSize)
/*
===============
idAudioHardwareWIN32::Unlock
===============
*/
{
    if (this->m_pSpeakers) {
        return this->m_pSpeakers->Unlock( pDSLockedBuffer, dwDSLockedBufferSize );
    }

    return false;
}

/*
===============
idAudioHardwareWIN32::GetCurrentPosition
===============
*/
bool
CAudioHardwareWIN32::GetCurrentPosition(ULONG *pdwCurrentWriteCursor)
{
    if (this->m_pSpeakers) {
        return this->m_pSpeakers->GetCurrentPosition(pdwCurrentWriteCursor);
    }
    return false;
}

BOOL
CAudioBufferWIN32::Init(
    LPDIRECTSOUNDBUFFER apDSBuffer,
    DWORD dwDSBufferSize,
    ISound* pWaveFile)
/*
===============
idAudioBufferWIN32::idAudioBuffer
===============
*/
{

    m_apDSBuffer = apDSBuffer;

    m_dwDSBufferSize = dwDSBufferSize;
    m_pBaseSound      = pWaveFile;

    if (pWaveFile)
    {
        FillBufferWithSound(m_apDSBuffer, false);

        m_apDSBuffer->SetCurrentPosition(0);
    }

    return TRUE;
}

VOID
CAudioBufferWIN32::Free()
/*
===============
idAudioBufferWIN32::iFree()
===============
*/
{
    m_pBaseSound->Free();
    delete m_pBaseSound;
    m_pBaseSound = 0;

    SAFE_RELEASE(m_apDSBuffer); 
    m_apDSBuffer = NULL;
}

int
CAudioBufferWIN32::FillBufferWithSound(
    LPDIRECTSOUNDBUFFER pDSB,
    bool bRepeatWavIfBufferLarger)
/*
===============
idAudioBufferWIN32::FillBufferWithSound
===============
*/
{
    int hr; 
    void*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    ULONG   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

    if (pDSB == NULL)
        return -1;

    // we may not even have a wavefile
    if (m_pBaseSound == NULL) {
        return -1;
    }


    // Make sure we have focus, and we didn't just switch in from
    // an app which had a DirectSound device
    if (FAILED(hr = RestoreBuffer(pDSB, NULL)))
    {
        return -1;
    }

    // Lock the buffer down
    if (FAILED(hr = pDSB->Lock(0, m_dwDSBufferSize, &pDSLockedBuffer, &dwDSLockedBufferSize, NULL, NULL, 0L)))
    {
        return -1;
    }

    // Reset the wave file to the beginning 
    m_pBaseSound->ResetFile();

    DWORD  dwReadedData = m_pBaseSound->Read((BYTE *) pDSLockedBuffer, dwDSLockedBufferSize, &dwWavDataRead);

    if (dwReadedData == 0)
    {
        return hr;
    }

    if (dwWavDataRead == 0)
    {
        // Wav is blank, so just fill with silence
        memset(pDSLockedBuffer, (byte)(m_pBaseSound->GetWaveFormat().Format.wBitsPerSample == 8 ? 128 : 0 ), dwDSLockedBufferSize );
    }
    else if (dwWavDataRead < (int)dwDSLockedBufferSize)
    {
        // If the wav file was smaller than the DirectSound buffer, 
        // we need to fill the remainder of the buffer with data 
        if (bRepeatWavIfBufferLarger)
        {
            // Reset the file and fill the buffer with wav data
            int dwReadSoFar = dwWavDataRead;    // From previous call above.
            while (dwReadSoFar < (int)dwDSLockedBufferSize)
            {  
                // This will keep reading in until the buffer is full 
                // for very short files
                if (FAILED(hr = m_pBaseSound->ResetFile()))
                {
                    return hr;
                }

                hr = m_pBaseSound->Read((BYTE *)pDSLockedBuffer + dwReadSoFar, dwDSLockedBufferSize - dwReadSoFar, &dwWavDataRead);

                if (FAILED(hr))
                {
                    return hr;
                }

                dwReadSoFar += dwWavDataRead;
            } 
        }
        else
        {
            // Don't repeat the wav file, just fill in silence 
            memset((BYTE *) pDSLockedBuffer + dwWavDataRead, 
                (BYTE)(m_pBaseSound->GetWaveFormat().Format.wBitsPerSample == 8 ? 128 : 0 ), dwDSLockedBufferSize - dwWavDataRead);
        }
    }

    // Unlock the buffer, we don't need it anymore.
    pDSB->Unlock(pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0);

    return S_OK;
}

int
CAudioBufferWIN32::RestoreBuffer(
    LPDIRECTSOUNDBUFFER pDSB,
    bool* pbWasRestored)
/*
===============
idAudioBufferWIN32::RestoreBuffer
Desc: Restores the lost buffer. *pbWasRestored returns true if the buffer was 
      restored.  It can also NULL if the information is not needed.
===============
*/
{
    int hr;

    if (pDSB == NULL)
    {
        return -1;
    }

    if (pbWasRestored)
    {
        *pbWasRestored = false;
    }

    ULONG dwStatus;

    if (FAILED( hr = pDSB->GetStatus(&dwStatus)))
    {
        return hr;
    }

    if (dwStatus & DSBSTATUS_BUFFERLOST)
    {
        // Since the app could have just been activated, then
        // DirectSound may not be giving us control yet, so 
        // the restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.
        do
        {
            hr = pDSB->Restore();
            if (hr == DSERR_BUFFERLOST)
            {
                Sleep(10);
            }
            hr = pDSB->Restore();

        } while(hr);

        if (pbWasRestored != NULL)
        {
            *pbWasRestored = true;
        }

        return S_OK;
    } else {
        return S_OK;
    }
}

int
CAudioBufferWIN32::Play(
    DWORD dwPriority, DWORD dwFlags)
/*
===============
idAudioBufferWIN32::Play
Desc: Plays the sound using voice management flags.  Pass in DSBPLAY_LOOPING
      in the dwFlags to loop the sound
===============
*/
{
    int hr;
    bool    bRestored;

    if (m_apDSBuffer == NULL)
    {
        return -1;
    }

    // Restore the buffer if it was lost
    if (FAILED( hr = RestoreBuffer(m_apDSBuffer, &bRestored)))
    {
    }

    if (bRestored)
    {
        // The buffer was restored, so we need to fill it with new data
        /*if (FAILED( hr = FillBufferWithSound(m_apDSBuffer, false)))
        {
        }*/

        // Make DirectSound do pre-processing on sound effects
        Reset();
    }

    m_apDSBuffer->Play(0, dwPriority, dwFlags);

    return 0;
}

int
CAudioBufferWIN32::Stop()
/*
===============
idAudioBufferWIN32::Stop
Desc: Stops the sound from playing
===============
*/
{
    if (this == NULL || m_apDSBuffer == NULL)
    {
        return -1;
    }

    m_apDSBuffer->Stop();

    return 0;
}

int
CAudioBufferWIN32::Reset()
/*
===============
idAudioBufferWIN32::Reset
Desc: Reset all of the sound buffers
===============
*/
{
    if( m_apDSBuffer == NULL ) {
        return -1;
    }

    m_apDSBuffer->SetCurrentPosition(0);

    return 0;
}

bool
CAudioBufferWIN32::IsSoundPlaying()
/*
===============
idAudioBufferWIN32::IsSoundPlaying
Desc: Checks to see if a buffer is playing and returns true if it
===============
*/
{
    if (m_apDSBuffer == NULL)
    {
        return false;
    }

    if (m_apDSBuffer)
    {
        ULONG dwStatus = 0;
        m_apDSBuffer->GetStatus( &dwStatus );
        if (dwStatus & DSBSTATUS_PLAYING)
        {
            return true;
        }
    }
    return false;
}

bool
CAudioBufferWIN32::Lock(
    void **pDSLockedBuffer,
    ULONG *dwDSLockedBufferSize)
/*
===============
idAudioBufferWIN32::Lock
===============
*/
{
    int hr;
    // Restore the buffer if it was lost
    bool bRestored;
    if (FAILED( hr = RestoreBuffer(m_apDSBuffer, &bRestored)))
    {
        return false;
    }

    // Lock the DirectSound buffer
    if (FAILED( hr = m_apDSBuffer->Lock(0, m_dwDSBufferSize, pDSLockedBuffer, dwDSLockedBufferSize, NULL, NULL, 0)))
    {
        return false;
    }
    return true;
}

bool
CAudioBufferWIN32::Unlock(
    void *pDSLockedBuffer,
    DWORD dwDSLockedBufferSize)
/*
===============
idAudioBufferWIN32::Unlock
===============
*/
{
    // Unlock the DirectSound buffer
    m_apDSBuffer->Unlock(pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0);
    return true;
}

bool
CAudioBufferWIN32::GetCurrentPosition(
    ULONG *pdwCurrentWriteCursor)
/*
===============
idAudioBufferWIN32::GetCurrentPosition
===============
*/
{
    int hr;

    // Make sure we have focus, and we didn't just switch in from
    // an app which had a DirectSound device
    if (FAILED(hr = RestoreBuffer(m_apDSBuffer, NULL)))
    {
        return false;
    }

    if (FAILED(hr = m_apDSBuffer->GetCurrentPosition(NULL, pdwCurrentWriteCursor)))
    {
        return false;
    }

    return true;
}

void
CAudioBufferWIN32::SetVolume(float x)
/*
===============
idAudioBufferWIN32::SetVolume
===============
*/
{
    if (m_apDSBuffer)
    {
        m_apDSBuffer->SetVolume((LONG)x);
    }
}
