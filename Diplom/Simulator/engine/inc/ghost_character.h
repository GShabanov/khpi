/***************************************************************************************\
*   File:                                                                               *
*       ghost_charecter.h                                                               *
*                                                                                       *
*   Abstract:                                                                           *
*       Ghost character dispatch function                                               *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Jun-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __GHOST_CHARACTER_H__
#define __GHOST_CHARACTER_H__


class CGhostCharacter : public CCharacter
{

    class ISoundMap           *m_pSoundMap;
    class ISprite             *m_pSprite;

    SLIST_HEADER              m_TrackingHead;

    typedef struct _TRACK_ENTRY {
        SLIST_ENTRY    entry;
        int            recursion;
        int            state;
        POINT          point;
    } TRACK_ENTRY, *PTRACK_ENTRY;

    LONG    m_X;
    LONG    m_Y;

    LONG    m_lCurrentBlending;
    LONG    m_lBlendingInc;
    BOOL    m_bMouseDown;

    DWORD   m_dwCurrentHeight;

    void MakeTrackedPath();

    BOOL CheckVisit(LONG x, LONG y);
public:
	CGhostCharacter(class IScene *scene)
		: CCharacter(scene)
	{
	}

    virtual BOOL Init();
    virtual void Free();
    virtual void Draw(class ISurface* surface);
    virtual void Dispatch(class CSharedEnvironment* environment);
};

#endif // __GHOST_CHARACTER_H__