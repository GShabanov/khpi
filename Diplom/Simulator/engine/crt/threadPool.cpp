
#include "stdafx.h"
#include "threadPool.h"

#define THREAD_POOL_THREADS  4

CThreadPool::CThreadPool()
{
    InitializeSListHead(&this->m_WorkerEventsHead);
}

BOOL
CThreadPool::Initialize()
{
    this->m_bShotdownPool = FALSE;
    this->m_hThreadChangeState = CreateSemaphore(NULL, 0, THREAD_POOL_THREADS, NULL);

    if (this->m_hThreadChangeState == NULL)
        return FALSE;


    this->m_threadRemoveLock = CRemoveLock();


    DWORD   dwThreadId = 0;
    for (int i = 0; i < THREAD_POOL_THREADS; i++)
    {
        this->m_hInvalidateThreads[i] = CreateThread(
            NULL,
            NULL,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(&CThreadPool::WorkerThread),
            this,
            NULL,
            &dwThreadId);

        if (this->m_hInvalidateThreads[i] == 0)
        {
            CloseHandle(this->m_hThreadChangeState);
            return FALSE;
        }
    }
    
    return TRUE;
}

VOID
CThreadPool::CleanUp()
{
    HANDLE  hThreadDispatchComplete = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (hThreadDispatchComplete == NULL)
        return;

    
    if (this->m_threadRemoveLock.Acquire() != TRUE)
    {
        return;
    }

    this->m_bShotdownPool = TRUE;

    ReleaseSemaphore(this->m_hThreadChangeState, 4, NULL);

    this->m_threadRemoveLock.ReleaseAndWait(hThreadDispatchComplete);

    for (int i = 0; i < THREAD_POOL_THREADS; i++)
    {
        WaitForSingleObject(this->m_hInvalidateThreads[i], INFINITE);

        CloseHandle(this->m_hInvalidateThreads[i]);
    }


    CloseHandle(hThreadDispatchComplete);

    CloseHandle(this->m_hThreadChangeState);

    //
    //  drop not dispatched events
    //
    PSLIST_ENTRY Entry;
    while ((Entry = InterlockedPopEntrySList(&this->m_WorkerEventsHead)) != 0)
    {
        PWORKER_EVENT workEvent = CONTAINING_RECORD(Entry, WORKER_EVENT, Entry);

        workEvent->lpResult = NULL;

        workEvent->removeLock.Release();

    }

}

CThreadPool::PWORKER_EVENT
CThreadPool::AddEvent(WORKER_METHOD method, PVOID parameter)
{
    //
    // reference thread
    //
    if (this->m_threadRemoveLock.Acquire() != TRUE)
        return NULL;

    WORKER_EVENT *_event = (WORKER_EVENT *)malloc(sizeof(WORKER_EVENT));

    if (_event == 0)
    {
        this->m_threadRemoveLock.Release();
        return NULL;
    }

    _event->lpContext = parameter;
    _event->lpResult = 0;
    _event->Method = method;

    _event->removeLock = CRemoveLock();

    //
    // Acquire until complete
    //
    _event->removeLock.Acquire();

    InterlockedPushEntrySList(&this->m_WorkerEventsHead, &_event->Entry);

    ReleaseSemaphore(this->m_hThreadChangeState, 1, NULL);

    this->m_threadRemoveLock.Release();

    return _event;
}

PVOID
CThreadPool::WaitForEvent(PWORKER_EVENT Event)
{
    HANDLE  hWaitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (hWaitEvent == NULL)
        return NULL;

    if (Event->removeLock.Acquire() != TRUE)
    {
        CloseHandle(hWaitEvent);
        return NULL;
    }

    Event->removeLock.ReleaseAndWait(hWaitEvent);

    PVOID result = Event->lpResult;

    CloseHandle(hWaitEvent);
    free(Event);

    return result;
}

DWORD WINAPI
CThreadPool::WorkerThread(CThreadPool *lpThis)
{
    while (!lpThis->m_bShotdownPool && lpThis->m_threadRemoveLock.Acquire())
    {
        WaitForSingleObject(lpThis->m_hThreadChangeState, INFINITE);

        DWORD   dwThreadId = GetCurrentThreadId();
        PSLIST_ENTRY Entry = 0;

        while ((Entry = InterlockedPopEntrySList(&lpThis->m_WorkerEventsHead)) != 0)
        {
            PWORKER_EVENT workEvent = CONTAINING_RECORD(Entry, WORKER_EVENT, Entry);

            workEvent->lpResult = (lpThis->*workEvent->Method)(workEvent->lpContext);

            workEvent->removeLock.Release();

        }

        lpThis->m_threadRemoveLock.Release();

    }
    return 0;
}

