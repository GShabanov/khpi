#pragma once

#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

#include <string>

class INotification
{

public:

    virtual void Send(std::string title, std::string message) = 0;

};


#endif // __NOTIFICATION_H__