#pragma once

using namespace std;

class ITaskNotify
{
protected:
    ITask*  m_task;
    string  m_text;

    ITaskNotify(ITask* task)
    {
        m_task = task;
    }

public:

    virtual ITask *GetTask() = 0;

    virtual string GetText() {

        return m_text;
    };

};


class CSmsNotify : public ITaskNotify
{

public:

    CSmsNotify(ITask *task, string text)
        : ITaskNotify(task)
    {
        m_text = "SMS: " + text;
    }

    virtual ITask* GetTask()
    {
        return m_task;
    }

};

class CEmailNotify : public ITaskNotify
{

public:

    CEmailNotify(ITask* task, string text)
        : ITaskNotify(task)
    {
        m_text = "Email: " + text;
    }

    virtual ITask* GetTask()
    {
        return m_task;
    }

};

class CMessengerNotify : public ITaskNotify
{

public:

    CMessengerNotify(ITask* task, string text)
        : ITaskNotify(task)
    {
        m_text = "Messanger: " + text;
    }

    virtual ITask* GetTask()
    {
        return m_task;
    }

};
