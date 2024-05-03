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


public:


    CSprite();

    BOOL  IsLoaded() {

        return m_InitOk;
    }

    virtual BOOL Init(const CString& filename);
    virtual BOOL Init(const LPTSTR resource);
    virtual void Draw(DWORD* canvas, CRect& canvasDimensions, CRect& drawRect, BOOL  drawBackground);

    virtual ~CSprite();

};


#endif // __SPRITE_H__