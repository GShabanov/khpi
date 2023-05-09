#pragma once

#include <string>
#include <iostream>

struct  Request
{
    int m_Id;
};

struct _Response {

    int          code;
    std::string  message;

};

class CAdminClass
{

public:
    void OnNotifyAdmin(const _Response &response)
    {

        std::cout << "validation failed: " << response.message << ", (code: " << response.code << ")\n";
    }

};


class CHandlerBase
{
    CAdminClass* m_pAdminClass;

    CHandlerBase() 
        : m_pAdminClass(NULL)
    {
    }

public:

    CHandlerBase(CAdminClass* pAdminClass) {

        m_pAdminClass = pAdminClass;
    }

    //
    // процесінг запиту (будь якого)
    //

    _Response ProcessRequest(Request request)
    {
        Request *existingObject = GetExistingRequestFromDb(request.m_Id);

        if (!CheckOrder(*existingObject, request))
        {

            _Response  response;

            response.code = 400;
            response.message = "order is invalid";

            //
            // нотіфікуємо адміна
            //
            if (m_pAdminClass != NULL)
            {

                m_pAdminClass->OnNotifyAdmin(response);
            }

            return response;
        }

        bool updateSucceeded = UpdateRequestInDatabase(request);

        return FormatResponse(updateSucceeded, request);
    }

protected:
    virtual Request* GetExistingRequestFromDb(int id) = 0;
    virtual bool CheckOrder(const Request &existingObject, const Request &request) = 0;
    virtual bool UpdateRequestInDatabase(Request &request) = 0;

    virtual _Response FormatResponse(bool updateSucceeded, const Request &request)
    {
        _Response  response;

        if (updateSucceeded) {

            response.code = 200;
            response.message = "updated";
        }
        else
        {
            response.code = 500;
            response.message = "update failed";
        }

        return response;
    }

};
