#pragma once

using namespace std;

class ITask
{
public:

    virtual string  Name() = 0;

protected:
    //
    // do not allow explicit instantiation
    //
    ITask() { }


};


class CTask : public ITask
{
private:

    string  m_name;

public:
    CTask(string name)
        : ITask()
    {
        m_name = name;
    }

    ~CTask()
    {
    }

    virtual string  Name()
    {
        return string(m_name);
    }
};