#pragma once
#ifndef __CGRAPHIC_BUTTON_H__
#define __CGRAPHIC_BUTTON_H__

#include "Sprite.h"

class CGraphicButton : public CButton
{
public:

private:

    CSprite       m_SpriteNormal;
    CSprite       m_SpritePushed;

public:

    CGraphicButton()
        : CButton()
    {
    }

    virtual ~CGraphicButton();

    BOOL Create(
        CSprite& pSpriteNormal,
        CSprite& pSpritePushed,
        DWORD dwStyle,
        const RECT& rect,
        CWnd* pParentWnd,
        UINT nID
    );

    BOOL LoadResource();

    void Draw(DWORD* canvas, CRect& canvasSize, CRect& drawRect);

protected:
    
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()

};


#endif // __CGRAPHIC_BUTTON_H__
