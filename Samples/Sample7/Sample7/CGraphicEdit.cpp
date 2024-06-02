/***************************************************************************************\
*   File:                                                                               *
*       CGraphicEdit.cpp                                                                *
*                                                                                       *
*   Abstract:                                                                           *
*       Component for custom drawing button                                             *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    30-May-2024                                                     *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"
#include "CGraphicEdit.h"
#include "ChildView.h"



BEGIN_MESSAGE_MAP(CGraphicEdit, CEdit)
    ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL
CGraphicEdit::Create(
    CSprite& pFontSprite,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID
    )
{

    this->m_FontSprite = pFontSprite;

    BOOL _return = __super::Create(dwStyle, rect, pParentWnd, nID);

    if (_return == 0)
    {
        return FALSE;
    }



    return TRUE;
}

CGraphicEdit::~CGraphicEdit()
{
    CEdit::~CEdit();

}

afx_msg
void
CGraphicEdit::OnPaint()
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
CGraphicEdit::Draw(DWORD* canvas, CRect& canvasSize, CRect& drawRect)
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

    CString  windowText;

    this->GetWindowText(windowText);

    int textLength = windowText.GetLength();

    if (textLength == 0)
        return;

    LONG oneSymbolLength = windowRect.Width() / textLength;

    CRect   symbolRect;

    symbolRect = windowRect;
    symbolRect.right = symbolRect.left + oneSymbolLength;

    int i = 0;

    while (textLength != 0)
    {

        CRect spriteRect = m_spriteSymbol;

        TCHAR  a = windowText[i];


        if (a >= _T('0') && a <= _T('9'))
        {
            int value = (unsigned char)(a - _T('0'));

            spriteRect.left = (m_spriteSymbol.Width() + 1) * value;
            spriteRect.right = spriteRect.left + m_spriteSymbol.Width();
        }
        else if (a == _T(':'))
        {
            spriteRect.left = (m_spriteSymbol.Width() + 1) * 11;
            spriteRect.right = spriteRect.left + m_spriteSymbol.Width();

        }
        else if (a == _T('/'))
        {
            spriteRect.left = (m_spriteSymbol.Width() + 1) * 11;
            spriteRect.right = spriteRect.left + m_spriteSymbol.Width();

        }
        else
        {
            symbolRect.left += oneSymbolLength;
            symbolRect.right += oneSymbolLength;

            textLength--;
            i++;
            continue;
        }


        m_FontSprite.DrawPartial(canvas, parentRect, symbolRect, spriteRect, FALSE);

        symbolRect.left += oneSymbolLength;
        symbolRect.right += oneSymbolLength;
        i++;
        textLength--;
    }

}
