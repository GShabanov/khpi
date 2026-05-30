/***************************************************************************************\
*   File:                                                                               *
*       charecter.h                                                                     *
*                                                                                       *
*   Abstract:                                                                           *
*       character overall dispatch function                                             *
*                                                                                       *
*   Author:                                                                             *
*       Gennady Shabanov (gennady.shabanov@gmail.com) 10-Jun-2014                       *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
// $Revision: 132 $
#ifndef __CHARACTER_H__
#define __CHARACTER_H__

class ICharacter : public IGameObject
{
protected:

    class IScene  *m_pScene;

	ICharacter(class IScene *scene)
        : IGameObject()
        , m_pScene(scene)
    {
    }
public:

};

class CCharacter : public ICharacter
{
private:
    __inline void iDispatch(class CSharedEnvironment *environment)
    {
        UNREFERENCED_PARAMETER(environment);
    }

    __inline void iDraw(class ISurface *surface)
    {
        UNREFERENCED_PARAMETER(surface);
    }

    __inline BOOL iEmitSound(int iIndex)
    {
        UNREFERENCED_PARAMETER(iIndex);
        return FALSE;
    }

    __inline BOOL iTouch(LONG x, LONG y)
    {
        UNREFERENCED_PARAMETER(x);
        UNREFERENCED_PARAMETER(y);
        return FALSE;
    }

protected:
    CCharacter(class IScene *scene)
        : ICharacter(scene)
    {
    }
};

#endif // __CHARACTER_H__