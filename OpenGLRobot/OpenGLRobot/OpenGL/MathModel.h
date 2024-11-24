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

    glm::mat4   m_chains[6];

public:

    CMathModel()
    {
        m_ArrowAngle = 0.0f;
        m_CrankAngle = 0.2f;
        m_CraneArrowAngle = 0.0f;
        m_RodAngle = 0.0f;

        memset(m_chains, 0, sizeof(m_chains));
    }

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
