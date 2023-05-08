#pragma once

#include "database.h"

typedef enum {

    ePostgreSql = 0,
    eMySQL,
} dbType;


class IDataBaseBuiler
{

public:
    virtual IDatabase *GetDataBase() = 0;

    virtual void Release() = 0;

    static IDataBaseBuiler* QueryBuilder(dbType builderType);
};

class CPostgreSQLBuilder : public IDataBaseBuiler
{

public:

    virtual IDatabase* GetDataBase()
    {
        CPostgreSQL* sql = new CPostgreSQL();

        //
        // ��� ���� ���� ����������� ����������� ��� CPostgreSQL ��������
        //

        return sql;
    }

    virtual void Release()
    {
        delete this;
    }

    CPostgreSQLBuilder()
    {
    }

};


class CMySQLBuilder : public IDataBaseBuiler
{

public:
    virtual IDatabase* GetDataBase()
    {

        CMySQL *mySql = new CMySQL();

        //
        // ��� ���� ���� ����������� ����������� ��� CMySQL ��������
        //
        
        return mySql;

    }

    virtual void Release()
    {
        delete this;
    }


    CMySQLBuilder()
    {

    }



};


IDataBaseBuiler*
IDataBaseBuiler::QueryBuilder(dbType builderType)
{
    switch (builderType)
    {
    case ePostgreSql:

        return new CPostgreSQLBuilder();

    case eMySQL:

        return new CMySQLBuilder();

    default:

        throw std::string("unknown database !!");
    }
}