#include <string>
#include <list>
#include <iostream>

#include "Project.h"
#include "User.h"
#include "Task.h"
#include "Log.h"
#include "TaskNotify.h"


CProject::~CProject()
{
    while (!m_userList.empty())
    {
        IUser* user = m_userList.back();

        delete user;

        m_userList.pop_back();

    }

    while (!m_taskList.empty())
    {
        ITask* task = m_taskList.back();

        delete task;

        m_taskList.pop_back();

    }

    IProject::~IProject();
}

void CProject::AddUser(IUser *user)
{
    m_userList.push_back(user);

    m_ProjectLog->LogEvent("[*]  User is added to project");
}

void CProject::AddTask(class ITask *task)
{
    m_taskList.push_back(task);


}

void CProject::UpdateProject()
{
    bool isAssigned = false;

    for ( ; ; )
    {
        //
        // assign tasks
        //
        if (!m_taskList.empty())
        {
            ITask* currentTask = m_taskList.front();

            ITaskNotify* newTask;

            if (m_NotifyBy == 0)
            {

                newTask = new CSmsNotify(currentTask, "SMS for new task!");

                m_NotifyBy++;
            }
            else if (m_NotifyBy == 1)
            {
                newTask = new CEmailNotify(currentTask, "Email for new task!");

                m_NotifyBy++;
            }
            else
            {

                newTask = new CMessengerNotify(currentTask, "Email for new task!");
                m_NotifyBy = 0;
            }


            for (UserIter i = m_userList.begin(); i != m_userList.end(); i++)
            {

                IUser* user = (*i);


                if (!user->IsTaskAssigned() && user->IsAdmin())
                {
                    user->AssignTask(newTask);


                    //
                    // inform admin user
                    //
                    for (UserIter j = m_userList.begin(); j != m_userList.end(); j++)
                    {

                        IUser* adminUser = (*j);
                        //
                        // inform admin user
                        //
                        if (adminUser->IsAdmin())
                        {
                            adminUser->AssignTask(newTask);
                        }
                    }

                    isAssigned = true;
                }

            }

            delete newTask;

            if (!isAssigned)
                break;

            //
            // remove assigned task
            //
            m_taskList.pop_front();


        }
        else
        {
            break;
        }

        //
        // all Users busy
        //
        if (!isAssigned)
            break;

    }
    //
    // work on tasks
    //
    for (UserIter i = m_userList.begin(); i != m_userList.end(); i++)
    {
        (*i)->Work();
    }

}