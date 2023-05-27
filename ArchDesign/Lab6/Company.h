#pragma once


#include "Visitor.h"
#include "Department.h"

class CCompany : public IBase
{
protected:

    std::list<CDepartment*>   m_departments;

public:
    CCompany(const std::string& name, const std::list<CDepartment*> departments)
        : IBase(name)
    {
        m_departments = departments;
    }

    std::list<CDepartment*>& GetDepartmentList() {

        return m_departments;
    }

    virtual void* OnVisit(IVisitor& visitor)
    {
        return visitor.VisitCompany(*this);
    }

protected:
};
