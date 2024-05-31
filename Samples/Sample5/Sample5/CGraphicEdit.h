#pragma once
#ifndef __CGRAPHIC_EDIT_H__
#define __CGRAPHIC_EDIT_H__

#include "Sprite.h"

class CGraphicEdit : public CEdit
{
public:

private:

    //
    // 84 x 132    (one symbol)
    //
    CSprite       m_FontSprite;
    const CRect   m_spriteSymbol;
public:

    CGraphicEdit()
        : CEdit()
        , m_spriteSymbol(0, 0, 82, 131)
    {
    }

    virtual ~CGraphicEdit();

    BOOL Create(
        CSprite& pFontSprite,
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


#endif // __CGRAPHIC_EDIT_H__
