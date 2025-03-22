/***************************************************************************************\
*   File:                                                                               *
*       controlState.cpp                                                                *
*                                                                                       *
*   Abstract:                                                                           *
*       Observer for component state interface implementation                           *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    7-Sep-2024                                                      *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#pragma once
#ifndef __CONTROLSTATE_H__
#define __CONTROLSTATE_H__

class CStateComponent;


//
// Mediator class
//
class CStateMediator
{
private:
    CArray<CStateComponent*>   m_components;

public:
    float                  m_angles[6];

    CStateMediator() {
        memset(m_angles, 0, sizeof(m_angles));
    }

    void subscribe(CStateComponent* component);
    void unsubscribe(CStateComponent* component);
    
    void UpdateAngle(CStateComponent* sender, int num, float angle);
};

// 
// observer class
//
class CStateComponent
{

protected:
    class CStateMediator* m_pMediator;

public:
    CStateComponent() {
        m_pMediator = NULL;
    }

    CStateComponent(class CStateMediator* pMediator) {
        m_pMediator = pMediator;
    }

    void setMediator(class CStateMediator* pMediator) {
        m_pMediator = pMediator;
    }

    void subscribeComponent() {
        m_pMediator->subscribe(this);
    }

    virtual ~CStateComponent() = default;

    virtual void  OnSetAngle(int num, float angle) { };
};



#endif // __ROBOTSTATE_H__