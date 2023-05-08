
#include <windows.h>
#include <tchar.h>
#include <string>
#include <list>
#include <iostream>

#include "singleton.h"
#include "databaseBuilder.h"



int
_tmain(int argc, TCHAR* argv[])
{

    //
    // ������������� ������
    //
    IDataBaseBuiler* PostgreSqlBuilder = IDataBaseBuiler::QueryBuilder(ePostgreSql);

    IDataBaseBuiler* mySqlSqlBuilder = IDataBaseBuiler::QueryBuilder(eMySQL);


    //
    // ��������� ���� ������ ������� ����, � ��������� �� Builder, � ����� �����������, ���. ����� GetDataBase
    //
    IDatabase *database1 = PostgreSqlBuilder->GetDataBase();
    IDatabase* database2 = mySqlSqlBuilder->GetDataBase();


    //
    // ��������� ��'���� builder-�
    //
    PostgreSqlBuilder->Release();

    mySqlSqlBuilder->Release();



    //
    // ������������� ������������ ���������� !
    // ��� ��� ��������� ���� �������
    //
    // IDatabase* database3 = new CPostgreSQL();


    //
    // ������ � ����� �����
    //
    database1->Select(std::string("Select"));

    database2->Select(std::string("Select"));


    delete database1;
    delete database2;

    return 0;
}

