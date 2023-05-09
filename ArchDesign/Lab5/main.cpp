
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>

#include "OrderHandler.h"
#include "ProductHandler.h"
#include "UserHandler.h"


class COrderProcessing
{
public:

    _Response ProcessRequest(CHandlerBase *handler, Request& request)
    {
        return handler->ProcessRequest(request);
    }

};

int
_tmain(int argc, TCHAR* argv[])
{

    Request  productRequest = { 1 };
    Request  userRequest = { 2 };
    Request  orderRequest = { 3 };



    CAdminClass      adminReportService;
     

    COrderProcessing processing;


    CProductHandler productHandler(&adminReportService);
    CUserHandler    userHandler(&adminReportService);
    COrderHandler   orderHandler(&adminReportService);


    processing.ProcessRequest(&productHandler, productRequest);
    processing.ProcessRequest(&userHandler, userRequest);
    processing.ProcessRequest(&orderHandler, orderRequest);


    return 0;
}

