/***************************************************************************************\
*   File:                                                                               *
*       removeLock.h                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       reference count work routines                                                   *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 1-Feb-2015                        *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#ifndef __REMOVE_LOCK_H__
#define __REMOVE_LOCK_H__

class __declspec(novtable) \
CRemoveLock
{
private:
    BOOL                   m_bRemove;
    BOOL                   m_bReserved[3];
    LONG                   m_RefCount;
    HANDLE                 m_evRemove;

public:
    inline explicit CRemoveLock()
    {
        //
        // device remove lock
        //
        this->m_RefCount = 1;
        this->m_bRemove = FALSE;
        this->m_evRemove = NULL; //CreateEvent(NULL, FALSE, FALSE, NULL);

    }

    inline ~CRemoveLock() {
        //CloseHandle(this->m_evRemove);
    }

    BOOL Acquire()
    {
        InterlockedIncrement(&this->m_RefCount);

        if (this->m_bRemove)
        {
            if (InterlockedDecrement(&this->m_RefCount) == 0) {
                SetEvent(this->m_evRemove);
            }

            return FALSE;
        }

        return TRUE;
    }

    void Release()
    {
        LONG usage = InterlockedDecrement(&this->m_RefCount);

        if (usage == 0) {

            SetEvent(this->m_evRemove);

        }
    }

    void ReleaseAndWait(HANDLE hRemoveEvent, DWORD timeout = INFINITE)
    {
        //
        // release and wait remove lock
        //
        HANDLE hRemove = CreateEvent(NULL, FALSE, FALSE, NULL);
        this->m_evRemove = hRemove;
        this->m_bRemove = TRUE;

        InterlockedDecrement(&this->m_RefCount);

        //
        // wait for completion
        //
        if (InterlockedDecrement(&this->m_RefCount) > 0)
        {
            WaitForSingleObject(this->m_evRemove, timeout);
        }

        this->m_evRemove = NULL;
        CloseHandle(hRemove);
    }

};

#endif // __REMOVE_LOCK_H__