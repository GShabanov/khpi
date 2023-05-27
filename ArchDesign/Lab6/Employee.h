#pragma once

#ifndef __EMPLOYEE_H__
#define __EMPLOYEE_H__

#include "Visitor.h"

class CEmployee : public IBase
{
    unsigned int m_salary;

public:
    CEmployee(const std::string &name, unsigned int salary)
        : IBase(name)
    {
        m_salary = salary;
    }

    unsigned int GetSalary() {
        return m_salary;
    }

    virtual void* OnVisit(IVisitor& visitor)
    {
        return visitor.VisitEmployee(*this);
    }

protected:
};

#endif // __EMPLOYEE_H__
