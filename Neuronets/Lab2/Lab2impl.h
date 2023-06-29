#pragma once

#ifndef __LAB2_IMPL_H__
#define __LAB2_IMPL_H__


template <class Type>
class  CSolver
{


    typedef struct _RULES {
        int  phiRule;
        int  omegaRule;
        int  muRule;
    } RULES;

    static RULES   m_rulesArray[];

    Type   m_angle;
    Type   m_velocity;
    Type   m_maxTime;


    Type GetMembershipFunction(Type x, Type a, Type b, Type c, Type d);

    Type GetTestMembershipFunction(int term, Type x);

    Type GetGeneralMembershipFunction(Type phi, Type omega, Type mu);

public:

    class CUpdateCallback
    {
    public:
        virtual bool UpdateCallback(Type t, Type phi, Type omega, Type mu) = 0;
    };

    Type  GetAngle();

    Type AngularVelocity();

    //
    // training data
    //

    ~CSolver()
    {
    }

    bool   Initialize(Type startAngle = 0, Type startVelocity = 0, Type maxTime = 50);

    void   StabilizeAtValue(Type  degree = 0.0, CUpdateCallback* callbackClass = NULL);


};

template <class Type>
typename CSolver<Type>::RULES   CSolver<Type>::m_rulesArray[] = {
   { 1, 1, 6 },
   { 1, 2, 6 },
   { 1, 3, 6 },
   { 1, 4, 6 },
   { 1, 5, 6 },
   { 1, 6, 6 },
   { 2, 1, 6 },
   { 2, 2, 6 },
   { 2, 3, 6 },
   { 2, 4, 5 },
   { 2, 5, 5 },
   { 2, 6, 5 },
   { 3, 1, 6 },
   { 3, 2, 6 },
   { 3, 3, 6 },
   { 3, 4, 5 },
   { 3, 5, 5 },
   { 3, 6, 5 },
   { 4, 1, 2 },
   { 4, 2, 2 },
   { 4, 3, 2 },
   { 4, 4, 1 },
   { 4, 5, 1 },
   { 4, 6, 1 },
   { 5, 1, 2 },
   { 5, 2, 2 },
   { 5, 3, 2 },
   { 5, 4, 1 },
   { 5, 5, 1 },
   { 5, 6, 1 },
   { 6, 1, 1 },
   { 6, 2, 1 },
   { 6, 3, 1 },
   { 6, 4, 1 },
   { 6, 5, 1 },
   { 6, 6, 1 }
};


#include "Lab2impl.cpp"

#endif // __LAB2_IMPL_H__
