/***************************************************************************************\
*   File:                                                                               *
*       MathModel.h                                                                     *
*                                                                                       *
*   Abstract:                                                                           *
*       Mathematical model implementation                                               *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    7-Sep-2024                                                      *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#pragma once
#ifndef __OPENGL_MATHMODEL_H__
#define __OPENGL_MATHMODEL_H__

#include "GlUtils.h"

class CMathModel
{
private:

    float   m_ArrowAngle;
    float   m_CrankAngle;
    float   m_CraneArrowAngle;
    float   m_RodAngle;

    const float  m_ArrowLength = 95.0f;
    const float  m_crankLength = 25.0f;

    glm::mat4   m_chains[6];

public:

    CMathModel();

    void UpdateAngles();


    void setArrowAngle(float angle)
    {
        m_ArrowAngle = angle;
        UpdateAngles();
    }

    void setCrankAngle(float angle)
    {
        m_CrankAngle = angle;
        UpdateAngles();
    }

    glm::mat4 &getArrowMatrix()
    {
        return m_chains[0];
    }

    glm::mat4 &getCraneMatrix()
    {
        return m_chains[1];
    }

    glm::mat4& getRod1Matrix()
    {
        return m_chains[2];
    }

    glm::mat4& getRod2Matrix()
    {
        return m_chains[3];
    }

    glm::mat4& getCrank1Matrix()
    {
        return m_chains[4];
    }

    glm::mat4& getCrank2Matrix()
    {
        return m_chains[5];
    }

};

#endif // __OPENGL_MATHMODEL_H__
