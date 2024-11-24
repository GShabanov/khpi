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
    //
    // arrow matrix
    //
    glm::mat4 matrix = glm::mat4(1.0);
    matrix = glm::rotate(matrix, glm::radians(90.0f) + m_ArrowAngle, glm::vec3(0.0, 0.0, 1.0));
    m_chains[0] = matrix;

    //
    // crane arrow matrix
    //
    matrix = glm::mat4(1.0);
    matrix = glm::rotate(matrix, m_CraneArrowAngle, glm::vec3(0.0, 0.0, 1.0));
    matrix = glm::translate(matrix, glm::vec3(0.0, 95.0f, 0.0));

    m_chains[1] = matrix;


    //
    // crank matrix 1
    //
    glm::mat4 crankMatrix1 = glm::mat4(1.0);
    crankMatrix1 = glm::rotate(crankMatrix1, glm::radians(0.0f) + m_CrankAngle, glm::vec3(0.0, 0.0, 1.0));
    crankMatrix1 = glm::translate(crankMatrix1, glm::vec3(0.0, 0.0f, 12.0f));
    m_chains[4] = crankMatrix1;


    //
    // crank matrix 2
    //
    glm::mat4 crankMatrix2 = crankMatrix1;
    crankMatrix2 = glm::translate(crankMatrix2, glm::vec3(0.0, 0.0f, -29.0f));
    m_chains[5] = crankMatrix2;


    //
    // rods
    //    
    glm::mat4 rodMatrix1 = crankMatrix1;
    rodMatrix1 = glm::translate(rodMatrix1, glm::vec3(-25.96f, 0.0f, 0.0f));
    rodMatrix1 = glm::rotate(rodMatrix1, glm::radians(0.0f) + m_RodAngle, glm::vec3(0.0, 0.0, 1.0));
    rodMatrix1 = glm::translate(rodMatrix1, glm::vec3(0.0, 0.0f, -3.0f));
    m_chains[2] = rodMatrix1;

    glm::mat4 rodMatrix2 = crankMatrix2;
    rodMatrix2 = glm::translate(rodMatrix2, glm::vec3(-25.96f, 0.0f, 0.0f));
    rodMatrix2 = glm::rotate(rodMatrix2, glm::radians(0.0f) + m_RodAngle, glm::vec3(0.0, 0.0, 1.0));
    rodMatrix2 = glm::translate(rodMatrix2, glm::vec3(0.0, 0.0f, 3.0f));
    m_chains[3] = rodMatrix2;
}

