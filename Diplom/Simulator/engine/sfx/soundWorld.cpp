/***************************************************************************************\
*   File:                                                                               *
*       soundWorld.cpp                                                                  *
*                                                                                       *
*   Abstract:                                                                           *
*       sound world implementation                                                      *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 23-Feb-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 34 $
#include "stdafx.h"
#include <math.h>

#include "inc\soundWorld.h"
#include "inc\win_snd.h"

#include "inc\waveFormat.h"
#include "inc\sound.h"

BOOL
CSoundWorld::Init()
{
    this->m_fi = 0.0;

    InitializeCriticalSection(&this->m_SoundEmitLock);
    InitializeListHead(&this->m_SoundEmitHead);

    this->m_pSoundTemplate = malloc(MIXBUFFER_SAMPLES * 6 * sizeof(DWORD));

    if (this->m_pSoundTemplate == 0)
    {
        DeleteCriticalSection(&this->m_SoundEmitLock);
        return FALSE;
    }

    return TRUE;
}

void
CSoundWorld::ReleaseSound(ISound *pSound)
{
    RemoveEntryList(&pSound->m_ChainedEntry);

    pSound->Dereference();
    pSound->Free();
    delete pSound;
}

VOID
CSoundWorld::Free()
{

    EnterCriticalSection(&this->m_SoundEmitLock);
    while (!IsListEmpty(&this->m_SoundEmitHead))
    {
        register LIST_ENTRY *entry = RemoveHeadList(&this->m_SoundEmitHead);

        ReleaseSound(CONTAINING_RECORD(entry, ISound, m_ChainedEntry));
    }

    LeaveCriticalSection(&this->m_SoundEmitLock);

    free(this->m_pSoundTemplate);
    DeleteCriticalSection(&this->m_SoundEmitLock);    
}

VOID
CSoundWorld::MixLoop(unsigned int current44kHz, int numSpeakers, float *finalMixBuffer)
{
    UNREFERENCED_PARAMETER(current44kHz);

    EnterCriticalSection(&this->m_SoundEmitLock);

    LIST_ENTRY  *entry = this->m_SoundEmitHead.Flink;

    SIZE_T  sampleSize = sizeof(SHORT) * numSpeakers;
    DWORD   dwSamples = MIXBUFFER_SAMPLES;
    DWORD   dwReaded = 0;

    while (entry != &this->m_SoundEmitHead)
    {
        DWORD   dwToRead = dwSamples * sampleSize;

        memset(this->m_pSoundTemplate, 0, MIXBUFFER_SAMPLES * sampleSize);

        ISound *pSound = CONTAINING_RECORD(entry, ISound, m_ChainedEntry);

        if (
            (pSound->Read((BYTE *)this->m_pSoundTemplate, dwToRead, &dwReaded) == FALSE) || 
            (dwReaded < dwToRead)
            )
        {
            entry = entry->Flink;

            dwSamples = dwReaded / sampleSize;

            ReleaseSound(pSound);
        }
        else
        {
            entry = entry->Flink;
        }

        // TODO: make copy to buffer out from criticalSection
        float *ptr = finalMixBuffer;
        SHORT *sSpeakerSample = (SHORT *)this->m_pSoundTemplate;
        //BYTE *usSpeakerSample = (BYTE *)this->m_pSoundTemplate;

        for (DWORD i = 0; i < dwSamples; i++)
        {
            for (int j = 0; j < numSpeakers; j++)
            {
                *ptr++ = ((float)*sSpeakerSample);
                sSpeakerSample++;
            }
        }
    }

    LeaveCriticalSection(&this->m_SoundEmitLock);

    /*
    for (int i = 0; i < MIXBUFFER_SAMPLES; i++)
    {
        *(ptr + 0) = 0x7FFF * sin(this->m_fi);
        *(ptr + 1) = 0x7FFF * sin(this->m_fi);

        this->m_fi += 0.08F;

        if (this->m_fi >= 6.28)
        {
            this->m_fi = 0.0;
        }
        ptr = (float *)((unsigned char *)ptr + (sizeof(float) * numSpeakers));
    }
    */
}

BOOL
CSoundWorld::EmitSound(ISound *pSound)
{

    if (pSound->Reference() == 0)
    {
        return FALSE;
    }

    EnterCriticalSection(&this->m_SoundEmitLock);

    InsertTailList(&this->m_SoundEmitHead, &pSound->m_ChainedEntry);

    LeaveCriticalSection(&this->m_SoundEmitLock);

    return TRUE;
}