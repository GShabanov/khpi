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
#include "pch.h"

#include "controlState.h"

void
CStateMediator::subscribe(CStateComponent* component)
{
    //
    // check already present
    //
    for (INT_PTR i = 0, j = m_components.GetSize(); i < j; i++)
    {
        if (m_components[i] == component)
        {
            return;
        }
    }

    m_components.Add(component);
}

void
CStateMediator::unsubscribe(CStateComponent* component)
{
    for (INT_PTR i = 0, j = m_components.GetSize(); i < j; i++)
    {
        if (m_components[i] == component)
        {
            m_components.RemoveAt(i);
            break;
        }

    }
}


void
CStateMediator::UpdateAngle(CStateComponent* sender, int num, float angle)
{
    this->m_angles[num] = angle;

    for (INT_PTR i = 0, j = m_components.GetSize(); i < j; i++)
    {
        if (m_components[i] != sender)
        {
            m_components[i]->OnSetAngle(num, angle);
        }

    }
}

