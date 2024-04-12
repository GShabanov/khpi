/***************************************************************************************\
*   File:                                                                               *
*       EventPump.h                                                                     *
*                                                                                       *
*   Abstract:                                                                           *
*       dispatch events from different sources                                          *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 16-Jan-2008                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __EVENT_PUMP__
#define __EVENT_PUMP__

#define MAX_EVENT  64

class CEventPump
{
private:
    typedef struct _EVENT_ENTRY {
        SLIST_ENTRY   entry;
        SIZE_T          eventSize;
        UCHAR           eventData[MAX_EVENT - sizeof(SLIST_ENTRY) - sizeof(SIZE_T)];
    } EVENT_ENTRY, *PEVENT_ENTRY;

private:
	DECLSPEC_CACHEALIGN \
    CRITICAL_SECTION         m_EventListLock;
    SLIST_HEADER            m_EventListHead;
    SLIST_HEADER            m_CacheList;
	SIZE_T                   m_Cached;
    BOOL                     m_bDispatchEvents;
    HANDLE                   m_hNewEvent;
    ULONG                    m_ulTimeout;

public:
    inline CEventPump() 
        : m_ulTimeout(INFINITE)
    {
    }

    static BOOL Create(IN CEventPump  *pInstance);
    VOID Destroy();

    BOOL AddNewEvent(IN PVOID EventData, IN SIZE_T EventSize, IN LONG Timeout = INFINITE OPTIONAL);
    BOOL MsgWaitEvent(PVOID EventData);
    BOOL CheckEvent(PVOID EventData);

    BOOL WaitEvent(PVOID EventData);
};
#endif // __EVENT_PUMP__