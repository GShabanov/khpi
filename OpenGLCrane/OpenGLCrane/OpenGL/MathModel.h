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
    struct ForceMomentResult
    {
        float reactionA;       // Вертикальная реакция в A
        float reactionD;       // Вертикальная реакция в D
        float momentAtD;       // Момент силы в точке D
        float momentAtB;       // Момент в точке B от нагрузки
        float forceInCD;       // Продольная сила в штанге CD
    };


    float   m_ArrowAngle;
    float   m_CrankAngle;
    float   m_CraneArrowAngle;
    float   m_RodAngle;

    const float  m_ArrowLength = 95.0f;
    const float  m_crankLength = 25.0f;

    const float m_AB = m_ArrowLength;
    const float m_AB2 = m_AB * m_AB;
    const float m_AD = m_crankLength;
    const float m_AD2 = m_AD * m_AD;
    const float m_BC = 29.7f;
    const float m_BC2 = m_BC * m_BC;
    const float m_CD = 104.19f;
    const float m_CD2 = m_CD * m_CD;

    glm::mat4   m_chains[6];

    bool RecalcAngles(float arrowAngle, float crankAngle, float& rodAngle, float& craneArrowAngle);

    glm::vec2 CMathModel::GetPointC()
    {
        glm::vec4 world = m_chains[1] * glm::vec4(0, m_BC, 0, 1);
        return glm::vec2(world.x, world.y);
    }

    glm::vec2 CMathModel::GetPointD()
    {
        glm::vec4 world = m_chains[4] * glm::vec4(-m_crankLength, 0, 0, 1);
        return glm::vec2(world.x, world.y);
    }

    glm::vec2 CMathModel::GetPointB()
    {
        glm::vec4 world = m_chains[0] * glm::vec4(0, m_ArrowLength, 0, 1);
        return glm::vec2(world.x, world.y);
    }
public:

    CMathModel();

    void UpdateAngles();

    ForceMomentResult CalcForceMomentEpure(float loadForce /* F1 */);


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
