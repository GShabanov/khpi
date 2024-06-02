/***************************************************************************************\
*   File:                                                                               *
*       CGraphicButton.cpp                                                              *
*                                                                                       *
*   Abstract:                                                                           *
*       Component for custom drawing button                                             *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    11-May-2024                                                     *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"
#include "CGraphicButton.h"
#include "ChildView.h"



BEGIN_MESSAGE_MAP(CGraphicButton, CButton)
    ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL
CGraphicButton::Create(
    CSprite& pSpriteNormal,
    CSprite& pSpritePushed,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID
    )
{

    this->m_SpriteNormal = pSpriteNormal;
    this->m_SpritePushed = pSpritePushed;

    BOOL _return = __super::Create(_T(""), dwStyle, rect, pParentWnd, nID);

    if (_return == 0)
    {
        return FALSE;
    }



    return TRUE;
}

CGraphicButton::~CGraphicButton()
{
    CButton::~CButton();

}

afx_msg
void CGraphicButton::OnPaint()
{

    //
    // disable any painting
    //
    PAINTSTRUCT     paintStruct = { 0, };
    CDC* dc;

    dc = this->BeginPaint(&paintStruct);

    if (dc != NULL)
    {
        this->EndPaint(&paintStruct);
    }
}

void
CGraphicButton::Draw(DWORD* canvas, CRect& canvasSize, CRect& drawRect)
{
    CWnd* pParentWnd = this->GetParent();

    CRect    parentRect;

    pParentWnd->GetClientRect(&parentRect);

    CRect    windowRect;
    //
    // get position in screen coords
    //
    this->GetWindowRect(&windowRect);

    //
    // get position on parent
    //
    pParentWnd->ScreenToClient(windowRect);

    if (this->GetState() & (BST_PUSHED | BST_CHECKED))
    {
        m_SpritePushed.Draw(canvas, parentRect, windowRect, FALSE);
    }
    else
    {
        m_SpriteNormal.Draw(canvas, parentRect, windowRect, FALSE);
    }
}