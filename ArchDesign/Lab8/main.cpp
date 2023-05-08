
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
    // конструювання білдерів
    //
    IDataBaseBuiler* PostgreSqlBuilder = IDataBaseBuiler::QueryBuilder(ePostgreSql);

    IDataBaseBuiler* mySqlSqlBuilder = IDataBaseBuiler::QueryBuilder(eMySQL);


    //
    // створення бази данних певного типу, в залежності від Builder, а також ініціалізація, див. метод GetDataBase
    //
    IDatabase *database1 = PostgreSqlBuilder->GetDataBase();
    IDatabase* database2 = mySqlSqlBuilder->GetDataBase();


    //
    // звільняємо об'єкти builder-у
    //
    PostgreSqlBuilder->Release();

    mySqlSqlBuilder->Release();



    //
    // безпосередньо конструювати заборонено !
    // тут без коментарів буде помилка
    //
    // IDatabase* database3 = new CPostgreSQL();


    //
    // робота з базою даних
    //
    database1->Select(std::string("Select"));

    database2->Select(std::string("Select"));


    delete database1;
    delete database2;

    return 0;
}

