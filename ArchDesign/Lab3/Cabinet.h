#include "Memento.h"


class CCabinet : public CMemento
{
    //
    // ми отримуємо поля в класі від екземпляра CMemento
    //


public:

    CCabinet(int Id)
        : CMemento(Id)
    {

    }

    //
    // дозвіл на серіалізацію класу в класі CMemento
    //
    CMemento* SaveState()
    {

        return new CMemento(m_Id, m_allowCookie, m_profileComplete, m_propositions);
    }

    void RestoreState(CMemento* memento)
    {
        m_Id = memento->m_Id;
        m_allowCookie = memento->m_allowCookie;
        m_profileComplete = memento->m_profileComplete;
        m_propositions = memento->m_propositions;
    }




    void UserId(int value) {
        m_Id = value;
    }

    void AllowCookie(bool value) {
        m_allowCookie = value;
    }

    void ProfileComplete(bool value) {
        m_profileComplete = value;
    }

    void Propositions(bool value) {
        m_propositions = value;
    }

};
