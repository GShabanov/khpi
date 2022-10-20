#include <string>
#include <list>
#include <iostream>

#include "Task.h"
#include "User.h"
#include "TaskNotify.h"
#include "Log.h"

//
// see main.cpp for comments
//


void  CUser::AssignTask(ITaskNotify* taskNote)
{
    m_taskList.push_back(taskNote->GetTask());

    m_taskAssigned = true;
}


void  CUser::Work()
{
    if (m_taskAssigned)
    {
        ITask* task = m_taskList.front();

        delete task;

        m_taskList.pop_front();

        m_taskAssigned = false;
    }
}


void  CAdminUser::AssignTask(class ITaskNotify* taskNote)
{
    taskNote->GetTask();

    m_logger->LogEvent("[*] " + taskNote->GetText() + " " + taskNote->GetTask()->Name());
}
