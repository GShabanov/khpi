#pragma once

#ifndef __TASK1_IMPL_H__
#define __TASK1_IMPL_H__

#include "..\..\Shared\matrix.h"

template <class Type>
class  CSolver
{

    typedef struct _POINT_DATA {
        Type       x1;
        Type       x2;

        bool       type;
    } POINT_DATA, * PPOINT_DATA;

    bool            m_Initialized;

    std::vector<Type>  CalcIteration(std::vector<Type>& x);

public:

    //--------------------------------------------------------------
    // accessors
    //--------------------------------------------------------------


    //
    // graph
    //
    POINT_DATA        m_Function1Data[1000];
    POINT_DATA        m_Function2Data[1000];
    CArray<POINT_DATA>  m_Iterations;
    CArray<POINT_DATA>  m_Error;

    Type              m_x1;
    Type              m_x2;

    const Type        m_epsilonMax = 0.00001;  // максимальний епсілон



    CSolver<Type>()
    {
        m_Initialized = false;
    }

    ~CSolver()
    {
        m_Error.RemoveAll();
    }

    bool   IsInitialized() {

        return m_Initialized;
    }

    void   BuildGraph(Type from, Type to, Type epsilon);

    bool   CheckValue(Type x1, Type x2);

    bool   Initialize();
    void   SetDefaults();



};

#include "Task1impl.cpp"

#endif // __TASK1_IMPL_H__
