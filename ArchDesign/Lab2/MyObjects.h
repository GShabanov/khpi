#pragma once
#include <assert.h>
#include <string>


class IComponent;

//
// mediator class
//
class IMediator
{
public:
    virtual void Notify(IComponent *sender, const std::string &from) = 0;
};


//
// base class for all components
//
class IComponent
{
protected:
    IMediator* m_mediator;

    bool       m_active;

public:
    IComponent(IMediator* mediator)
    {
        m_active = true;
        m_mediator = mediator;
    }

    bool Active() {
        return m_active;
    }

    void Active(bool active) {
        m_active = active;
    }

};


class CCheckbox : public IComponent
{
private:
    bool m_isChecked;


public:
    void OnCheck(bool isChecked)
    {
        m_isChecked = isChecked;

        this->m_mediator->Notify(this, std::string("CCheckbox"));
    }

    CCheckbox(IMediator* mediator)
        : IComponent(mediator)
        , m_isChecked(false)
    {
    }

    bool IsChecked() {
        return m_isChecked;
    }

    void Active(bool active) {
        m_active = active;
    }

};

class CDate : public IComponent
{
private:
    time_t m_value;


public:
    void OnChange(time_t value)
    {
        m_value = value;

        this->m_mediator->Notify(this, std::string("CData"));
    }

    CDate(IMediator* mediator)
        : IComponent(mediator)
        , m_value(0)
    {
    }

    time_t Date() { return m_value; }

};

class CTimespan : public IComponent
{
private:

    time_t            m_selectedTimeSpan;
    std::list<time_t> m_avaliableTimeSpans;


public:
    void OnChange(time_t value)
    {
        m_selectedTimeSpan = value;

        this->m_mediator->Notify(this, std::string("CTimespan"));
    }

    CTimespan(IMediator* mediator)
        : IComponent(mediator)
        , m_selectedTimeSpan(0)
    {
    }

    void Init(time_t date)
    {

        for (int i = 0; i < 8; i++)
        {
            m_avaliableTimeSpans.push_back(time_t(date + i * 16));
        }
    }

};


class CTextbox : public IComponent
{
private:
    std::string m_value;
    bool        m_required;

    void OnChange(const std::string& value)
    {
        m_value = value;

        this->m_mediator->Notify(this, std::string("CTextbox"));
    }

public:
    CTextbox(IMediator* mediator)
        : IComponent(mediator)
        , m_required(false)
    {
    }

    const std::string Value() { return m_value; }

    bool IsRequired() {
        return m_required;
    }
    void IsRequired(bool required) {
        m_required = required;
    }


};
