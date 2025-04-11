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


    m_epureData = CalcForceMomentEpure(100.0f);
}

glm::vec2 CMathModel::GetPointInRodLocal(float length)
{
    return glm::vec2(0.0f, length); // если шатун направлен вверх
}


CMathModel::ForceMomentResult
CMathModel::CalcForceMomentEpure(float loadForce, glm::vec3 gravityDir)
{
    ForceMomentResult result{};

    // Geometry constants (in millimeters)
    //const float AB = m_ArrowLength;
    //const float BC = 29.7f;
    //const float CD = 104.19f;
    //const float crankLength = m_crankLength;

    float density = 7850.0f; // Steel density (kg/m³)
    float g = 9.81f; // gravity acceleration (m/s²)

    // Cross-sectional areas
    float upperBeamArea_mm2 = 10.0f * 10.0f; // Upper arm cross-section
    float lowerBeamArea_mm2 = 15.0f * 15.0f; // Lower arm cross-section
    float rodArea_mm2 = 5.0f * 5.0f;         // Rod cross-section
    float crankArea_mm2 = 5.0f * 5.0f;       // Crank cross-section

    // Upper beam weight calculation
    float upperBeamArea_m2 = upperBeamArea_mm2 * 1e-6f;
    float upperBeamLength_m = (m_BC + m_BE) / 1000.0f;
    float upperBeamMass = density * upperBeamArea_m2 * upperBeamLength_m;
    float upperBeamWeight = upperBeamMass * g;

    // Lower beam weight calculation
    float lowerBeamArea_m2 = lowerBeamArea_mm2 * 1e-6f;
    float lowerBeamLength_m = m_AB / 1000.0f;
    float lowerBeamMass = density * lowerBeamArea_m2 * lowerBeamLength_m;
    float lowerBeamWeight = lowerBeamMass * g;

    // Rod weight calculation
    float rodArea_m2 = rodArea_mm2 * 1e-6f;
    float rodLength_m = m_CD / 1000.0f;
    float rodMass = density * rodArea_m2 * rodLength_m;
    float rodWeight = rodMass * g;

    // Crank weight calculation
    float crankArea_m2 = crankArea_mm2 * 1e-6f;
    float crankLength_m = m_crankLength / 1000.0f;
    float crankMass = density * crankArea_m2 * crankLength_m;
    float crankWeight = crankMass * g;

    // World-space forces
    glm::vec3 loadWorld = gravityDir * loadForce;
    glm::vec3 upperBeamWeightWorld = gravityDir * upperBeamWeight;
    glm::vec3 lowerBeamWeightWorld = gravityDir * lowerBeamWeight;
    glm::vec3 rodWeightWorld = gravityDir * rodWeight;
    glm::vec3 crankWeightWorld = gravityDir * crankWeight;

    glm::vec3 totalUpperBeamForceWorld = loadWorld + upperBeamWeightWorld;

    //
    // Force at E (top of upper arm)
    //
    glm::vec3 forceAtELocal = glm::vec3(glm::inverse(m_chains[1]) * glm::vec4(loadWorld, 0.0f));
    result.forceAtE.direction = glm::normalize(forceAtELocal);
    result.forceAtE.magnitude = glm::length(forceAtELocal);

    //
    // Moment at B caused by load at E
    //
    glm::vec2 r_BE_local(0.0f, m_BE);
    result.momentAtB = r_BE_local.x * forceAtELocal.y - r_BE_local.y * forceAtELocal.x;

    //
    // Reaction at E (along the beam towards B)
    //
    glm::vec2 beamDirLocal = glm::normalize(glm::vec2(0.0f, -glm::sign(m_BE)));
    float reactionMagE = glm::dot(-glm::vec2(forceAtELocal), beamDirLocal);

    if (reactionMagE < 0.0f)
    {
        reactionMagE = -reactionMagE;
        beamDirLocal = -beamDirLocal;
    }

    result.reactionAtE.direction = beamDirLocal;
    result.reactionAtE.magnitude = reactionMagE;

    //
    // Reaction at B (base of upper arm)
    //
    glm::vec3 reactionWorldB = -totalUpperBeamForceWorld;
    glm::vec3 reactionBLocalFull = glm::vec3(glm::inverse(m_chains[1]) * glm::vec4(reactionWorldB, 0.0f));

    glm::vec2 armAxisLocal = glm::normalize(glm::vec2(0.0f, 1.0f));
    float reactionMagnitudeB = glm::dot(glm::vec2(reactionBLocalFull), armAxisLocal);

    if (reactionMagnitudeB < 0.0f)
    {
        reactionMagnitudeB = -reactionMagnitudeB;
        armAxisLocal = -armAxisLocal;
    }

    result.reactionAtB.direction = armAxisLocal;
    result.reactionAtB.magnitude = reactionMagnitudeB;

    // Force at B (upper arm force downwards)
    result.forceAtB.direction = -result.reactionAtB.direction;
    result.forceAtB.magnitude = result.reactionAtB.magnitude;

    // --- Force at C1
    glm::vec3 forceC1Local = glm::vec3(glm::inverse(m_chains[4]) * glm::vec4(loadWorld, 0.0f));
    float lenC1 = glm::length(forceC1Local);
    if (lenC1 > 1e-6f) {
        result.forceAtC1.direction = forceC1Local / lenC1;
        result.forceAtC1.magnitude = lenC1;
    }
    else {
        result.forceAtC1.direction = glm::vec2(0.0f);
        result.forceAtC1.magnitude = 0.0f;
    }

    // --- Force at C2
    glm::vec3 forceC2Local = glm::vec3(glm::inverse(m_chains[5]) * glm::vec4(loadWorld, 0.0f));
    float lenC2 = glm::length(forceC2Local);
    if (lenC2 > 1e-6f) {
        result.forceAtC2.direction = forceC2Local / lenC2;
        result.forceAtC2.magnitude = lenC2;
    }
    else {
        result.forceAtC2.direction = glm::vec2(0.0f);
        result.forceAtC2.magnitude = 0.0f;
    }

    // Total load supported by both cranks (D1 + D2)
    glm::vec3 rodsTotalWeightWorld = rodWeightWorld * 2.0f;
    glm::vec3 cranksTotalWeightWorld = crankWeightWorld * 2.0f;

    glm::vec3 reactionWorldD = -(totalUpperBeamForceWorld + rodsTotalWeightWorld + cranksTotalWeightWorld) * 0.5f;

    // --- Reaction at D1 (left crank)
    glm::vec3 reactionD1Local = glm::vec3(glm::inverse(m_chains[2]) * glm::vec4(reactionWorldD, 0.0f));
    float lenD1 = glm::length(reactionD1Local);
    if (lenD1 > 1e-6f) {
        result.reactionAtD1.direction = reactionD1Local / lenD1;
        result.reactionAtD1.magnitude = lenD1;
    }
    else {
        result.reactionAtD1.direction = glm::vec2(0.0f);
        result.reactionAtD1.magnitude = 0.0f;
    }

    // --- Reaction at D2 (right crank)
    glm::vec3 reactionD2Local = glm::vec3(glm::inverse(m_chains[3]) * glm::vec4(reactionWorldD, 0.0f));
    float lenD2 = glm::length(reactionD2Local);

    if (lenD2 > 1e-6f) {
        result.reactionAtD2.direction = reactionD2Local / lenD2;
        result.reactionAtD2.magnitude = lenD2;
    }
    else {
        result.reactionAtD2.direction = glm::vec2(0.0f);
        result.reactionAtD2.magnitude = 0.0f;
    }

    //--- force at D1
    result.forceAtD1.direction = -result.reactionAtD1.direction;
    result.forceAtD1.magnitude = result.reactionAtD1.magnitude;

    //--- force at D2
    result.forceAtD2.direction = -result.reactionAtD2.direction;
    result.forceAtD2.magnitude = result.reactionAtD2.magnitude;

    //--- Reaction at C1
    result.reactionAtC1.direction = -result.forceAtC1.direction;
    result.reactionAtC1.magnitude = result.forceAtC1.magnitude;

    //--- Reaction at C2
    result.reactionAtC2.direction = -result.forceAtC2.direction;
    result.reactionAtC2.magnitude = result.forceAtC2.magnitude;

    //--- force at B
    result.forceAtB.direction = -result.reactionAtB.direction;
    result.forceAtB.magnitude = result.reactionAtB.magnitude;


    //--- force at A
    glm::vec3 totalStructureLoadWorld = -(reactionWorldD * 2.0f + totalUpperBeamForceWorld + rodsTotalWeightWorld + cranksTotalWeightWorld);
    glm::vec3 reactionALocalFull = glm::vec3(glm::inverse(m_chains[0]) * glm::vec4(totalStructureLoadWorld, 0.0f));

    // projection to lower beam axis (local Y)
    glm::vec2 baseAxisLocal = glm::normalize(glm::vec2(0.0f, 1.0f));
    float reactionMagnitudeA = glm::dot(glm::vec2(reactionALocalFull), baseAxisLocal);

    if (reactionMagnitudeA < 0.0f)
    {
        reactionMagnitudeA = -reactionMagnitudeA;
        baseAxisLocal = -baseAxisLocal;
    }

    result.reactionAtA.direction = baseAxisLocal;
    result.reactionAtA.magnitude = reactionMagnitudeA;

    // force At A:
    result.forceAtA.direction = -result.reactionAtA.direction;
    result.forceAtA.magnitude = result.reactionAtA.magnitude;

    return result;
}
