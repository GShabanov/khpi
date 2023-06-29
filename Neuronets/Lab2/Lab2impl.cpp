
#include "pch.h"
#ifndef __LAB2_IMPL_C__
#define __LAB2_IMPL_C__

#include "Lab2impl.h"

#include <windows.h>
#define _USE_MATH_DEFINES // for C++
#include <math.h>
#include <assert.h>


template <class Type>
bool
CSolver<Type>::Initialize(Type startAngle, Type startVelocity, Type maxTime)
{

    m_angle = startAngle;
    m_velocity = startVelocity;
    m_maxTime = maxTime;


    return true;
}



template <class Type>
Type
CSolver<Type>::GetMembershipFunction(Type x, Type a, Type b, Type c, Type d)
{
    if (x <= a)
    {
        return 0;
    }
    else if ((a <= x) && (x <= b))
    {
        assert((b - a) > 0);

        if ((b - a) <= 0)
            return 1;

        return (x - a) / (b - a);
    }
    else if ((b <= x) && (x <= c))
    {
        return 1;
    }
    else if ((c <= x) && (x <= d))
    {
        assert((d - c) > 0);

        if ((d - c) <= 0)
            return 1;

        return (d - x) / (d - c);
    }
    else
    {
        return 0;
    }
}


template <class Type>
Type
CSolver<Type>::GetTestMembershipFunction(int term, Type x)
{
    switch (term)
    {
    case 1:
        return GetMembershipFunction(x, -100, -100, -100, -50);
    case 2:
        return GetMembershipFunction(x, -100, -50, -50, -10);
    case 3:
        return GetMembershipFunction(x, -50, -10, 0, 0);
    case 4:
        return GetMembershipFunction(x, 0, 0, 10, 50);
    case 5:
        return GetMembershipFunction(x, 10, 50, 50, 100);
    case 6:
        return GetMembershipFunction(x, 50, 100, 100, 100);

    default:;
    }

    return 0;
}

template <class Type>
Type
CSolver<Type>::GetGeneralMembershipFunction(Type phi, Type omega, Type mu)
{
    Type xi_dash = 0;

    for (size_t i = 0; i < RTL_NUMBER_OF_V1(m_rulesArray); i++)
    {
        Type alfa = GetTestMembershipFunction(m_rulesArray[i].phiRule, phi);
        Type beta = GetTestMembershipFunction(m_rulesArray[i].omegaRule, omega);

        Type gama = min(alfa, beta);

        Type delta = GetTestMembershipFunction(m_rulesArray[i].muRule, mu);

        Type xi = min(gama, delta);

        if (xi_dash < xi)
        {
            xi_dash = xi;
        }
    }

    return xi_dash;
}

template <class Type>
Type
CSolver<Type>::GetAngle() {  // angle in radians

    return m_angle * M_PI / 180;
}

template <class Type>
Type
AngularVelocity() {

    return m_velocity * M_PI / 180;
}


template <class Type>
void
CSolver<Type>::StabilizeAtValue(Type  degree, CUpdateCallback* callbackClass)
{

    Type t = 0;
    Type t_max = m_maxTime;
    Type dt = 0.1;
    Type dmu = 1;

    Type phi = m_angle * M_PI / 180;
    Type omega = m_velocity * M_PI / 180;
    Type phi_min = -M_PI;
    Type phi_max = M_PI;
    Type omega_min = -M_PI / 9;
    Type omega_max = M_PI / 9;
    Type mu_min = -M_PI / 36;
    Type mu_max = M_PI / 36;


    do
    {

        Type phi_ = 200 * (phi - phi_min) / (phi_max - phi_min) - 100;
        Type omega_ = 200 * (omega - omega_min) / (omega_max - omega_min) - 100;
        Type mu_ = -100;
        Type s1 = 0;
        Type s2 = 0;

        do
        {
            Type xi_dash = GetGeneralMembershipFunction(phi_, omega_, mu_);
            s1 = s1 + mu_ * xi_dash * dmu;
            s2 = s2 + xi_dash * dmu;

            mu_ = mu_ + dmu;

        }
        while (mu_ < (100 - dmu * 0.5));

        Type mu_dash = s1 / s2;
        Type mu = (mu_dash + 100) * (mu_max - mu_min) / 200 + mu_min;

        phi = phi + omega * dt;
        omega = omega + mu * dt;

        if (callbackClass != NULL)
        {
            bool _continue = callbackClass->UpdateCallback(t, phi, omega, mu);

            if (!_continue)
                break;
        }

        t = t + dt;

    }
    while (t < (t_max - dt * 0.5));

}
#endif // __LAB2_IMPL_C__