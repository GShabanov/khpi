#pragma once

class CUserHandler : public CHandlerBase
{
public:
    CUserHandler(CAdminClass* pAdminClass)
        : CHandlerBase(pAdminClass)
    {
    }

protected:
    virtual Request* GetExistingRequestFromDb(int id)
    {
        Request* request = new Request;

        request->m_Id = id;

        return request;
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

    virtual _Response FormatResponse(bool updateSucceeded, const Request& request)
    {
        _Response response = __super::FormatResponse(updateSucceeded, request);


        if (updateSucceeded)
        {

            response.message = "user updated";
        }
        else
        {

            response.message = "user update failed";
        }


        return response;
    }
};
