/***************************************************************************************\
*   File:                                                                               *
*       scene.h                                                                         *
*                                                                                       *
*   Abstract:                                                                           *
*       current played scene interface                                                  *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __SCENE_H__
#define __SCENE_H__

class IScene : public IGameObject
{
protected:

    LONG            m_MaxX;
    LONG            m_MaxY;
    LONG            m_MinX;
    LONG            m_MinY;

protected:
    class IMouse            *m_Cursors[3];
    class ITexture          *m_TextureArray[20];
    class ISprite           *m_SpritesArray[20];
    class IMoveMap          *m_MapsArray[10];
    class ICharacter        *m_CharacterArray[10];
    class IWaveSource       *m_Sounds[10];
    class IFont             *m_Fonts[10];

    class ISoundWorld        *m_pSoundWorld;
    class IFileSystem        *m_pFileSystem;

    class CSharedEnvironment  *m_pSharedEnvironment;

public:
    IScene(class CSharedEnvironment *pSharedEnvironment)
        : IGameObject()
        , m_pSharedEnvironment(pSharedEnvironment)
        , m_pSoundWorld(0)
        , m_pFileSystem(0)
    {
    }

    virtual ~IScene()
    {
        IGameObject::~IGameObject();
    }

    //
    // accessors
    //
    LONG GetMaxX() { return this->m_MaxX; };
    LONG GetMaxY() { return this->m_MaxY; };
    LONG GetMinX() { return this->m_MinX; };
    LONG GetMinY() { return this->m_MinY; };

    class IMouse  *GetCurrentMouse()
    {
        return m_Cursors[0];
    }

    class ITexture *GetTexture(int index);
    class ISprite *GetSprite(int i);

    class IMoveMap *GetHeightMap()
    {
        return m_MapsArray[0];
    }

    class IMoveMap *GetMoveMap()
    {
        return m_MapsArray[1];
    }

    class IMoveMap *GetActiveZoneMap()
    {
        return m_MapsArray[2];
    }

    class ISoundMap *GetSoundMap()
    {
        return (class ISoundMap *)m_MapsArray[2];
    }

    class IWaveSource *GetSound(int i)
    {
        IWaveSource *pWave = m_Sounds[i];

        if (pWave->Reference() == 0)
            return 0;

        return pWave;
    }

    class IFont *GetFont(int i)
    {
        IFont *pFont = m_Fonts[i];

        if (pFont->Reference() == 0)
            return 0;

        return pFont;
    }

    virtual BOOL EmitSound(int iIndex) = 0;

    class ICharacter **GetCharacters()
    {
        return m_CharacterArray;
    }

    class ISoundWorld *GetSoundWorld()
    {
        return m_pSoundWorld;
    }

};

class CGhostScene : public IScene
{
private:

protected:
    BOOL InitializeCursors();
    BOOL InitializeMaps();
    BOOL InitializeSprites();
    BOOL InitializeTextures();
    BOOL InitializeCharacters();
    BOOL InitializeSounds();
    BOOL InitializeFonts();

    VOID ShutdownFonts();
    VOID ShutdownSounds();
    VOID ShutdownCharacters();
    VOID ShutdownTextures();
    VOID ShutdownSprites();
    VOID ShutdownMaps();
    VOID ShutdownCursors();

public:

	CGhostScene(class CSharedEnvironment *environment)
		: IScene(environment)
	{
    }

    virtual BOOL Init();
    virtual void Free();
    virtual void Dispatch(class CSharedEnvironment* environment);
    virtual void Draw(class ISurface* surface);
    virtual BOOL EmitSound(int iIndex);
};

#endif // __SCENE_H__