
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

    CConsoleLogger  logger;
    CProject        project(&logger);


    //
    // adding users for project
    //
    for (int i = 0; i < USERS_COUNT; i++) {

        IUser* user = nullptr;
        //
        // assign user 0 as administrator
        //
        if (i == 0)
        {
            user = new CAdminUser(&logger, "admin " + to_string(i));

            if (user == nullptr)
            {
                return -1;
            }

        }
        else
        {
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

