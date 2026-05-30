/***************************************************************************************\
*   File:                                                                               *
*       soundWorld.h                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       sound world interface                                                           *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 23-Feb-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 51 $
#ifndef __SOUND_WORLD_H__
#define __SOUND_WORLD_H__

class ISoundWorld : public IGameObject
{
protected:

public:
    ISoundWorld()
        : IGameObject()
    {
    }

    virtual void MixLoop(unsigned int current44kHz, int numSpeakers, float* finalMixBuffer) = 0;

    virtual BOOL EmitSound(class ISound* pSound) = 0;
};

class CSoundWorld : public ISoundWorld
{
private:

    float               m_fi;
    CRITICAL_SECTION    m_SoundEmitLock;
    LIST_ENTRY          m_SoundEmitHead;

    PVOID               m_pSoundTemplate;

    static  void ReleaseSound(class ISound *pSound);
public:
    CSoundWorld::CSoundWorld(void)
        : ISoundWorld()
        , m_fi(0.0)
        , m_pSoundTemplate(NULL)
    {
        InitializeListHead(&m_SoundEmitHead);

        memset(&m_SoundEmitLock, 0, sizeof(CRITICAL_SECTION));
    }

    ~CSoundWorld(void)
    {
    }

    virtual BOOL   Init();
    virtual VOID   Free();
    virtual VOID   MixLoop(unsigned int current44kHz, int numSpeakers, float* finalMixBuffer);
    virtual BOOL   EmitSound(class ISound* pSound);
};
#endif // __SOUND_WORLD_H__