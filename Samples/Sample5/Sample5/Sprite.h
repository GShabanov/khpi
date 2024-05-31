#pragma once
#ifndef __SPRITE_H__
#define __SPRITE_H__

class CSprite
{
protected:

    HBITMAP     m_Bitmap;
    PVOID       m_BitmapData;

    LONG        m_bmHeight;
    LONG        m_bmWidth;

    BOOL        m_InitOk;

private:
    BOOL LoadImageFile(const CString& filename);
    BOOL LoadFromResources(LPCTSTR resource, LPCTSTR type);

    PVOID   GetBitmapData();

public:

    void GetSize(CRect& size) const {

        CRect _return(0, 0, m_bmWidth, m_bmHeight);

        size = _return;
    }

    CSprite();
    CSprite& operator=(const CSprite&);

    BOOL  IsLoaded() {

        return m_InitOk;
    }

    virtual BOOL Init(const CString& filename);
    virtual BOOL Init(LPCTSTR resource, LPCTSTR type);
    virtual void Draw(DWORD* canvas, CRect& canvasDimensions, CRect& drawRect, BOOL  drawBackground);
    virtual void DrawPartial(DWORD* canvas, CRect& canvasDimensions, CRect& drawRect, CRect& spriteRect, BOOL  drawBackground);

    virtual ~CSprite();

private:
    CSprite(CSprite& other) { }

};


#endif // __SPRITE_H__