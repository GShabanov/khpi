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

#ifndef M_PI
#define M_PI       (float)3.14159265358979323846   // pi
#endif // M_PI

CMathModel::CMathModel()
{
    m_ArrowAngle = glm::radians(90.0f);
    m_CrankAngle = glm::radians(90.0f);
    m_CraneArrowAngle = 0.0f;
    m_RodAngle = 0.0f;

    memset(m_chains, 0, sizeof(m_chains));
}

bool
CMathModel::RecalcAngles(
    float arrowAngle,
    float crankAngle,
    float& rodAngle,
    float& craneArrowAngle
    )
{

    //
    // we are usign CCW angles
    //

    crankAngle = M_PI + crankAngle; // rotated backwards
    arrowAngle = arrowAngle + glm::radians(90.0f);      // the default angle is 90 degres
    //
    // angle difference
    //
    float alfa = crankAngle - arrowAngle;     // angle to BD

    //
    // get crank and arrow ends
    //
    float Bx = m_ArrowLength * cosf(arrowAngle);
    float By = m_ArrowLength * sinf(arrowAngle);

    float Dx = m_crankLength * cosf(crankAngle);
    float Dy = m_crankLength * sinf(crankAngle);

    float BD = sqrtf(powf(Bx - Dx, 2.0f) + powf(By - Dy, 2.0f));
    float BD2 = BD * BD;

    //-----------------------
    // first triangle
    //-----------------------
    float a = (BD2 + m_AB2 - m_AD2) / (2 * BD * m_AB);  // betta1
    float b = (BD2 + m_AD2 - m_AB2) / (2 * BD * m_AD);  // gamma1

    if (a > 1.0 || a < -1.0)
        return false;

    float betta1 = 0.0;
    float gamma1 = 0.0;

    //
    // We calculate the arccosine on the larger side, thus we reduce the calculation error
    //
    if (abs(a) > abs(b))
    {
        betta1 = acosf(a);
        gamma1 = M_PI - (alfa + betta1);
    }
    else
    {
        gamma1 = acosf(b);
        betta1 = M_PI - (alfa + gamma1);
    }

    //-----------------------
    // Second triangle
    //-----------------------
    float gamma2 = 0.0;

    a = (BD2 + m_CD2 - m_BC2) / (2 * BD * m_CD);

    if (a > 1.0 || a < -1.0)
        return false;

    gamma2 = acosf(a);

    float gamma = gamma1 + gamma2;



    float betta2 = 0.0;
    float lambda = 0.0;

    a = (BD2   + m_BC2 - m_CD2) / (2 * BD * m_BC);   // betta2
    b = (m_BC2 + m_CD2 - BD2) / (2 * m_BC * m_CD);   // lambda

    if (a > 1.0 || a < -1.0)
        return false;


    if (abs(a) > abs(b))
    {
        betta2 = acosf(a);
        lambda = M_PI - (gamma2 + betta2);
    }
    else
    {
        lambda = acosf(b);
        betta2 = M_PI - (gamma2 + lambda);
    }


    //
    // Last calculations. We need the outer angles of the triangle.
    //
    float betta = betta1 + betta2;

    craneArrowAngle = M_PI - betta; // we are interested in a complementary angle
    rodAngle = gamma;

    return true;
}


void
CMathModel::UpdateAngles()
{

    float arrowAngle = m_ArrowAngle - glm::radians(90.0f);
    float craneArrowAngle = m_CraneArrowAngle;
    float crankAngle = m_CrankAngle - glm::radians(90.0f);
    float rodAngle = m_RodAngle;
    glm::mat4 arrowMatrix;


    RecalcAngles(arrowAngle, crankAngle, rodAngle, craneArrowAngle);

    //rodAngle += glm::radians(90.0f);
    //
    // arrow matrix
    //
    glm::mat4 matrix = glm::mat4(1.0);
    matrix = glm::rotate(matrix, arrowAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    arrowMatrix = matrix;
    matrix = glm::translate(matrix, glm::vec3(0.0f, 0.0f, 10.0f));
    m_chains[0] = matrix;

    //
    // crane arrow matrix
    //
    matrix = arrowMatrix;
    matrix = glm::translate(matrix, glm::vec3(0.0f, m_ArrowLength, 0.0f));
    matrix = glm::rotate(matrix, -glm::radians(90.0f) + craneArrowAngle, glm::vec3(0.0f, 0.0f, 1.0f));

    m_chains[1] = matrix;


    //
    // crank matrix 1
    //
    glm::mat4 crankMatrix1 = glm::mat4(1.0);
    crankMatrix1 = glm::rotate(crankMatrix1, crankAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    crankMatrix1 = glm::translate(crankMatrix1, glm::vec3(0.0f, 0.0f, 13.0f));
    m_chains[4] = crankMatrix1;


    //
    // crank matrix 2
    //
    glm::mat4 crankMatrix2 = crankMatrix1;
    crankMatrix2 = glm::translate(crankMatrix2, glm::vec3(0.0f, 0.0f, -29.0f));
    m_chains[5] = crankMatrix2;


    //
    // rods
    //    
    glm::mat4 rodMatrix1 = crankMatrix1;
    glm::mat4 rodMatrix2;

    rodMatrix1 = glm::translate(rodMatrix1, glm::vec3(-m_crankLength, 0.0f, 0.0f));
    rodMatrix1 = glm::rotate(rodMatrix1, rodAngle, glm::vec3(0.0f, 0.0f, 1.0f));


    rodMatrix2 = rodMatrix1;
    rodMatrix1 = glm::translate(rodMatrix1, glm::vec3(0.0f, 0.0f, 3.0f));
    m_chains[2] = rodMatrix1;

    rodMatrix2 = glm::translate(rodMatrix2, glm::vec3(0.0f, 0.0f, -32.0f));
    m_chains[3] = rodMatrix2;
}


CMathModel::ForceMomentResult
CMathModel::CalcForceMomentEpure(float loadForce /* F1 */)
{
    ForceMomentResult result{};

    // lengths
    const float AB = m_ArrowLength;   // 95.0f
    const float AD = m_crankLength;   // 25.0f
    const float BC = 29.7f;
    const float BE = 100.0f; //
    const float CD = 104.19f;

    //
    const float AE = AB + BE; // A > B > E
    const float AC = AB - BC;
    const float CE = BE;

    // 
    float A_mm2 = 100.0f; // мм^2
    float A_m2 = A_mm2 * 1e-6f;
    float density = 7850.0f;
    float g = 9.81f;
    float L_str_m = (BC + BE) / 1000.0f;
    float mass = density * A_m2 * L_str_m;
    float weight = mass * g; // 

    float totalLoad = loadForce + weight;

    //
    float r_weight = AB + BE / 2.0f;
    float r_load = AE; //

    //
    float moment_weight = weight * r_weight;
    float moment_load = loadForce * r_load;

    float moment_total = moment_weight + moment_load;

    //
    result.reactionD = moment_total / AD;

    //
    result.reactionA = totalLoad - result.reactionD;

    //
    glm::vec2 pointC = GetPointC(); //
    glm::vec2 pointD = GetPointD(); //

    glm::vec2 rCD = pointC - pointD;
    glm::vec2 F_load(0.0f, -loadForce); // сила вниз

    //
    result.momentAtD = rCD.x * F_load.y - rCD.y * F_load.x;

    //
    glm::vec2 dirCD = glm::normalize(rCD);

    //
    result.forceInCD = glm::dot(F_load, dirCD);

    
    glm::vec2 pointE = pointC + glm::vec2(BE, 0.0f);
    glm::vec2 pointB = GetPointB();
    glm::vec2 rBE = pointE - pointB;
    result.momentAtB = rBE.x * F_load.y - rBE.y * F_load.x;

    return result;
}
