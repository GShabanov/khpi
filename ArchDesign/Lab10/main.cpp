
#include <windows.h>
#include <tchar.h>
#include <string>
#include <list>
#include <iostream>

#include "HtmlElement.h"
#include "HtmlSelect.h"
#include "HtmlInput.h"
#include "HtmlFieldset.h"
#include "HtmlPage.h"


int
_tmain(int argc, TCHAR* argv[])
{

    //
    // перший FieldSet  ім'я та побатькові 
    //
        CHTMLElement *input1 = new CHtmlInput(std::string("firstName"));
        CHTMLElement *input2 = new CHtmlInput(std::string("lastName"));

    std::list<CHTMLElement*> workerInfo;

    workerInfo.push_back(input1);
    workerInfo.push_back(input2);

    CHtmlFieldset *workerfieldSet = new CHtmlFieldset(std::string("worker"), workerInfo);

    //
    // другий FieldSet  пол, зарплатня
    //
            std::vector<std::string>  genderVector;

            genderVector.push_back(std::string("male"));
            genderVector.push_back(std::string("female"));

        CHtmlSelect *gender = new CHtmlSelect(std::string("gender"), genderVector);

            std::vector<std::string>  salaryVector;

            salaryVector.push_back(std::string("100"));
            salaryVector.push_back(std::string("200"));
            salaryVector.push_back(std::string("300"));
            salaryVector.push_back(std::string("400"));
            salaryVector.push_back(std::string("400+"));

        CHtmlSelect *salary = new CHtmlSelect(std::string("salary"), salaryVector);

    std::list<CHTMLElement*> workerDetail;

    workerDetail.push_back(gender);
    workerDetail.push_back(salary);

    CHtmlFieldset* workerInfoFieldSet = new CHtmlFieldset(std::string("worker"), workerDetail);


    //
    // сторінка з двох fieldset
    //
    std::list<CHTMLElement*> htmlInfo;

    htmlInfo.push_back(workerfieldSet);
    htmlInfo.push_back(workerInfoFieldSet);



    CHtmlPage* htmlPage = new CHtmlPage(htmlInfo);


    //
    // будуємо всю сторінку
    //
    std::string  userFormHtml = htmlPage->GetData();


    std::cout << "output html" << std::endl;
    std::cout << userFormHtml;

    //
    // програма виводить
    //

    //<html>
    //<fieldset id = worker>
    //<input id = firstName/>
    //<input id = lastName/>
    //</fieldset>
    //<fieldset id = worker>
    //<select id = gender>
    //    <option>male</option>
    //    <option>female</option>
    //</select>
    //<select id = salary>
    //    <option>100 </option>
    //    <option>200 </option>
    //    <option>300 </option>
    //    <option>400 </option>
    //    <option>400+ </option>
    //</select>
    //</fieldset>
    //</html>


    delete htmlPage;

    return 0;
}

