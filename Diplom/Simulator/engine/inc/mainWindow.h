/***************************************************************************************\
*   File: 
*       mainWindow.h
*
*   Abstract:
*       Main user window predefines
*
*   Author: 
*       Gennady Shabanov (gennady.shabanov@gmail.com) 31-Oct-2008
*
*   Revision History:
\***************************************************************************************/
// $Revision: 132 $
#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__


class CMainWindow : public CDefaultWindow
{

public:

    static BOOL Create(
        IN HINSTANCE  hInstance,
		IN TCHAR *ptClassName,
		RECT     *lpRect,
        class CGameEngine *pGameEngine,
        OUT CMainWindow **ppNewInstance);

    VOID Destroy();


    VOID InvalidateClientArea();

private:
    CMainWindow(HINSTANCE hInstance, TCHAR *ptClassName);
    inline virtual ~CMainWindow();


    WNDPROC                   m_WindowSubclass;
    BOOL                      m_bMouseCaptured;

    class CGameEngine        *m_pGameEngine;

protected:

    static LRESULT
        WindowSubclass(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    virtual BOOL InitWindowInstance(IN LPRECT Rect);

    virtual LRESULT WindowProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

    virtual BOOL DestroyWindowInstance();

    virtual BOOL Do() { return FALSE; };
    virtual LRESULT WindowPreprocess(HWND /*hWnd*/, UINT /*message*/, WPARAM /*wParam*/, LPARAM /*lParam*/) { return LRESULT(-1); };


    VOID    ResizeClient(DWORD dwWidth, DWORD  dwHeight);

    BEGIN_MSG_HANDLERS()
    DEFINE_MSG_HANDLER(WM_COMMAND);
    DEFINE_MSG_HANDLER(WM_PAINT);
    DEFINE_MSG_HANDLER(WM_DESTROY);
    DEFINE_MSG_HANDLER(WM_SIZE);
    DEFINE_MSG_HANDLER(WM_MOUSEMOVE);
    DEFINE_MSG_HANDLER(WM_NCMOUSEMOVE);
    DEFINE_MSG_HANDLER(WM_LBUTTONDOWN);
    DEFINE_MSG_HANDLER(WM_LBUTTONUP);
    DEFINE_MSG_HANDLER(WM_USERCALLBACK);
    END_MSG_HANDLERS()
};


#endif //__MAIN_WINDOW_H__
