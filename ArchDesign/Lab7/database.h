#pragma once

typedef enum {

    eCMongoDb = 0,
    ePostgreSql,
} dbType;


class IDatabase
{


public:

    virtual bool Create(std::string& name, int handle) = 0;

    virtual unsigned char* Read(int handle, size_t size) = 0;

    virtual void Update(int handle, std::string& value, void* data) = 0;

    virtual void Delete(std::string& name) = 0;


public:

    //
    // factory method
    //
    static IDatabase* GetInterface(dbType type);
};




class CMongoDb : public Singleton<CMongoDb>, public IDatabase
{
public:
    CMongoDb()
    {
        std::cout << "CMongoDb::CMongoDb()" << std::endl;
    }

    virtual bool Create(std::string& name, int handle) {

        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(handle);

        throw std::string("unimplemented");

        return true;
    }

    virtual unsigned char* Read(int handle, size_t size)
    {
        UNREFERENCED_PARAMETER(handle);
        UNREFERENCED_PARAMETER(size);

        throw std::string("unimplemented");

        return NULL;
    }

    virtual void Update(int handle, std::string& value, void* data)
    {
        UNREFERENCED_PARAMETER(handle);
        UNREFERENCED_PARAMETER(value);
        UNREFERENCED_PARAMETER(data);

        throw std::string("unimplemented");
    }

    virtual void Delete(std::string& name)
    {
        UNREFERENCED_PARAMETER(name);

        throw std::string("unimplemented");
    }
};

class CPostgresSql : public Singleton<CPostgresSql>, public IDatabase
{


public:
    CPostgresSql()
    {
        std::cout << "CPostgresSql::CPostgresSql()" << std::endl;
    }

    virtual bool Create(std::string& name, int handle) {

        UNREFERENCED_PARAMETER(name);
        UNREFERENCED_PARAMETER(handle);

        throw std::string("unimplemented");

        return true;
    }

    virtual unsigned char* Read(int handle, size_t size)
    {
        UNREFERENCED_PARAMETER(handle);
        UNREFERENCED_PARAMETER(size);

        throw std::string("unimplemented");

        return NULL;
    }

    virtual void Update(int handle, std::string& value, void* data)
    {
        UNREFERENCED_PARAMETER(handle);
        UNREFERENCED_PARAMETER(value);
        UNREFERENCED_PARAMETER(data);

        throw std::string("unimplemented");
    }

    virtual void Delete(std::string& name)
    {
        UNREFERENCED_PARAMETER(name);

        throw std::string("unimplemented");
    }
};


//
// factory method
//

IDatabase*
IDatabase::GetInterface(dbType type)
{
    switch (type)
    {
    case eCMongoDb:

        return CMongoDb::Get();

    case ePostgreSql:

        return CPostgresSql::Get();

    default:

        throw std::string("unknown database !!");
    }

}
