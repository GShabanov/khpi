
#include <windows.h>
#include <tchar.h>
#include <string>
#include <list>
#include <iostream>

#include "singleton.h"
#include "database.h"



int
_tmain(int argc, TCHAR* argv[])
{

    //
    // ������ ���������� ������ ����� IDatabase
    // 
    // �� ������� ��������� ������������ ���� IDatabase
    //

    IDatabase *mongoDb = IDatabase::GetInterface(eCMongoDb);

    IDatabase* postgres = IDatabase::GetInterface(ePostgreSql);

    IDatabase* postgres11 = IDatabase::GetInterface(ePostgreSql);


    //
    // ����������, �� ��������� �������� �� ���� � ��� ����� ��'���!
    // 
    // �� � � �����, �� ������ ������.
    //
    if (postgres != postgres11)
    {
        std::cout << "Error: classes instances are not same !" << std::endl;
        return -1;
    }



    return 0;
}

