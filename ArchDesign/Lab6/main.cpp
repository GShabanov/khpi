
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>

#include "Visitor.h"
#include "Employee.h"
#include "Department.h"
#include "Company.h"

int
_tmain(int argc, TCHAR* argv[])
{
    std::list<CDepartment*>  departments;
    std::list<CEmployee*>  employes[3];

    //
    // Design department 
    //
    employes[0].push_back(new CEmployee("Sam", 500));
    employes[0].push_back(new CEmployee("Bill", 700));
    employes[0].push_back(new CEmployee("Cindy", 400));

    departments.push_back(new CDepartment("Design", employes[0]));


    //
    // Beta-testers department 
    //
    employes[1].push_back(new CEmployee("Lucy", 500));
    employes[1].push_back(new CEmployee("Vanessa", 400));
    employes[1].push_back(new CEmployee("Chriss", 400));

    departments.push_back(new CDepartment("Beta Testers", employes[1]));


    //
    // Programers department 
    //
    employes[2].push_back(new CEmployee("Anthony", 800));
    employes[2].push_back(new CEmployee("Luc", 900));
    employes[2].push_back(new CEmployee("Bob", 850));

    departments.push_back(new CDepartment("Testers", employes[2]));


    //
    // Programers company
    //
    CCompany* my_company = new CCompany("My Best Company", departments);


    IVisitor* salaryVisitor = new CSalaryReport();
    IVisitor* nameVisitor = new CNameReport();


    std::cout << "--------------------------------------------------" << std::endl;
    //
    // Get salary report for company
    //
    unsigned int salary = (unsigned int)my_company->OnVisit(*salaryVisitor);

    std::cout << "Company Salary sum: " << salary << std::endl << std::endl;

    std::cout << "--------------------------------------------------" << std::endl;

    my_company->OnVisit(*nameVisitor);


    //
    // Програма "обходить" переліки в середині компанії
    // В першому випадку за допомоги CSalaryReport і рахує зарплатню
    // В другому випадку за допомоги CNameReport видає перелік робітників
    // 

    // 
    // [*] Company: My Best Company
    // [*]    Department : Design
    // [*]        Employee : Sam, salary : 500
    // [*]        Employee : Bill, salary : 700
    // [*]        Employee : Cindy, salary : 400
    // [*]    Department : Beta Testers
    // [*]        Employee : Lucy, salary : 500
    // [*]        Employee : Vanessa, salary : 400
    // [*]        Employee : Chriss, salary : 400
    // [*]    Department : Testers
    // [*]        Employee : Anthony, salary : 800
    // [*]        Employee : Luc, salary : 900
    // [*]        Employee : Bob, salary : 850
    // Company Salary sum : 5450
    //
    // --------------------------------------------------
    // [*] Company : My Best Company
    // [*]    Department : Design
    // [*]        Employee : Sam
    // [*]        Employee : Bill
    // [*]        Employee : Cindy
    // [*]    Department : Beta Testers
    // [*]        Employee : Lucy
    // [*]        Employee : Vanessa
    // [*]        Employee : Chriss
    // [*]    Department : Testers
    // [*]        Employee : Anthony
    // [*]        Employee : Luc
    // [*]        Employee : Bob
    //

    return 0;
}