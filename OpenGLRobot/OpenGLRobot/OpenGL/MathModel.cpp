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
#include "pch.h"

#include "MathModel.h"


void
CMathModel::UpdateAngles()
{
    glm::mat4 matrix = glm::mat4(1.0);

    matrix = glm::rotate(matrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    m_chains[0] = matrix;

    matrix = glm::mat4(1.0);
    //
    // calc kinematic chain 1 matrix
    //

    matrix = glm::rotate(matrix, m_angle1, glm::vec3(0.0, 1.0, 0.0));
    matrix = glm::translate(matrix, glm::vec3(0.0, 95.0f, 0.0));

    m_chains[1] = matrix;
    //chain[1] = matrix;
}


glm::mat4
CMathModel::getArrowMatrix()
{
    return m_chains[0];
}

glm::mat4
CMathModel::getCraneMatrix()
{
    return m_chains[1];
}
