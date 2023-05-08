
#include <windows.h>
#include <tchar.h>
#include <string>
#include <map>
#include <iostream>

#include "Cabinet.h"

using namespace std;


class CWebServer
{
    CCabinet* m_pCabinet;


    std::map<int, CMemento*>  m_users;      // мапа для збереження снепшотів

    typedef std::pair<int, CMemento*> mapPair;      // допоміжна пара для мапи

public:
    CWebServer(CCabinet* userCabinet)
    {
        m_pCabinet = userCabinet;
    }


    //
    // подія: користувач входе
    //

    void OnEnter(int userId)
    {

        //
        // на вході у сторінку беремо користувача по ID, та відновлюємо його стан
        //

        CMemento* memento  = m_users.at(userId);

        m_pCabinet->RestoreState(memento);
    }

    //
    // подія: користувач виходе
    //

    void OnExit()
    {

        //
        // на виході зі сторінки зберігаємо стан
        //

        m_users.insert(mapPair(m_pCabinet->Id(), m_pCabinet->SaveState()));
    }
};


int
_tmain(int argc, TCHAR* argv[])
{

    int userId = 0;

    CCabinet* pUserCabinet = new CCabinet(userId);


    CWebServer* pWebServer = new CWebServer(pUserCabinet);


    //
    // користувач вийшов зі сторінки (зберігаємо стан)
    //

    pWebServer->OnExit();


    //
    // користувач знову зайшов на сторішнку - відновлюємо стан згідно з його ID
    //
    pWebServer->OnEnter(userId);


    return 0;
}

