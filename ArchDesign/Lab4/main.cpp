
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>


class IDeliveryStrategy
{
public:
    virtual double CalculateCost(class CLocation* location) = 0;
};


class OtherServiceStrategy : public IDeliveryStrategy
{
    virtual double CalculateCost(class CLocation* locations)
    {

        UNREFERENCED_PARAMETER(locations);

        double sum = 0.6;

        //
        // for (location in locations) 
        //     sum += location.costs();
        //

        return sum;
    }
};

class OwnDeliveryStrategy : public IDeliveryStrategy
{
    virtual double CalculateCost(class CLocation* locations)
    {
        UNREFERENCED_PARAMETER(locations);

        double sum = 1.6;

        //
        // for (location in locations) 
        //     sum += location.costs();
        //

        return sum;
    }
};


class SelfDeliveryStrategy : public IDeliveryStrategy
{
    virtual double CalculateCost(class CLocation* locations)
    {
        UNREFERENCED_PARAMETER(locations);

        return 0.0;
    }
};



class CFoodDelivery
{
private:
    IDeliveryStrategy* m_strategy;

public:
    CFoodDelivery(IDeliveryStrategy* strategy)
    {
        m_strategy = strategy;
    }


    void SetStrategy(IDeliveryStrategy* strategy)
    {
        m_strategy = strategy;
    }

    //
    // вираховуємо суму згідно стратегії
    //
    double CalculateCost(class CLocation* locations)
    {
        return m_strategy->CalculateCost(locations);
    }
};

int
_tmain(int argc, TCHAR* argv[])
{

    SelfDeliveryStrategy  selfDelivery;
    OwnDeliveryStrategy   ownDelivery;


    //
    // по замовченню стратегія - самовивіз
    //
    CFoodDelivery* pService = new CFoodDelivery(&selfDelivery);


    double costs = pService->CalculateCost(NULL);


    std::cout << "self delivery costs: " << costs << std::endl;

    //
    // змінюємо стратегію 
    //
    pService->SetStrategy(&ownDelivery);


    costs = pService->CalculateCost(NULL);


    std::cout << "Own delivery costs: " << costs << std::endl;


    delete pService;

    return 0;
}

