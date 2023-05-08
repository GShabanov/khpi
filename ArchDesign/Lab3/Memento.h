class CMemento
{

public:
    int  m_Id;

    bool m_allowCookie;

    bool m_profileComplete;

    bool m_propositions;


    CMemento(int Id)
    {
        m_Id = Id;
        m_allowCookie = false;
        m_profileComplete = false;
        m_propositions = false;
    }

    CMemento(int Id, bool allowCookie, bool profileComplete, bool propositions)
    {
        m_Id = Id;


        m_allowCookie = allowCookie;

        m_profileComplete = profileComplete;

        m_propositions = propositions;
    }

public:
    int Id() {
        return m_Id;
    }

    bool AllowCookie() {
        return m_allowCookie;
    }

    bool ProfileComplete() {
        return m_profileComplete;
    }

    bool Propositions() {
        return m_propositions;
    }

};

