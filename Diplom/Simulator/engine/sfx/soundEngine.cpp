/***************************************************************************************\
*   File:                                                                               *
*       soundEngine.cpp                                                                 *
*                                                                                       *
*   Abstract:                                                                           *
*       output sound main engine                                                        *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 36 $
#include "stdafx.h"
#include "inc\win_snd.h"

#include "inc\soundEngine.h"
#include "inc\waveFormat.h"
#include "inc\sound.h"
#include "inc\soundworld.h"

#include <math.h>

#include "environment.h"


BOOL
CSoundEngine::Create(IN CSoundEngine  *lpThis, class CSharedEnvironment *pShraredEnvironent, IN HWND hWnd)
{
	lpThis->CSoundEngine::CSoundEngine(hWnd, pShraredEnvironent);

    BOOL _return = lpThis->InitializeSoundOuput();

    if (_return == FALSE)
    {
        goto __FREE_CLASS;
    }

	return TRUE;


__FREE_CLASS:
    lpThis->CSoundEngine::~CSoundEngine();

    return FALSE;
}

VOID
CSoundEngine::Destroy()
{
	this->ShutdownSoundOuput();

	this->CSoundEngine::~CSoundEngine();

}

/*
============
CSoundEngine::MixedSoundToSamples
============
*/
void
CSoundEngine::MixedSoundToSamples(short *samples, const float *mixBuffer, const int numSamples )
{

    for (int i = 0; i < numSamples; i++)
    {
        if (mixBuffer[i] <= -32768.0f )
        {
            samples[i] = -32768;
        }
        else if (mixBuffer[i] >= 32767.0f)
        {
            samples[i] = 32767;
        }
        else
        {
            samples[i] = (short) mixBuffer[i];
        }
    }
}

VOID
CSoundEngine::SoundOutput(ISoundWorld *pSoundWorld)
{
    LARGE_INTEGER   perfFrequency;  // 1second = 1000milliseconds = 1000*1000microseconds
    QueryPerformanceFrequency(&perfFrequency);

    //
    // инициализируем стартовое число тиков системы
    //
    LARGE_INTEGER   perfCountBefore;
    QueryPerformanceCounter(&perfCountBefore);

    ULONG dwCurrentWritePos;
    DWORD dwCurrentBlock;

    // and here in bytes
    // get the current byte position in the buffer where the sound hardware is currently reading
    if ( !this->m_pSoundHW->GetCurrentPosition(&dwCurrentWritePos))
    {
        return;
    }
    // mixBufferSize is in bytes
    dwCurrentBlock = dwCurrentWritePos / this->m_pSoundHW->GetMixBufferSize();


    if (this->m_nextWriteBlock == 0xffffffff ) {
        this->m_nextWriteBlock = dwCurrentBlock;
    }

    if (dwCurrentBlock != this->m_nextWriteBlock)
    {
        return;
    }

    // lock the buffer so we can actually write to it
    short *fBlock = NULL;
    ULONG fBlockLen = 0;

    this->m_pSoundHW->Lock((void **)&fBlock, &fBlockLen);

    if (!fBlock)
    {
        return;
    }

    int numSpeakers = this->m_pSoundHW->GetNumberOfSpeakers();

    this->m_nextWriteBlock++;
    this->m_nextWriteBlock %= ROOM_SLICES_IN_BUFFER;

    if (this->m_nextWriteBlock < this->m_prevWriteBlock)
    {
        m_Buffers++;                  // buffer wrapped
    }

    this->m_prevWriteBlock = this->m_nextWriteBlock;

    // nextWriteSample is in multi-channel samples inside the buffer
    unsigned int nextWriteSamples = this->m_nextWriteBlock * MIXBUFFER_SAMPLES;

    // newSoundTime is in multi-channel samples since the sound system was started
    unsigned int newSoundTime = this->m_Buffers * (MIXBUFFER_SAMPLES * ROOM_SLICES_IN_BUFFER) + nextWriteSamples;
    
    // check for impending overflow
    // FIXME: we don't handle sound wrap-around correctly yet
    if (newSoundTime > 0x6fffffff)
    {
        m_Buffers = 0;
    }

    if ((newSoundTime - this->m_CurrentSoundTime) > MIXBUFFER_SAMPLES ) {
    }

    // clear the buffer for all the mixing output
    memset(m_finalMixBuffer, 0, MIXBUFFER_SAMPLES * sizeof(float) * numSpeakers);

    // let the active sound world mix all the channels in unless muted or avi demo recording
    pSoundWorld->MixLoop(newSoundTime, numSpeakers, this->m_finalMixBuffer);
    /*if ( !muted && currentSoundWorld && !currentSoundWorld->fpa[0] ) {
        currentSoundWorld->MixLoop( newSoundTime, numSpeakers, finalMixBuffer );
    }*/

    short *dest = fBlock + nextWriteSamples * numSpeakers;

    this->MixedSoundToSamples(dest, this->m_finalMixBuffer, MIXBUFFER_SAMPLES * numSpeakers);

    // allow swapping the left / right speaker channels for people with miswired systems
    if (numSpeakers == 2 && this->m_pSharedEnvironent->m_sfxInfo.n_bReverse)
    {
        for (int j = 0; j < MIXBUFFER_SAMPLES; j++ )
        {
            register DWORD temp = *((DWORD *)dest + j);
            *((DWORD *)dest + j) = _lrotl(temp, 16);
        }
    }

    this->m_pSoundHW->Unlock(fBlock, fBlockLen);

    this->m_CurrentSoundTime = newSoundTime;

    //
    // получаем дельту тиков c момента сохранения 
    //
    LARGE_INTEGER   perfCountDelta;
    QueryPerformanceCounter(&perfCountDelta);

    perfCountDelta.QuadPart = perfCountDelta.QuadPart > perfCountBefore.QuadPart ? \
        perfCountDelta.QuadPart - perfCountBefore.QuadPart : \
        perfCountBefore.QuadPart - perfCountDelta.QuadPart;

	this->m_SndPerfomance.QuadPart = perfCountDelta.QuadPart / (perfFrequency.QuadPart / 1000000);
}

BOOL
CSoundEngine::InitializeSoundOuput()
{
    IAudioHardware    *pSoundHW = 0;
    pSoundHW = new CAudioHardwareWIN32();

    this->m_nextWriteBlock = 0xFFFFFFFF;
    this->m_prevWriteBlock = 0;
    this->m_Buffers = 0;
    this->m_CurrentSoundTime = 0;
    // make a 16 byte aligned finalMixBuffer
    this->m_finalMixBuffer = (float *)((((int)this->m_realAccum) + 15) & ~15);

    if (pSoundHW == 0)
        return FALSE;

    if (pSoundHW->Init(this->m_window) != TRUE)
    {
        delete pSoundHW;
        return FALSE;
    }

    this->m_pSoundHW = pSoundHW;
    
	return TRUE;

}

VOID
CSoundEngine::ShutdownSoundOuput()
{
    this->m_pSoundHW->Free();

    delete this->m_pSoundHW;

    this->m_pSoundHW = 0;
}