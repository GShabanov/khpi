
#include "pch.h"
#ifndef __TASK1_IMPL_C__
#define __TASK1_IMPL_C__

#include "Task1impl.h"


template <class Type>
bool
CSolver<Type>::Initialize()
{
    //srand(GetTickCount());
    srand(1);

    m_Initialized = false;

    SetDefaults();

    return true;
}


template <class Type>
void
CSolver<Type>::BuildGraph(Type from, Type to, Type epsilon)
{

    // 1.  Знайти корені системи нелінійних рівнянь, що розташовані в першому квадранті з точністю ε = 1е ^ - 5 методом Ньютона
    // 
    // {
    // {  x1^2 + x2^2 - 32 = 0
    // {  x1^3 - x2        = 0 
    // {
    // 
    // перше рівняння
    // 
    // x1 = sqrt(32 - x2 ^ 2);   | область значень 32 - x2^2 >= 0  або x2 <= 5,65
    // x2 = sqrt(32 - x1 ^ 2);   | область значень 32 - x2^2 >= 0  або x2 <= 5,65
    //
    // друге рівняння
    // x1 = sqrt3(x2);
    // x2 = x1^3
    //



    Type x1 = 0.0;
    Type inc = (to - from) / ARRAYSIZE(m_Function1Data);     // обираємо шаг для малювання

    x1 = from;

    for (size_t i = 0; i < ARRAYSIZE(m_Function1Data); i++)
    {

        Type x2 = sqrt(32 - x1 * x1);

        m_Function1Data[i].x1 = x1;
        m_Function1Data[i].x2 = x2;
        m_Function1Data[i].type = 0;

        x1 += inc;
    }

    x1 = from;

    for (size_t i = 0; i < ARRAYSIZE(m_Function2Data); i++)
    {

        Type x2 = x1 * x1 * x1;

        m_Function2Data[i].x1 = x1;
        m_Function2Data[i].x2 = x2;
        m_Function2Data[i].type = 0;

        x1 += inc;
    }

    std::vector<double>  x;
    std::vector<double>  x_new;
    x.push_back(1);
    x.push_back(1);

    //element.x1 = x[0];
    //element.x2 = x[1];

    //m_Iterations[0].x1 = x[0];
    //m_Iterations[0].x2 = x[1];

    for ( ; ; )
    {
        POINT_DATA  element;

        x_new = this->CalcIteration(x);


        Type  dx1 = (x[0] - x_new[0]) * (x[0] - x_new[0]);
        Type  dx2 = (x[1] - x_new[1]) * (x[1] - x_new[1]);

        //
        // ми не можемо обчислити норму так як різниця стала нулем
        //
        if (0 == dx1 || 0 == dx2)
            break;

        dx1 = sqrt(dx1);
        dx2 = sqrt(dx2);

        if (dx1 > dx2)
        {
            if (dx1 < epsilon)
                break;
        }
        else
        {
            if (dx2 < epsilon)
                break;
        }

        element.x1 = dx1;
        element.x2 = dx2;

        m_Error.Add(element);

        element.x1 = x_new[0];
        element.x2 = x_new[1];

        m_Iterations.Add(element);

        x = x_new;

    }

    m_x1 = x_new[0];
    m_x2 = x_new[1];

    m_Initialized = true;
}

template <class Type>
std::vector<Type>
CSolver<Type>::CalcIteration(std::vector<Type> &x)
{
    //
    // обираємо 
    // 
    // { f1 = x2 = sqrt(32 - x1 ^ 2);
    // { f2 = x2 = x1^3
    // 
    //
    //          { dF1/dx1  dF1/dx2 }  
    //  J(x) =  {                  }
    //          { dF2/dx1  dF2/dx2 }
    //
    //  dF1/dx1 = 2*x1
    //  dF1/dx2 = 2*x2
    //  dF2/dx1 = 3*x1^2
    //  dF2/dx2 = -1
    //
    CMatrix<Type>   Jacobian;


    Jacobian(0, 0) = 2 * x[0];
    Jacobian(0, 1) = 2 * x[1];
    Jacobian(1, 0) = 3 * x[0] * x[0];
    Jacobian(1, 1) = -1;

    CMatrix<Type>  JacobianInv;

    //
    //  J(x)^-1
    //
    JacobianInv = Jacobian.inverse();

    //          { x1^2 + x2^2 - 32 }  
    //  F(x) =  {                  }
    //          { x1^3 - x2        }
    //
    //
    std::vector<Type>  Fv1;
    std::vector<Type>  Fv2;

    //
    // rows vector to form a vertical matrix
    //
    Fv1.push_back(x[0] * x[0] + x[1] * x[1] - 32);
    Fv2.push_back(x[0] * x[0] * x[0] - x[1]);

    CMatrix<Type>      F(2, 1, 0.0);

    F(0) = Fv1;
    F(1) = Fv2;

    CMatrix<Type>  nextIteration = JacobianInv * F;

    std::vector<Type> x1;

    //nextIteration.dump();

    x1.push_back(x[0] - nextIteration(0, 0));
    x1.push_back(x[1] - nextIteration(1, 0));

    return x1;
}


template <class Type>
void
CSolver<Type>::SetDefaults()
{

    m_Error.RemoveAll();
    m_Iterations.RemoveAll();


    for (size_t i = 0; i < ARRAYSIZE(m_Function1Data); i++)
    {
        m_Function1Data[i].x1 = 0;
        m_Function1Data[i].x2 = 0;
        m_Function1Data[i].type = 0;
    }

    for (size_t i = 0; i < ARRAYSIZE(m_Function2Data); i++)
    {
        m_Function2Data[i].x1 = 0;
        m_Function2Data[i].x2 = 0;
        m_Function2Data[i].type = 0;
    }

    //m_epsilon = 1.0 / 1000;

    m_x1 = 0.0;
    m_x2 = 0.0;
}




#endif // __TASK1_IMPL_C__