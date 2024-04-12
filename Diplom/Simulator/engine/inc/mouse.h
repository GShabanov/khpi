/***************************************************************************************\
*   File:                                                                               *
*       mouse.h                                                                         *
*                                                                                       *
*   Abstract:                                                                           *
*       mouse dispatch and cursors                                                      *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Jun-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __MOUSE_H__
#define __MOUSE_H__

class IMouse : public IDisposable
{
protected:

    LONG    m_X;
    LONG    m_Y;

    class  IScene   *m_pScene;

	IMouse(class IScene *scene)
        : IDisposable()
        , m_pScene(scene)
    {
        m_X = 0;
        m_Y = 0;
    }
public:

    virtual BOOL Init(class ITexture* texture) = 0;
    virtual void Draw(class ISurface* surface) = 0;
    virtual  void Dispatch(class CSharedEnvironment* environment) = 0;
};

class CMouse : public IMouse
{

    class ITexture   *m_pTexture;
    LONG              m_sizeX;
    LONG              m_sizeY;

    LONG     m_lCurrentFrame;
    LONG     m_lFrameIncrement;
    ULONG    m_lCurrentTicks;
    BOOL     m_bAnimate;

public:
	CMouse(class IScene *scene)
		: IMouse(scene)
	{

        m_sizeX = 0;
        m_sizeY = 0;

        m_lCurrentFrame = 0;
        m_lFrameIncrement = 1;
        m_lCurrentTicks = 0;

        m_bAnimate = FALSE;
	}

    virtual BOOL Init(class ITexture* texture);
    virtual void Free();
    virtual void Draw(class ISurface* surface);
    virtual void Dispatch(class CSharedEnvironment* environment);
};

#endif // __MOUSE_H__