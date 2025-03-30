/***************************************************************************************\
*   File:                                                                               *
*       GlVector.h                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       Vector arrow drawing implementation                                             *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    28-Mar-2025                                                     *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"
#include "shader.h"
#include "GlVector.h"


CVector::~CVector()
{

    if (m_initialized)
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }
}

// Load texture
BOOL
CVector::setup(glm::vec3& start, glm::vec3& direction, float length)
{
    BOOL   _return = FALSE;

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    float data[] = {
        start.x, start.y, start.z,
        direction.x, direction.y, direction.z,
        length
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    // layout(location = 0) vec3 start;
    // layout(location = 1) vec3 direction;
    // layout(location = 2) float length;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    _return = TRUE;

    if (_return)
    {
        m_initialized = TRUE;
    }

    return _return;
}

void
CVector::Draw(glm::mat4 transform, class CShader& vectorShader)
{
    if (vectorShader.get_id() == 0)
        return;
    //
    // draw zero
    //
    vectorShader.use();
    vectorShader.setMat4("model", transform);

    GLboolean cullface = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    glBindVertexArray(m_vao);


    glDrawArrays(GL_POINTS, 0, 1);

    glBindVertexArray(0);

    cullface ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

}
