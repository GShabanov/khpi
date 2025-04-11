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
CVector::setup(glm::vec3& start, glm::vec3& direction, float length, glm::vec4& color)
{
    BOOL   _return = FALSE;
    VBO_STRUCT  data;

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    data.origin = start;
    data.direction = direction;
    data.color = color;
    data.length = length;


    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);

    // layout(location = 0) vec3 start;
    // layout(location = 1) vec3 direction;
    // layout(location = 2) vec4  color;
    // layout(location = 3) float length;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(data), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(data), (void*)FIELD_OFFSET(VBO_STRUCT, direction));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(data), (void*)FIELD_OFFSET(VBO_STRUCT, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(data), (void*)FIELD_OFFSET(VBO_STRUCT, length));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    _return = TRUE;

    if (_return)
    {
        m_initialized = TRUE;
    }

    return _return;
}

BOOL
CVector::update(glm::vec3& start, glm::vec3& direction, float length)
{

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    VBO_STRUCT* data = (VBO_STRUCT*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

    if (data != NULL)
    {
        data->origin = start;
        data->direction = direction;
        data->length = length;

        // release buffer
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return true;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return false;
}


void
CVector::Draw(glm::mat4& transform, class CShader& vectorShader)
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
