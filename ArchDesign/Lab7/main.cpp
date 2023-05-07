
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
    // виклик фабричного методу класу IDatabase
    // 
    // в≥н створюЇ екземпл€р абстрактного типу IDatabase
    //

    IDatabase *mongoDb = IDatabase::GetInterface(eCMongoDb);

    IDatabase* postgres = IDatabase::GetInterface(ePostgreSql);

    IDatabase* postgres11 = IDatabase::GetInterface(ePostgreSql);


    //
    // перев≥р€Їмо, що вказ≥вники указують на один ≥ той самий об'Їкт!
    // 
    // це ≥ Ї доказ, що одинак працюЇ.
    //
    if (postgres != postgres11)
    {
        std::cout << "Error: classes instances are not same !" << std::endl;
        return -1;
    }



    return 0;
}

