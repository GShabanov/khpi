#pragma once

#ifndef __VISITOR_H__
#define __VISITOR_H__


#include <list>


class CEmployee;
class CDepartment;
class CCompany;
class IVisitor;


class IBase
{
protected:
    std::string             m_name;

public:
    IBase(const std::string& name)
    {
        m_name = name;
    }

    std::string GetName() {
        return m_name;
    }

    virtual void *OnVisit(IVisitor& visitor) = 0;
};

class IVisitor
{
public:
    virtual void* VisitEmployee(CEmployee& employee) = 0;

    virtual void* VisitDepartment(CDepartment &department) = 0;

    virtual void* VisitCompany(CCompany &company) = 0;
};



//
// Salary report using Visitor implementation
//

class CSalaryReport : public IVisitor
{
public:
    virtual void* VisitEmployee(CEmployee& employee);

    virtual void* VisitDepartment(CDepartment& department);

    virtual void* VisitCompany(CCompany& company);

};

//
// Name report using Visitor implementation
//

class CNameReport : public IVisitor
{
public:
    virtual void* VisitEmployee(CEmployee& employee);

    virtual void* VisitDepartment(CDepartment& department);

    virtual void* VisitCompany(CCompany& company);

};


#endif // __VISITOR_H__

