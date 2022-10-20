
#include <windows.h>
#include <tchar.h>
#include <string>
#include <list>
#include <iostream>

#include "Project.h"
#include "User.h"
#include "Task.h"
#include "Log.h"

#define USERS_COUNT  5

#define TASKS_COUNT  10

int
_tmain(int argc, TCHAR* argv[])
{

    //
    // 1. Single Responsibility Principle
    // 
    // We are passing classes functionality 
    //

    //
    // 2. Open Closed Principle
    // 
    // See declaration CAdminUser inside User.h
    //

    //
    // 3. Liskov Substitution Principle
    //
    // See declaration CAdminUser inside User.h
    //

    //
    // 4. Interface Segregation Principle
    //
    // See declaration IUser / CUser / CAdminUser inside User.h
    // See declaration ITaskNotify / inside TaskNotify.h
    //

    //
    // 5. Dependency Inversion Principle
    //
    //  project.UpdateProject();
    // 
    //  is working with abstactions of IUser, ITask, ITaskNotify
    //


    CConsoleLogger  logger;
    CProject        project(&logger);


    

    //
    // adding users for project
    //
    for (int i = 0; i < USERS_COUNT; i++)
    {

        IUser* user = nullptr;

        //
        // assign user 0 as administrator
        //
        if (i == 0)
        {
            //
            // 2. Open Closed Principle
            //
            user = new CAdminUser(&logger, "admin " + to_string(i));

            if (user == nullptr)
            {
                return -1;
            }

        }
        else
        {
            //
            // 2. Open Closed Principle
            //
            user = new CUser(&logger, "user " + to_string(i));

            if (user == nullptr)
            {
                return -1;
            }
        }

        project.AddUser(user);
    }


    //
    // adding tasks for project
    //
    for (int i = 0; i < TASKS_COUNT; i++)
    {
        ITask* task = new CTask("task N" + to_string(i));

        if (task == nullptr)
        {
            return -1;
        }

        project.AddTask(task);
    }

    //
    // work with project
    //
    for (int i = 0; i < TASKS_COUNT; i++)
    {
        project.UpdateProject();
    }


    return 0;
}

