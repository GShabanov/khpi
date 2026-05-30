/***************************************************************************************\
*   File:                                                                               *
*       logger.h                                                                        *
*                                                                                       *
*   Abstract:                                                                           *
*       logger implementation                                                           *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 25-Apr-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "gfx\inc\primitives.h"
#include "gfx\inc\vertex.h"

class ILogger : public IGameObject
{
protected:

    LONG            m_MaxX;
    LONG            m_MaxY;
protected:
    class IMouse            *m_Cursors[3];
    class IFont             *m_Font;

    class IFileSystem        *m_pFileSystem;

    class CSharedEnvironment *m_pSharedEnvironment;

public:
    ILogger(class CSharedEnvironment *pSharedEnvironment)
        : IGameObject()
        , m_pSharedEnvironment(pSharedEnvironment)
    {
    }

};

class CWindowLogger : public ILogger
{
private:

    BOOL InitializeFonts();
    VOID ShutdownFonts();

	void makeTexture(UCHAR **texture);

	BOOL				      m_bChangeAngle;
	UCHAR					 *m_texture;

    void drawCube(class ISurface *surface);
    void dispatchCube();
    Face              m_faces[12];
    Vertex            m_vertex[8];

protected:

    class ITexture          *m_TextureArray[10];
    class ISprite           *m_SpritesArray[10];

    BOOL InitializeTextures();
    BOOL InitializeSprites();

    VOID ShutdownTextures();
    VOID ShutdownSprites();

public:

	CWindowLogger(class CSharedEnvironment *environment)
		: ILogger(environment)
	{
    }

    virtual BOOL Init();
    virtual void Free();
    virtual void Draw(class ISurface* surface);
    virtual void Dispatch(class CSharedEnvironment* environment);
};

#endif // __LOGGER_H__