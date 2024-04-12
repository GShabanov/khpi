/***************************************************************************************\
*   File: 
*       DefaultWindow.h
*
*   Abstract:
*       Base interface for windows classes implementation
*
*   Author: 
*       Gennady Shabanov (gennady.shabanov@gmail.com) 31-Oct-2008
*
*   Revision History:
\***************************************************************************************/
// $Revision: 132 $
#ifndef __DEFAULT_WINDOW_H__
#define __DEFAULT_WINDOW_H__

#include <Uxtheme.h>
#include <vssym32.h>

#define RECTWIDTH(Rect) (Rect.right - Rect.left)
#define RECTHEIGHT(Rect) (Rect.bottom - Rect.top)

#define BEGIN_MSG_HANDLERS()
#define END_MSG_HANDLERS()

#define MSG_FUNCTION(message) \
    HANDLER_##message

#define DEFINE_MSG_HANDLER(message) \
    LRESULT HANDLER_##message##(WPARAM wParam, LPARAM lParam)

#define MSG_HANDLER_FUNCTION(_class, message) \
    LRESULT _class::HANDLER_##message##(WPARAM wParam, LPARAM lParam)

#define HANDLE_MSG(message)    \
    case (message): return HANDLER_##message((wParam), (lParam))

class CDefaultWindow
{
protected:

    HINSTANCE           m_hInstance;
    TCHAR              *m_ptClassName;
    HWND                m_hwndWnd;
    HWND                m_hwndParent;
    //RECT                m_MainWndDimensions;
    TCHAR              *m_ptWindowTitle;
    HCURSOR             m_hkCurrentCursor;
    DWORD               m_dwThreadId;
    static HMODULE      m_hmUxTheme;
	static LONG         m_lUxThemeRef;
    HWND                m_hwndLostFocus;

    typedef BOOL     (__stdcall *_IsAppThemed)(VOID);
    typedef HTHEME   (__stdcall *_OpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
    typedef HRESULT  (__stdcall *_GetThemeInt)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int *piVal);
    typedef HRESULT  (__stdcall *_GetThemeMargins)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, LPRECT prc, MARGINS *pMargins);
    typedef HRESULT  (__stdcall *_GetThemePosition)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, POINT *pPoint);

    typedef HRESULT  (__stdcall *_CloseThemeData)(HTHEME hTheme);
    typedef int      (__stdcall *_GetThemeSysSize)(HTHEME hTheme, int iSizeID);
    typedef COLORREF (__stdcall *_GetThemeSysColor)(HTHEME hTheme, int iColorID);
    typedef HRESULT  (__stdcall *_GetThemeSysFont)(HTHEME hTheme, int iFontID, LOGFONT *plf);
    typedef HRESULT  (__stdcall *_GetThemeSysInt)(HTHEME hTheme, int iIntID, int *piValue);

    typedef HRESULT (__stdcall *_DrawThemeBackground)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
    typedef HRESULT (__stdcall *_DrawThemeText)(
        HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT *pRect);

    typedef HRESULT (__stdcall *_GetThemePartSize)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, RECT *prc, THEMESIZE eSize, SIZE *psz);
    typedef HRESULT (__stdcall *_GetThemeRect)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, RECT *pRect);
    typedef int (__stdcall *_GetThemeSysSize)(HTHEME hTheme, int iSizeID);

    _IsAppThemed         m_IsAppThemed;
    _OpenThemeData       m_OpenThemeData;
    _GetThemeInt         m_GetThemeInt;
    _GetThemePartSize    m_GetThemePartSize;
    _GetThemeMargins     m_GetThemeMargins;
    _GetThemePosition    m_GetThemePosition;
    _GetThemeSysColor    m_GetThemeSysColor;
    _GetThemeSysSize     m_GetThemeSysSize;
    _GetThemeSysFont     m_GetThemeSysFont;
    _GetThemeSysInt      m_GetThemeSysInt;
    _GetThemeRect        m_GetThemeRect;
    _CloseThemeData      m_CloseThemeData;
    _DrawThemeBackground m_DrawThemeBackground;
    _DrawThemeText       m_DrawThemeText;

public:

    RECT            m_InflatingRect;

    virtual BOOL    Do() = 0;

    BOOL IsAppThemed(VOID);

    __forceinline HTHEME OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
    {
        if (m_OpenThemeData == NULL)
            return NULL;

        return this->m_OpenThemeData(hwnd, pszClassList);
    }

    __forceinline HRESULT GetThemeInt(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int *piVal)
    {
        if (m_GetThemeInt == NULL)
            return NULL;

        return this->m_GetThemeInt(hTheme, iPartId, iStateId, iPropId, piVal);
    }

    __forceinline HRESULT GetThemeMargins(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, LPRECT prc, MARGINS *pMargins)
    {
        if (m_GetThemeMargins == NULL)
            return NULL;

        return this->m_GetThemeMargins(hTheme, hdc, iPartId, iStateId, iPropId, prc, pMargins);
    }

    __forceinline HRESULT GetThemePosition(HTHEME hTheme, int iPartId, int iStateId, int iPropId, POINT *pPoint)
    {
        if (m_GetThemePosition == NULL)
            return NULL;

        return this->m_GetThemePosition(hTheme, iPartId, iStateId, iPropId, pPoint);
    }

    __forceinline HRESULT GetThemePartSize(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, RECT *prc, THEMESIZE eSize, SIZE *psz)
    {
        if (m_GetThemePartSize == NULL)
            return NULL;

        return this->m_GetThemePartSize(hTheme, hdc, iPartId, iStateId, prc, eSize, psz);
    }

    __forceinline int GetThemeSysSize(HTHEME hTheme, int iSizeID)
    {
        if (m_GetThemeSysSize == NULL)
            return NULL;

        return this->m_GetThemeSysSize(hTheme, iSizeID);
    }

    __forceinline COLORREF GetThemeSysColor(HTHEME hTheme, int iColorID)
    {
        if (m_GetThemeSysColor == NULL)
            return NULL;

        return this->m_GetThemeSysColor(hTheme, iColorID);
    }

    __forceinline HRESULT GetThemeSysFont(HTHEME hTheme, int iFontID, LOGFONT *plf)
    {
        if (m_GetThemeSysFont == NULL)
            return NULL;

        return this->m_GetThemeSysFont(hTheme, iFontID, plf);
    }

    __forceinline HRESULT GetThemeSysInt(HTHEME hTheme, int iIntID, int *piValue)
    {
        if (m_GetThemeSysInt == NULL)
            return NULL;

        return this->m_GetThemeSysInt(hTheme, iIntID, piValue);
    }


    __forceinline HRESULT GetThemeRect(HTHEME hTheme, int iPartId, int iStateId, int iPropId, RECT *pRect)
    {
        if (m_GetThemeRect == NULL)
            return NULL;

        return this->m_GetThemeRect(hTheme, iPartId, iStateId, iPropId, pRect);
    }

    __forceinline HRESULT CloseThemeData(HTHEME hTheme)
    {
        if (m_CloseThemeData == NULL)
            return NULL;

        return this->m_CloseThemeData(hTheme);
    }

    __forceinline HRESULT DrawThemeBackground(
        HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
    {
        if (m_DrawThemeBackground == NULL)
            return NULL;

        return this->m_DrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
    }

    __forceinline HRESULT DrawThemeText(
        HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT *pRect)
    {
        if (m_DrawThemeText == NULL)
            return NULL;

        return this->m_DrawThemeText(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect);
    }

    __forceinline const DWORD  GetWindowThreadId()
    {
        return this->m_dwThreadId;
    }

    __forceinline const HWND GetWindow()
    {
        return this->m_hwndWnd;
    }

    __forceinline CDefaultWindow* GetParentClass(CDefaultWindow* pClass)
    {
        if (pClass->m_hwndParent != NULL)
        {
            return reinterpret_cast<CDefaultWindow*>((LONG_PTR)
                GetWindowLong(pClass->m_hwndParent, GWLP_USERDATA));
        }
        else
        {
            return NULL;
        }

    }

    __forceinline HCURSOR SetCursor(const HCURSOR hCursor)
    {
        register HCURSOR hkPrevios = this->m_hkCurrentCursor;
        this->m_hkCurrentCursor = hCursor;
        return hkPrevios;
    }

    BOOL GetClientRect(LPRECT lpRect);

    static BOOL IsWindowAccessible(HWND hWnd);

    static HWND   GetNextTabbedWindow(HWND hWnd);
    static HWND   GetPreviosTabbedWindow(HWND hWnd);
    static ATOM   RegisterWindowClass(IN TCHAR *ptClassName, IN HINSTANCE hInstance);

protected:

    explicit CDefaultWindow(HINSTANCE hInstance, IN HWND hwndParent, TCHAR  *ptClassName);

    virtual ~CDefaultWindow();

    virtual BOOL InitWindowInstance(IN HWND hwndParent, IN LPRECT Rect, DWORD dwStyle);
    virtual BOOL DestroyWindowInstance();

    virtual LRESULT
        WM_THEMECHANGE_Dispatch();

    virtual LRESULT
        WindowProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

    virtual LRESULT
        WindowPreprocess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

    static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK 
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

#endif // __DEFAULT_WINDOW_H__
