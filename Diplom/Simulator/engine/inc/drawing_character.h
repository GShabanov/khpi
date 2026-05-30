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
#ifndef __DRAWING_CHARACTER_H__
#define __DRAWING_CHARACTER_H__


class CDrawingCharacter : public CCharacter
{

    class ISoundMap           *m_pSoundMap;
    class ISprite             *m_pSprite;


    LONG    m_X;
    LONG    m_Y;

    LONG    m_lCurrentBlending;
    LONG    m_lBlendingInc;
    BOOL    m_bMouseDown;

    DWORD   m_dwCurrentHeight;

public:
    CDrawingCharacter(class IScene *scene)
		: CCharacter(scene)
	{
	}

    virtual BOOL Init();
    virtual void Free();
    virtual void Draw(class ISurface* surface);
    virtual void Dispatch(class CSharedEnvironment* environment);
};

#endif // __DRAWING_CHARACTER_H__