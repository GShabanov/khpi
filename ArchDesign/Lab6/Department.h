#pragma once

#ifndef __DEPARTMENT_H__
#define __DEPARTMENT_H__

#include "Visitor.h"
#include "Employee.h"

class CDepartment : public IBase
{
protected:
    std::list<CEmployee*>   m_employees;

public:
    CDepartment(const std::string& name, const std::list<CEmployee*> employees)
        : IBase(name)
    {
        m_employees = employees;
    }

    std::list<CEmployee*> & GetEmployeesList() {

        return m_employees;
    }


    virtual void *OnVisit(IVisitor& visitor)
    {
        return visitor.VisitDepartment(*this);
    }

protected:
};


#endif // __DEPARTMENT_H__