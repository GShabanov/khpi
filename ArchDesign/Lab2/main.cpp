
#include <windows.h>
#include <tchar.h>
#include <string>
#include <list>
#include <iostream>

#include "MyObjects.h"


class CMyPage : public IMediator
{
private:
    CTextbox    *m_Name;
    CTextbox    *m_Phone;

    CCheckbox   *m_TakeFromShop;
    CCheckbox   *m_FromOtherReciever;

    CDate       *m_DeliveryDate;
    CTimespan   *m_DeliveryTime;


    virtual void Notify(IComponent* sender, const std::string& from)
    {
        std::cout << "event from: " << from << std::endl;

        if (sender == m_TakeFromShop)
        {

            CCheckbox* _sender = (CCheckbox*)sender;

            m_Name->Active(!_sender->IsChecked() && m_FromOtherReciever->IsChecked());
            m_Phone->Active(!_sender->IsChecked() && m_FromOtherReciever->IsChecked());
            m_DeliveryDate->Active(!_sender->IsChecked());
            m_FromOtherReciever->Active(!_sender->IsChecked());
        }
        else
        if (sender == m_FromOtherReciever)
        {
            CCheckbox* _sender = (CCheckbox*)sender;

            m_Name->Active(_sender->IsChecked());
            m_Name->IsRequired(_sender->IsChecked());
            m_Phone->Active(_sender->IsChecked());
            m_Phone->IsRequired(_sender->IsChecked());
        }
        else
        if (sender == m_DeliveryDate)
        {
            CTimespan* _sender = (CTimespan*)sender;

            m_DeliveryTime->Init(m_DeliveryDate->Date());
            m_DeliveryTime->Active(true);
        }
        else
        {
            throw new std::invalid_argument("invalid event sender");
        }

    }

public:
    CMyPage()
    {
        m_Name = new CTextbox(this);
        m_Phone = new CTextbox(this);
        m_FromOtherReciever = new CCheckbox(this);
        m_TakeFromShop = new CCheckbox(this);

        m_DeliveryDate = new CDate(this);
        m_DeliveryTime = new CTimespan(this);
    }

    //
    // simulation event
    //
    void ClickFromShop()
    {
        m_FromOtherReciever->OnCheck(false);
        m_TakeFromShop->OnCheck(true);
    }

    void ClickFromOther()
    {
        m_TakeFromShop->OnCheck(false);
        m_FromOtherReciever->OnCheck(true);
    }

    ~CMyPage()
    {
        delete m_Name;
        delete m_Phone;
        delete m_FromOtherReciever;
        delete m_TakeFromShop;

        delete m_DeliveryDate;
        delete m_DeliveryTime;
    }
};

int
_tmain(int argc, TCHAR* argv[])
{
    CMyPage   page;

    page.ClickFromShop();

    page.ClickFromOther();


    return 0;
}

