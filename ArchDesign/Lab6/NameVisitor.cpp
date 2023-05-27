
#include <string>
#include <list>
#include <iostream>

#include "Visitor.h"
#include "Employee.h"
#include "Department.h"
#include "Company.h"


void*
CNameReport::VisitEmployee(CEmployee& employee)
{
    std::cout << "[*]        Employee: " << employee.GetName() << std::endl;

    return (void*)NULL;
}

void*
CNameReport::VisitDepartment(CDepartment& department)
{

    std::list<CEmployee*>::iterator Iter;

    std::cout << "[*]    Department: " << department.GetName() << std::endl;


    for (Iter = department.GetEmployeesList().begin(); Iter != department.GetEmployeesList().end(); Iter++)
    {
        CEmployee* employee = *Iter;

        employee->OnVisit(*this);
    }

    return (void*)NULL;
}

void*
CNameReport::VisitCompany(CCompany& company)
{

    std::list<CDepartment*>::iterator Iter;

    std::cout << "[*] Company: " << company.GetName() << std::endl;


    for (Iter = company.GetDepartmentList().begin(); Iter != company.GetDepartmentList().end(); Iter++)
    {
        CDepartment* department = *Iter;

        department->OnVisit(*this);
    }

    return (void*)NULL;
}