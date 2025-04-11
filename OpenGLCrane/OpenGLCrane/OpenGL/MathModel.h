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
public:
    struct ForceVector
    {
        glm::vec2 direction; // normalized local-space direction
        float magnitude;     // in Newtons
    };

    struct ForceMomentResult
    {
        // Moments at joints
        float momentAtA;
        float momentAtB;
        float momentAtC1;
        float momentAtC2;
        float momentAtD1;
        float momentAtD2;

        // Axial forces in rods
        float axialForceRod1;
        float axialForceRod2;

        // External forces at key points
        ForceVector forceAtE;
        ForceVector reactionAtE; // Beam reaction at point E

        ForceVector forceAtC1;
        ForceVector reactionAtC1;

        ForceVector forceAtC2;
        ForceVector reactionAtC2;


        // Reaction forces at joints and supports
        ForceVector forceAtD1;
        ForceVector reactionAtD1;
        ForceVector forceAtD2;
        ForceVector reactionAtD2;

        ForceVector forceAtB;
        ForceVector reactionAtB;


        ForceVector reactionAtA;   //
        ForceVector forceAtA;      //
    };

private:
    float   m_ArrowAngle;
    float   m_CrankAngle;
    float   m_CraneArrowAngle;
    float   m_RodAngle;

    const float  m_ArrowLength = 95.0f;
    const float  m_crankLength = 25.0f;

    const float  m_BE = 100.0f;


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
    glm::vec2 GetPointInRodLocal(float length);

public:

    CMathModel();

    void UpdateAngles();

    ForceMomentResult m_epureData;

    ForceMomentResult CalcForceMomentEpure(float loadForce, glm::vec3 gravityDir = glm::vec3(0.0f, -1.0f, 0.0f));


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
