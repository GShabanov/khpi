/***************************************************************************************\
*   File:                                                                               *
*       drawEngine.h                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       draw engine for simple WINAPI                                                   *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 16-Jan-2013                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Id:  $
#ifndef __DRAW_ENGINE__
#define __DRAW_ENGINE__

class CDrawEngine
{
private:
    DECLSPEC_CACHEALIGN \
    CRITICAL_SECTION          m_EngineLock;

    HWND                      m_window;
    RECT                      m_Rect;

    class CSharedEnvironment *m_pSharedEnvironment;

    class ISurface           *m_CurrentSurface;
private:

    BOOL InitializeSurface();
    VOID ShutdownSurface();


protected:

    __declspec(noinline) CDrawEngine(HWND hWnd, class CSharedEnvironment *pSharedEnvironment)
        : m_window(hWnd)
    {
        this->m_pSharedEnvironment = pSharedEnvironment;
    }

public:
    static BOOL Create(IN CDrawEngine  *lpThis, class CSharedEnvironment *pShraredEnvironent, IN HWND hWnd);
    VOID Destroy();
    VOID DrawingProc(class IGameObject *pGameObject);

    VOID CopyToWnd(RECT *lpCopyRect);
};

#endif // __DRAW_ENGINE__
