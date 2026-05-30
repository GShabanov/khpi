/***************************************************************************************\
*   File:                                                                               *
*       soundEngine.h                                                                   *
*                                                                                       *
*   Abstract:                                                                           *
*       sound access method and interfaces                                              *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 36 $
#ifndef __SOUND_ENGINE_H__
#define __SOUND_ENGINE_H__

class CSoundEngine
{
private:
    DECLSPEC_CACHEALIGN \
    CRITICAL_SECTION          m_EngineLock;

	HWND                      m_window;
	LARGE_INTEGER             m_SndPerfomance;

    int                       m_Buffers;                // statistics
    unsigned int              m_CurrentSoundTime;       // set by the async thread and only used by the main thread

    unsigned int              m_prevWriteBlock;
    unsigned int              m_nextWriteBlock;
    float                     m_realAccum[6*MIXBUFFER_SAMPLES+16];
    float *                   m_finalMixBuffer;         // points inside realAccum at a 16 byte aligned boundary

    class IAudioHardware     *m_pSoundHW;
    class CSharedEnvironment *m_pSharedEnvironent;
private:

	BOOL InitializeSoundOuput();
	VOID ShutdownSoundOuput();


    void MixedSoundToSamples(short *samples, const float *mixBuffer, const int numSamples);

protected:

    CSoundEngine(HWND hWnd, class CSharedEnvironment *pSharedEnvironent)
		: m_window(hWnd)
    {
        this->m_pSoundHW = 0;
        this->m_pSharedEnvironent = pSharedEnvironent;
    }

public:
    static BOOL Create(IN CSoundEngine  *lpThis, class CSharedEnvironment *pShraredEnvironent, IN HWND hWnd);
    VOID Destroy();
    VOID SoundOutput(class ISoundWorld *pSoundWorld);
};

#endif // __SOUND_ENGINE_H__