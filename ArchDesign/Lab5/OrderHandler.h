#pragma once

#include "Handlers.h"

class COrderHandler : public CHandlerBase
{
public:
    COrderHandler(CAdminClass* pAdminClass)
        : CHandlerBase(pAdminClass)
    {
    }

protected:
    virtual Request* GetExistingRequestFromDb(int id)
    {
        Request* model = new Request;

        model->m_Id = id;

        return model;
    }

    virtual bool CheckOrder(const Request& existingObject, const Request& request)
    {

        UNREFERENCED_PARAMETER(existingObject);
        UNREFERENCED_PARAMETER(request);

        return true;
    }

    virtual bool UpdateRequestInDatabase(Request& request)
    {
        UNREFERENCED_PARAMETER(request);
        return true;
    }

    virtual _Response FormatResponse(bool updateSucceeded, Request &request)
    {
        _Response response = __super::FormatResponse(updateSucceeded, request);


        if (updateSucceeded) {

            response.message = "order updated";
        }
        else {
            response.message = "order update failed";
        }


        return response;
    }
};
