
#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

class CThreadPool
{
    HANDLE              m_hInvalidateThreads[10];
    HANDLE              m_hThreadChangeState;

    CRemoveLock         m_threadRemoveLock;
    BOOL                m_bShotdownPool;
    SLIST_HEADER        m_WorkerEventsHead;

protected:
    typedef PVOID  (CThreadPool::*WORKER_METHOD)(PVOID Context);
    typedef struct      _WORKER_EVENT
    {
        SLIST_ENTRY     Entry;
        PVOID           lpContext;
        PVOID           lpResult;
        WORKER_METHOD   Method;
        CRemoveLock     removeLock;
    } WORKER_EVENT, *PWORKER_EVENT;

protected:
    PWORKER_EVENT  AddEvent(WORKER_METHOD method, PVOID parameter);
    PVOID WaitForEvent(PWORKER_EVENT Event);

public:
    CThreadPool();

    BOOL Initialize();
    VOID CleanUp();


private:

    static DWORD WINAPI WorkerThread(CThreadPool *lpParam);
};

#endif // __THREAD_POOL_H__