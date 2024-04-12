/***************************************************************************************\
*   File:                                                                               *
*       object.h                                                                        *
*                                                                                       *
*   Abstract:                                                                           *
*       general abstract class for objects definition                                   *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Jun-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __OBJECT_H__
#define __OBJECT_H__


//
// overall base class
//
class IObject
{
protected:

    SIZE_T GetInstanceSize() { return sizeof(*this); };
    //
    // prevent instance creation
    //
    IObject()  noexcept
    {
    }

    virtual ~IObject() { };


public:

    virtual BOOL Init();
    virtual VOID Free();
};

//
// base class for garbage collected object types
//
class IDisposable : public IObject
{
protected:

    LONG              m_lRefCount;
    BOOL              m_bRemove;
    HANDLE            m_hRemoveEvent;


    //
    // prevent instance creation
    //
    IDisposable() noexcept
        : IObject()
        , m_lRefCount(1)
        , m_bRemove(FALSE)
        , m_hRemoveEvent(INVALID_HANDLE_VALUE)
    {
    }

public:
    virtual void* Reference();

    virtual BOOL Dereference();

    virtual void WaitDereference(HANDLE hWaitObject);

};

//
// object thats can be dispatched
//
class IGameObject : public IDisposable
{
protected:

    //
    // prevent instance creation
    //
    IGameObject() noexcept
        : IDisposable()
    {
    }

public:
    virtual void Dispatch(class CSharedEnvironment* pEnvironment);
    virtual void Draw(class ISurface* surface);
    virtual BOOL EmitSound(int iIndex);
    virtual BOOL Touch(LONG x, LONG y);
};

#endif // __OBJECT_H__