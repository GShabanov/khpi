
#include "stdafx.h"
#include "object.h"

BOOL
IObject::Init()
{
    return TRUE;
}

VOID
IObject::Free()
{
}



PVOID
IDisposable::Reference()
{
    InterlockedIncrement(&this->m_lRefCount);

    if (this->m_bRemove)
    {
        InterlockedDecrement(&this->m_lRefCount);
        return NULL;
    }
    return (PVOID)1;
}

BOOL
IDisposable::Dereference()
{
    if (InterlockedDecrement(&this->m_lRefCount) == 0)
    {
        delete this;
        return TRUE;
    }

    return FALSE;
}

VOID
IDisposable::WaitDereference(HANDLE hWaitObject)
{
    UNREFERENCED_PARAMETER(hWaitObject);
    InterlockedDecrement(&this->m_lRefCount);

}


void
IGameObject::Dispatch(CSharedEnvironment* pEnvironment)
{
    UNREFERENCED_PARAMETER(pEnvironment);
}

void
IGameObject::Draw(ISurface* surface)
{
    UNREFERENCED_PARAMETER(surface);
}

BOOL
IGameObject::EmitSound(int iIndex)
{
    UNREFERENCED_PARAMETER(iIndex);
    return TRUE;
}

BOOL
IGameObject::Touch(LONG x, LONG y)
{
    UNREFERENCED_PARAMETER(x);
    UNREFERENCED_PARAMETER(y);

    return TRUE;
}
