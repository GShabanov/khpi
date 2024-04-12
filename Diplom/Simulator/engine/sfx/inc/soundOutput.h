/***************************************************************************************\
*   File:                                                                               *
*       soundOutput.h                                                                   *
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
#ifndef __SOUNDOUTPUT_H__
#define __SOUNDOUTPUT_H__


class ISoundOuput : public IObject
{
protected:
    DEFINE_METHOD(BOOL, ISoundOuput, INIT, (HWND hWindow));
    DEFINE_METHOD(DWORD, ISoundOuput, GET_CURRENT_SAMPLE, ());
    DEFINE_METHOD(VOID, ISoundOuput, COPY_DATA, (DWORD offset, PVOID data, SIZE_T samples));

    INIT_PFN                    m_Init;
    GET_CURRENT_SAMPLE_PFN      m_GetCurrentSample;
    COPY_DATA_PFN               m_CopyData;

    SIZE_T          m_SoundBufferSize;
    SIZE_T          m_SampleSize;
    DWORD           m_dwSamples;
    WORD            m_wBitsPerSample;
    int             m_nChannels;
    int             m_nSamplesPerSec;
    int             m_b16Bit;

    ISoundOuput()
        : IObject()
        , m_SoundBufferSize(0)
        , m_SampleSize(0)
        , m_dwSamples(0)
        , m_wBitsPerSample(0)
        , m_nChannels(0)
        , m_nSamplesPerSec(0)
        , m_b16Bit(0)
    {
    }

public:
    __inline BOOL Init(HWND hWindow)
    {
        return (this->*m_Init)(hWindow);
    }

    //
    // copy media samples
    //
    __inline VOID CopyData(DWORD offset, PVOID data, SIZE_T samples)
    {
        return (this->*m_CopyData)(offset, data, samples);
    }

    __inline DWORD GetCurrentSample()
    {
        return (this->*m_GetCurrentSample)();
    }

    __inline DWORD GetSamplesCount() {
        return m_dwSamples;
    }

    __inline DWORD GetSampleSize() {
        return m_SampleSize;
    }

    __inline WORD GetBits() {
        return m_wBitsPerSample;
    }

    __inline int GetChanels() {
        return m_nChannels;
    }

    __inline int GetSPS() {
        return m_nSamplesPerSec;
    }
};

class CDirrectSound : public ISoundOuput
{
private:
    struct IDirectSound         *m_pDirrectSound;
    struct IDirectSoundBuffer   *m_pBuffer;

    BOOL iInit(HWND hWindow);
    void iFree();
    DWORD iGetCurrentSample();
    VOID iCopyData(DWORD offset, PVOID data, SIZE_T samples);

public:

    CDirrectSound()
        : ISoundOuput()
    {
        m_Init = (INIT_PFN)&CDirrectSound::iInit;
        m_Free = (FREE_PFN)&CDirrectSound::iFree;
        m_GetCurrentSample = (GET_CURRENT_SAMPLE_PFN)&CDirrectSound::iGetCurrentSample;
        m_CopyData = (COPY_DATA_PFN)&CDirrectSound::iCopyData;

        m_pDirrectSound = 0;
        m_pBuffer = 0;
    }

};

#endif // __SOUNDOUTPUT_H__