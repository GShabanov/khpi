#pragma once



class  CGameEngine
{
private:
    HINSTANCE   m_hInstance;
    HANDLE      m_hDispatchEvent;
    LONGLONG    m_hDispatchFrequency;

    PVOID       m_pVectoredExceptionHandler;
    BOOL        m_bTerminating;
private:
    class CSharedEnvironment *m_pSharedEnvironment;
    class CMainWindow        *m_pMainWindow;
    class CLogWindow         *m_pLogWindow;

    class CDrawEngine        *m_pDrawEngine;
    class CDrawEngine        *m_pDrawEngineLogs;
    class CSoundEngine       *m_pSoundEngine;
    class ILogger            *m_pLogger;

    void OnIdle();
    BOOL ProcessMessage();
public:
    CGameEngine()
        : m_pSharedEnvironment(0)
        , m_pMainWindow(0)
        , m_pLogWindow(0)
        , m_pDrawEngine(0)
        , m_pSoundEngine(0)
    {
    }

    __inline class CSharedEnvironment *GetEnvironment() const 
    {
        return m_pSharedEnvironment;
    }

    __inline class CDrawEngine *GetDrawEngine() const
    {
        return m_pDrawEngine;
    }

    __inline class CDrawEngine *GetLogsDraw() const
    {
        return m_pDrawEngineLogs;
    }

    __inline class CSoundEngine *GetSoundEngine() const
    {
        return m_pSoundEngine;
    }

    __inline class CDefaultWindow *GetMainWindow() const
    {
        return (CDefaultWindow *)m_pMainWindow;
    }

    BOOL InitMainWindow(TCHAR *wndClass, RECT *rect);
    VOID FreeMainWindow();

    BOOL InitLogWindow(TCHAR *wndClass, RECT *rect);
    VOID FreeLogWindow();

    BOOL InitInstance(HINSTANCE hInstance);
    BOOL MainLoop();
    VOID ExitInstance();

    static LONG WINAPI VectoredHandler(PEXCEPTION_POINTERS ExceptionInfo);

};