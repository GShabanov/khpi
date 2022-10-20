#pragma once

using namespace std;

//
// see main.cpp for comments
//


//
// 2. Open Closed Principle
// 
// CAdminUser is extending functionality
//

class IUser
{
protected:
    class ILogger* m_logger;

public:

    IUser(class ILogger* logger)
    {
        m_logger = logger;
    }

    virtual void  AssignTask(class ITaskNotify* taskNote) = 0;

    virtual bool  IsTaskAssigned() = 0;

    virtual bool  IsAdmin() = 0;

    virtual void  Work() = 0;

    virtual ~IUser() { };
};


class CUser : public IUser
{
protected:

    string          m_name;
    bool            m_isAdmin;
    bool            m_taskAssigned;

    std::list<ITask*>  m_taskList;

    //
    // only user with a string is allowed to contruct
    //
    CUser(class ILogger* logger) : IUser(logger)
    {
        m_taskAssigned = false;
        m_isAdmin = false;
    }

public:
    CUser(class ILogger* logger, const string name)
        : CUser(logger)
    {
        m_name = string(name);
    }

    virtual ~CUser()
    {
        IUser::~IUser();
    }


    virtual void  AssignTask(class ITaskNotify* taskNote);

    virtual bool  IsTaskAssigned() {
        return m_taskAssigned;
    }

    virtual bool  IsAdmin() {
        return m_isAdmin;
    }

    virtual void  Work();
};


//
// the same class as regular user but for admin
//
class CAdminUser : public CUser
{
    //
    // 3. Liskov Substitution Principle
    //
    // We are using declaration inside base class
    //

public:
    CAdminUser(class ILogger* logger, const string name)
        : CUser(logger, name)
    {
        m_isAdmin = true;
        m_name = string(name);
    }

    virtual ~CAdminUser()
    {
        IUser::~IUser();
    }

    virtual void  AssignTask(class ITaskNotify* taskNote);

    virtual bool  IsTaskAssigned() {
        return false;
    }

    virtual bool  IsAdmin() {
        return m_isAdmin;
    }

    virtual void  Work()
    {

    }
};