/***************************************************************************************\
*   File: 
*       logWindow.h
*
*   Abstract:
*       Main user window predefines
*
*   Author: 
*       Gennady Shabanov (gennady.shabanov@gmail.com) 31-Oct-2008
*
*   Revision History:
\***************************************************************************************/
// $Revision: 35 $
#ifndef __LOG_WINDOW_H__
#define __LOG_WINDOW_H__


class CLogWindow : public CDefaultWindow
{

public:

    static BOOL Create(
        IN HINSTANCE  hInstance,
		IN TCHAR *ptClassName,
		RECT     *lpRect,
        class CGameEngine *pGameEngine,
        OUT CLogWindow **ppNewInstance);

    VOID Destroy();

    __inline BOOL IsTerminating() {
        return m_bTermitating;
    }

	VOID InvalidateClientArea();

private:
    explicit CLogWindow(HINSTANCE hInstance, TCHAR *ptClassName);
    inline virtual ~CLogWindow();


    WNDPROC                   m_WindowSubclass;
    BOOL                      m_bMouseCaptured;
    BOOL                      m_bMouseClicked;
    BOOL                      m_bMouseClick;
    BOOL                      m_bTermitating;
    class CGameEngine        *m_pGameEngine;

protected:

    static LRESULT
        WindowSubclass(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    virtual BOOL InitWindowInstance(IN LPRECT Rect);

    virtual LRESULT WindowProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

    virtual BOOL DestroyWindowInstance();

    VOID    ResizeClient(DWORD dwWidth, DWORD  dwHeight);

    BEGIN_MSG_HANDLERS()
    DEFINE_MSG_HANDLER(WM_COMMAND);
    DEFINE_MSG_HANDLER(WM_PAINT);
    DEFINE_MSG_HANDLER(WM_DESTROY);
    DEFINE_MSG_HANDLER(WM_SIZE);
    DEFINE_MSG_HANDLER(WM_ACTIVATE);
    DEFINE_MSG_HANDLER(WM_MOUSEMOVE);
    DEFINE_MSG_HANDLER(WM_NCMOUSEMOVE);
    END_MSG_HANDLERS()
};


#endif //__LOG_WINDOW_H__
