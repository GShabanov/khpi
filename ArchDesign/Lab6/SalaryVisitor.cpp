
#include <string>
#include <list>
#include <iostream>

#include "Visitor.h"
#include "Employee.h"
#include "Department.h"
#include "Company.h"


void*
CSalaryReport::VisitEmployee(CEmployee& employee)
{
    std::cout << "[*]        Employee: " << employee.GetName() << ", salary: " << employee.GetSalary() << std::endl;

    return (void*)(long)employee.GetSalary();
}

void*
CSalaryReport::VisitDepartment(CDepartment& department)
{
    unsigned int salaryInDepartment = 0;

    std::list<CEmployee*>::iterator Iter;

    std::cout << "[*]    Department: " << department.GetName() << std::endl;


    for (Iter = department.GetEmployeesList().begin(); Iter != department.GetEmployeesList().end(); Iter++)
    {
        CEmployee* employee = *Iter;

        salaryInDepartment += (unsigned int)(long)employee->OnVisit(*this);
    }

    return (void *)(long)salaryInDepartment;
}

void*
CSalaryReport::VisitCompany(CCompany& company)
{
    unsigned int salaryInCompany = 0;

    std::list<CDepartment *>::iterator Iter;

    std::cout << "[*] Company: " << company.GetName() << std::endl;


    for (Iter = company.GetDepartmentList().begin(); Iter != company.GetDepartmentList().end(); Iter++)
    {
        CDepartment* department = *Iter;

        salaryInCompany += (unsigned int)(long)department->OnVisit(*this);
    }

    return (void*)(long)salaryInCompany;
}