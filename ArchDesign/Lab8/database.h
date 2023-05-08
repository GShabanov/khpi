#pragma once


class IDatabase
{


public:

    virtual bool Select(const std::string& name) = 0;

    virtual bool Where(std::string& name) = 0;

    virtual void Limit(std::string& name) = 0;

    virtual std::string getSql(std::string& query) = 0;

};




class CPostgreSQL : public IDatabase
{

protected:

    //
    // заборонено безпосередньо конструювати, тільки через Builder
    //

    CPostgreSQL()
    {
        std::cout << "CPostgreSQL::CPostgreSQL()" << std::endl;
    }

public:
    virtual bool Select(const std::string& name)
    {

        std::cout << "CPostgreSQL::Select : " << name << std::endl;

        return true;
    }

    virtual bool Where(std::string& name)
    {
        UNREFERENCED_PARAMETER(name);

        throw std::string("unimplemented");

        return true;
    }

    virtual void Limit(std::string& name)
    {
        UNREFERENCED_PARAMETER(name);

        throw std::string("unimplemented");

    }

    virtual std::string getSql(std::string& query)
    {
        UNREFERENCED_PARAMETER(query);

        return std::string("unimplemented");

    }


    friend class CPostgreSQLBuilder;

};

class CMySQL : public IDatabase
{

protected:

    //
    // заборонено безпосередньо конструювати, тільки через Builder
    //
    CMySQL()
    {
        std::cout << "CMySQL::CMySQL()" << std::endl;
    }


public:

    virtual bool Select(const std::string& name)
    {
        std::cout << "CMySQL::Select : " << name << std::endl;

        return true;
    }

    virtual bool Where(std::string& name)
    {
        UNREFERENCED_PARAMETER(name);

        throw std::string("unimplemented");

        return true;
    }

    virtual void Limit(std::string& name)
    {
        UNREFERENCED_PARAMETER(name);

        throw std::string("unimplemented");

    }

    virtual std::string getSql(std::string& query)
    {
        UNREFERENCED_PARAMETER(query);

        return std::string("unimplemented");

    }

    friend class CMySQLBuilder;
};
