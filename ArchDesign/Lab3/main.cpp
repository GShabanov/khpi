
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


    std::map<int, CMemento*>  m_users;      // ���� ��� ���������� ��������

    typedef std::pair<int, CMemento*> mapPair;      // �������� ���� ��� ����

public:
    CWebServer(CCabinet* userCabinet)
    {
        m_pCabinet = userCabinet;
    }


    //
    // ����: ���������� �����
    //

    void OnEnter(int userId)
    {

        //
        // �� ���� � ������� ������ ����������� �� ID, �� ���������� ���� ����
        //

        CMemento* memento  = m_users.at(userId);

        m_pCabinet->RestoreState(memento);
    }

    //
    // ����: ���������� ������
    //

    void OnExit()
    {

        //
        // �� ����� � ������� �������� ����
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
    // ���������� ������ � ������� (�������� ����)
    //

    pWebServer->OnExit();


    //
    // ���������� ����� ������ �� �������� - ���������� ���� ����� � ���� ID
    //
    pWebServer->OnEnter(userId);


    return 0;
}

