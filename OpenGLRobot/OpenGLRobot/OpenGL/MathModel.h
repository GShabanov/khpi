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

    float   m_angle1;
    float   m_angle2;

    glm::mat4   m_chains[4];

public:

    CMathModel()
    {
        m_angle1 = 0.0f;

        m_angle2 = -0.2f;
    }

    void UpdateAngles();


    glm::mat4 getArrowMatrix();
    glm::mat4 getCraneMatrix();


};

#endif // __OPENGL_MATHMODEL_H__
