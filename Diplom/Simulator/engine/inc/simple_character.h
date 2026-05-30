/***************************************************************************************\
*   File:                                                                               *
*       simple_charecter.h                                                              *
*                                                                                       *
*   Abstract:                                                                           *
*       Simple character to test overall functionality over character interconnect      *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 02-May-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __SIMPLE_CHARACTER_H__
#define __SIMPLE_CHARACTER_H__

class CSimpleCharacter : public ICharacter
{

    Coord3D     m_Objects[5];
    ISprite    *m_pDotSprites[5];
    ISprite    *m_pTextureSprite2;
    ISprite3D  *m_pTextureSprite;

    BOOL    m_bMouseDown;
    DWORD   m_dwCurrentHeight;

public:
    CSimpleCharacter(class IScene *scene)
		: ICharacter(scene)
	{
	}

    virtual BOOL Init();
    virtual void Free();
    virtual void Draw(class ISurface* surface);
    virtual void Dispatch(class CSharedEnvironment* environment);
};

#endif // __SIMPLE_CHARACTER_H__
