#pragma once

using namespace std;

class IProject
{
protected:
    class ILogger* m_ProjectLog;

public:

    IProject(class ILogger* logger)
        : m_ProjectLog(logger)
    {
    }

    virtual void  AddUser(class IUser* user) = 0;

    virtual void  AddTask(class ITask* task) = 0;

    virtual void  UpdateProject() = 0;

    virtual ~IProject() { };
};


class CProject : public IProject
{
private:

    std::list<IUser *>  m_userList;
    std::list<ITask *>  m_taskList;
    typedef list<IUser *>::iterator UserIter;
    typedef list<ITask *>::iterator TaskIter;


    int m_NotifyBy;

public:
    CProject(class ILogger* logger)
        : IProject(logger)
    {
        m_NotifyBy = 0;
    }

    virtual ~CProject();

    virtual void  AddUser(class IUser *user);

    virtual void  AddTask(class ITask *task);

    virtual void  UpdateProject();

};