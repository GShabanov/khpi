/***************************************************************************************\
*   File:                                                                               *
*       gameEngine.cpp                                                                  *
*                                                                                       *
*   Abstract:                                                                           *
*       main entrypoint to game engine                                                  *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 3-Feb-2014                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#include "stdafx.h"
#include "resource.h"

#include "defaultWindow.h"
#include "mainWindow.h"
#include "logWindow.h"

#include "utils\inc\utilites.h"

#include "sfx\inc\win_snd.h"
#include "sfx\inc\waveFormat.h"
#include "sfx\inc\sound.h"
#include "sfx\inc\soundWorld.h"
#include "sfx\inc\soundEngine.h"
#include "gfx\inc\font.h"

#include "gameEngine.h"

#include "gfx\inc\drawEngine.h"


#include "environment.h"
#include "scene.h"
#include "logger.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

static SYSTEMTIME lpSystemTimeBefore;
static ULONG counter = 0;

static ULONG counter2 = 0;
static ULONG soundCntr = 0;

void
CGameEngine::OnIdle()
/*++

   Routine:
        OnIdle()

   Description:
        Override this member function to perform idle-time processing.

   Arguments:

   Return:
        void
--*/
{
    counter++;

    if (counter >= 100)
    {
        SYSTEMTIME lpSystemTime;

        GetSystemTime(&lpSystemTime);

        DWORD dwDeltaSeconds = (lpSystemTimeBefore.wSecond > lpSystemTime.wSecond) ? 
            (lpSystemTimeBefore.wSecond - lpSystemTime.wSecond) :
            (lpSystemTime.wSecond - lpSystemTimeBefore.wSecond);

        DWORD dwDeltaMSeconds = (lpSystemTimeBefore.wMilliseconds > lpSystemTime.wMilliseconds) ? 
            (lpSystemTimeBefore.wMilliseconds - lpSystemTime.wMilliseconds) :
            (lpSystemTime.wMilliseconds - lpSystemTimeBefore.wMilliseconds);

        this->m_pSharedEnvironment->m_gfxInfo.m_dwPerfomanceMsec = dwDeltaMSeconds;
        this->m_pSharedEnvironment->m_gfxInfo.m_dwPerfomanceSec = dwDeltaSeconds;

        memcpy(&lpSystemTimeBefore, &lpSystemTime, sizeof(lpSystemTime));

        counter = 0;
    }

    if (this->m_pSharedEnvironment->LockShared() != TRUE)
    {
        return;
    }

    IScene *pScene = this->m_pSharedEnvironment->GetCurrentScene();

    if (pScene != 0)
    {
        pScene->Dispatch(this->m_pSharedEnvironment);
    }

    if (pScene != 0)
    {
        counter2++;

        if (counter2 >= 2)
        {
            counter2 = 0;

            LARGE_INTEGER   drawPerfomanceBefore;
            QueryPerformanceCounter(&drawPerfomanceBefore);

            this->m_pDrawEngine->DrawingProc(pScene);

            //
            // получаем дельту тиков c момента сохранения 
            //
            LARGE_INTEGER   drawPerfomanceAfter;
            QueryPerformanceCounter(&drawPerfomanceAfter);

            drawPerfomanceAfter.QuadPart = drawPerfomanceAfter.QuadPart > drawPerfomanceBefore.QuadPart ? \
                drawPerfomanceAfter.QuadPart - drawPerfomanceBefore.QuadPart : \
                drawPerfomanceBefore.QuadPart - drawPerfomanceAfter.QuadPart;

            this->m_pSharedEnvironment->m_gfxInfo.m_DrawPerfomance.QuadPart = drawPerfomanceAfter.QuadPart / 
                (this->m_hDispatchFrequency / 1000000);


            this->m_pMainWindow->InvalidateClientArea();


        }
    }

    if (pScene != 0)
    {
        soundCntr++;

        if (soundCntr >= 4)
        {
            this->m_pSoundEngine->SoundOutput(pScene->GetSoundWorld());
            soundCntr = 0;   
        }
    }

    this->m_pSharedEnvironment->Update();

    //
    // output to log window
    //
    if (this->m_pSharedEnvironment->IsLogsActive()) 
    {
        this->m_pLogger->Dispatch(this->m_pSharedEnvironment);
        this->m_pDrawEngineLogs->DrawingProc(this->m_pLogger);

        this->m_pLogWindow->InvalidateClientArea();
    }

    this->m_pSharedEnvironment->Unlock();
}

BOOL
CGameEngine::ProcessMessage()
{
    //
    // this is a message, so dispatch it
    //
    MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {

        if (msg.message == WM_QUIT)
        {
            this->m_bTerminating = TRUE;
        }

        /*if (msg.message != WM_PAINT)
        {
            TCHAR   message[100];
            _stprintf(message, _T("Message: 0x%X\n"), msg.message);
            OutputDebugString(message);
        }*/

        // Otherwise, dispatch the message.
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        return TRUE;

    }   // End of PeekMessage while loop.

	return FALSE;

}

LONG WINAPI
CGameEngine::VectoredHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
    UNREFERENCED_PARAMETER(ExceptionInfo);

    __debugbreak();

    return EXCEPTION_CONTINUE_SEARCH;
}



BOOL
CGameEngine::InitInstance(HINSTANCE hInstance)
{
    /*this->m_pVectoredExceptionHandler = AddVectoredExceptionHandler(
        0, (PVECTORED_EXCEPTION_HANDLER)&CGameEngine::VectoredHandler);*/

    this->m_bTerminating = FALSE;
    this->m_hInstance = hInstance;
    //
    // Initialize global strings
    //
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_PLOTSOMETHING, szWindowClass, MAX_LOADSTRING);

    //
    // получаем колличество тиков системы за одну секунду
    //
    LARGE_INTEGER   perfFrequency;  // 1second = 1000milliseconds = 1000*1000microseconds
    QueryPerformanceFrequency(&perfFrequency);
    this->m_hDispatchFrequency = perfFrequency.QuadPart; /* 10 milliseconds */
    GetSystemTime(&lpSystemTimeBefore);


	RECT  rect;
	rect.top = 40;
	rect.left = 40;
    rect.right = rect.left + 1024; //640;
    rect.bottom = rect.top + 768; // 480;

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, FALSE);


    this->m_pSharedEnvironment = reinterpret_cast<CSharedEnvironment *>(new UCHAR[sizeof(CSharedEnvironment)]);

    if (this->m_pSharedEnvironment == 0)
    {
        return FALSE;
    }

    this->m_pSharedEnvironment->CSharedEnvironment::CSharedEnvironment();

    this->m_pSharedEnvironment->m_gfxInfo.SetRect(&rect);

    if (this->m_pSharedEnvironment->Initialize() != TRUE)
    {
        goto __destroy_environment;
    }

    this->m_hDispatchEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    if (this->m_hDispatchEvent == NULL)
    {
        goto __finalize_environment;
    }

    if (CDefaultWindow::RegisterWindowClass(szWindowClass, hInstance) == NULL)
    {
        goto __close_dispatch_event;
    }



    if (this->InitMainWindow(szWindowClass, &rect) != TRUE)
    {
        goto __unregister_class;
    }

    if (this->m_pSharedEnvironment->IsLogsActive())
    {
        RECT   rect2;

        LONG delta = rect.right - rect.left;

        rect2 = rect;

        rect2.left = rect.right;
        rect2.right = rect2.left + delta;

        if (this->InitLogWindow(szWindowClass, &rect2) != TRUE)
        {
            goto __unregister_class;
        }

    }

    if (this->m_pSharedEnvironment->LoadScene(1) != TRUE)
    {
        goto __unregister_class;
    }

    return TRUE;


__unregister_class:
	::UnregisterClass(szWindowClass, hInstance);

__close_dispatch_event:
    CloseHandle(this->m_hDispatchEvent);

__finalize_environment:
    this->m_pSharedEnvironment->Finalize();

__destroy_environment:
    this->m_pSharedEnvironment->CSharedEnvironment::~CSharedEnvironment();
    delete[] (UCHAR *)this->m_pSharedEnvironment;

    return FALSE;
}


BOOL
CGameEngine::MainLoop()
{
    //
    // инициализируем стартовое число тиков системы
    //
    LARGE_INTEGER   perfCountBefore;
    QueryPerformanceCounter(&perfCountBefore);

    do
    {

	    DWORD result = ::MsgWaitForMultipleObjects(
            1, &this->m_hDispatchEvent, FALSE, 10, QS_ALLINPUT);

        if (result > WAIT_OBJECT_0)
        {

            while (this->ProcessMessage())
            {

                //
                // получаем дельту тиков c момента сохранения 
                //
                LARGE_INTEGER   perfCountDelta;
                QueryPerformanceCounter(&perfCountDelta);

                perfCountDelta.QuadPart = perfCountDelta.QuadPart > perfCountBefore.QuadPart ? \
                    perfCountDelta.QuadPart - perfCountBefore.QuadPart : \
                    perfCountBefore.QuadPart - perfCountDelta.QuadPart;

                if (perfCountDelta.QuadPart >= (this->m_hDispatchFrequency / 10000))
                {
                    //
                    // выполняем полезную работу раз в 10 миллисекунд
                    //
                    this->OnIdle();

                    QueryPerformanceCounter(&perfCountBefore);
                }

                if (this->m_bTerminating == TRUE)
                {
                    break;
                }

            }

        }

        if (result == WAIT_TIMEOUT)
        {
            this->OnIdle();

            QueryPerformanceCounter(&perfCountBefore);
        }


    }
    while (this->m_bTerminating != TRUE);

    return TRUE;
}

VOID
CGameEngine::ExitInstance()
{
    PostQuitMessage(0);

    CloseHandle(this->m_hDispatchEvent);

    if (this->m_pSharedEnvironment->IsLogsActive()) 
    {
        FreeLogWindow();
    }

    FreeMainWindow();

    this->m_pSharedEnvironment->Finalize();
    this->m_pSharedEnvironment->CSharedEnvironment::~CSharedEnvironment();
    delete[] this->m_pSharedEnvironment;

    ::UnregisterClass(szWindowClass, this->m_hInstance);

}

BOOL
CGameEngine::InitLogWindow(TCHAR *wndClass, RECT *rect)
{

    this->m_pLogger = new CWindowLogger(this->m_pSharedEnvironment);

    if (this->m_pLogger == NULL)
    {
        return FALSE;
    }

    if (this->m_pLogger->Init() != TRUE)
    {
        goto __destroy_logger;
    }

    if (CLogWindow::Create(
        this->m_hInstance, wndClass, rect, this, &this->m_pLogWindow) != TRUE)
    {
        return FALSE;
    }

    ShowWindow(this->m_pLogWindow->GetWindow(), SW_HIDE);

    //
    // initialize GFX
    //
    this->m_pDrawEngineLogs = reinterpret_cast<CDrawEngine *>(
        new UCHAR[sizeof(CDrawEngine)]);

    if (this->m_pDrawEngineLogs == NULL)
    {
        goto __destroy_window;
    }

    if (CDrawEngine::Create(
        this->m_pDrawEngineLogs,
        this->m_pSharedEnvironment,
        this->m_pLogWindow->GetWindow()) != TRUE)
    {
        goto __clean_GFX;
    }

    return TRUE;


__clean_GFX:
    delete this->m_pDrawEngineLogs;

__destroy_window:
    this->m_pLogWindow->Destroy();

__destroy_logger:
    delete this->m_pLogger;

    return FALSE;
}

VOID
CGameEngine::FreeLogWindow()
{
    this->m_pDrawEngineLogs->Destroy();
    this->m_pLogWindow->Destroy();

    delete this->m_pDrawEngineLogs;

    this->m_pLogger->Free();
    delete this->m_pLogger;
}

BOOL
CGameEngine::InitMainWindow(TCHAR *wndClass, RECT *rect)
{
    if (CMainWindow::Create(
        this->m_hInstance, wndClass, rect, this, &this->m_pMainWindow) != TRUE)
    {
        return FALSE;
    }

    //
    // initialize GFX
    //
    this->m_pDrawEngine = reinterpret_cast<CDrawEngine *>(
        malloc(sizeof(CDrawEngine)));

    if (this->m_pDrawEngine == NULL)
    {
        goto __destroy_window;
    }

    if (CDrawEngine::Create(
        this->m_pDrawEngine,
        this->m_pSharedEnvironment,
        this->m_pMainWindow->GetWindow()) != TRUE)
    {
        goto __clean_GFX;
    }

    //
    // initialize SFX
    //
    this->m_pSoundEngine = reinterpret_cast<CSoundEngine *>(new UCHAR[sizeof(CSoundEngine)]);

    if (this->m_pSoundEngine == NULL)
    {
        goto __destroy_GFX;
    }

    if (CSoundEngine::Create(
        this->m_pSoundEngine,
        this->m_pSharedEnvironment,
        this->m_pMainWindow->GetWindow()) != TRUE)
    {
        goto __clean_SFX;
    }

    return TRUE;

__clean_SFX:
    delete this->m_pSoundEngine;

__destroy_GFX:
    this->m_pDrawEngine->Destroy();

__clean_GFX:
    delete this->m_pDrawEngine;

__destroy_window:
    this->m_pMainWindow->Destroy();

    return FALSE;
}

VOID
CGameEngine::FreeMainWindow()
{
    this->m_pSoundEngine->Destroy();
    this->m_pDrawEngine->Destroy();
    this->m_pMainWindow->Destroy();

    delete this->m_pSoundEngine;
    delete this->m_pDrawEngine;
}


int APIENTRY
_tWinMain(
          HINSTANCE hInstance,
          HINSTANCE hPrevInstance,
          LPTSTR    lpCmdLine,
          int       nCmdShow)
{
    UNREFERENCED_PARAMETER(nCmdShow);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    CGameEngine *pGameEngine = new CGameEngine();

    if (pGameEngine == NULL)
        return -1;

    if (pGameEngine->InitInstance(hInstance) != TRUE)
    {
        delete pGameEngine;
        return -1;
    }

    pGameEngine->MainLoop();

    pGameEngine->ExitInstance();

    delete pGameEngine;

    return (int)0;
}

