/***************************************************************************************\
*   File:                                                                               *
*       eventPump.cpp                                                                   *
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
#include "stdafx.h"
#include "eventPump.h"

BOOL
CEventPump::Create(IN CEventPump  *pInstance)
{
    pInstance->CEventPump::CEventPump();


    InitializeSListHead(&pInstance->m_EventListHead);
    InitializeSListHead(&pInstance->m_CacheList);
	pInstance->m_Cached = 0;

    ::InitializeCriticalSection(&pInstance->m_EventListLock);

    pInstance->m_hNewEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    if (pInstance->m_hNewEvent == NULL) {
        ::DeleteCriticalSection(&pInstance->m_EventListLock);
        return FALSE;
    }

    pInstance->m_bDispatchEvents = TRUE;

    return TRUE;
}

VOID
CEventPump::Destroy()
{

    ::EnterCriticalSection(&this->m_EventListLock);

    this->m_bDispatchEvents = FALSE;

	EVENT_ENTRY  *entry = (EVENT_ENTRY *)InterlockedPopEntrySList(&this->m_EventListHead);

    // finalyze eventes dispatchments
    while (entry)
    {
        delete[] entry;

		entry = (EVENT_ENTRY *)InterlockedPopEntrySList(&this->m_EventListHead);
    }

	entry = (EVENT_ENTRY *)InterlockedPopEntrySList(&this->m_CacheList);

    // finalyze eventes dispatchments
    while (entry)
    {
        delete[] entry;

		entry = (EVENT_ENTRY *)InterlockedPopEntrySList(&this->m_CacheList);
    }

    ::LeaveCriticalSection(&this->m_EventListLock);

    ::CloseHandle(this->m_hNewEvent);
    ::DeleteCriticalSection(&this->m_EventListLock);

	this->CEventPump::~CEventPump();
}

BOOL
CEventPump::AddNewEvent(IN PVOID EventData, IN SIZE_T EventSize, IN LONG Timeout OPTIONAL)
{
    UNREFERENCED_PARAMETER(Timeout);

    if (EventSize > (MAX_EVENT - sizeof(LIST_ENTRY) - sizeof(SIZE_T)))
        return FALSE;

    ::EnterCriticalSection(&this->m_EventListLock);

	EVENT_ENTRY  *pNewEvent = (EVENT_ENTRY *)InterlockedPopEntrySList(&this->m_CacheList);

	if (pNewEvent == 0)
	{
        pNewEvent = reinterpret_cast<EVENT_ENTRY *>(
			new UCHAR [sizeof(EVENT_ENTRY)]);
	}
	else
	{
		this->m_Cached--;
	}

    if (pNewEvent == NULL)
	{
		::LeaveCriticalSection(&this->m_EventListLock);
        return FALSE;
	}


    if (this->m_bDispatchEvents == FALSE)
    {
        delete[] (UCHAR *)pNewEvent;
        ::LeaveCriticalSection(&this->m_EventListLock);
        return FALSE;
    }

    pNewEvent->eventSize = EventSize;
    memcpy(pNewEvent->eventData, EventData, EventSize);

	InterlockedPushEntrySList(&this->m_EventListHead, &pNewEvent->entry);

    ::LeaveCriticalSection(&this->m_EventListLock);

    ::SetEvent(this->m_hNewEvent);

    return TRUE;
}

BOOL
CEventPump::CheckEvent(PVOID EventData)
{
    register SLIST_ENTRY *Entry;

    ::EnterCriticalSection(&this->m_EventListLock);

	if (this->m_bDispatchEvents == FALSE || (m_EventListHead.Next.Next == 0))
    {
        ::LeaveCriticalSection(&this->m_EventListLock);
        return FALSE;
    }

    Entry = InterlockedPopEntrySList(&this->m_EventListHead);

 

    if (Entry != NULL)
    {

        EVENT_ENTRY  *pEventEntry = CONTAINING_RECORD(Entry, EVENT_ENTRY, entry);

        memcpy(EventData, pEventEntry->eventData, pEventEntry->eventSize);

		if (this->m_Cached < 10)
		{
			InterlockedPushEntrySList(&this->m_CacheList, &pEventEntry->entry);
			this->m_Cached++;
		}
		else
		{
			delete[] pEventEntry;
		}

		::LeaveCriticalSection(&this->m_EventListLock);
        return TRUE;
    }

   ::LeaveCriticalSection(&this->m_EventListLock);

    return FALSE;
}

BOOL
CEventPump::MsgWaitEvent(PVOID EventData)
{

    for (;;)
    {
        if (this->CheckEvent(EventData) == TRUE)
            return TRUE;

        //
        // wait for message or new event
        //
        DWORD result = ::MsgWaitForMultipleObjects(
            1, &this->m_hNewEvent, FALSE, this->m_ulTimeout, QS_ALLINPUT);

        //
        // if wait succeded on event
        //
        if (result != WAIT_OBJECT_0)
        {
            return FALSE;
        }

    }
}

BOOL
CEventPump::WaitEvent(PVOID EventData)
{

    for (;;)
    {
        if (this->CheckEvent(EventData) == TRUE)
            return TRUE;

        //
        // wait for message or new event
        //
        DWORD result = ::WaitForMultipleObjects(
            1, &this->m_hNewEvent, FALSE, this->m_ulTimeout);

        //
        // if wait succeded on event
        //
        if (result != WAIT_OBJECT_0)
        {
            return FALSE;
        }

    }
}