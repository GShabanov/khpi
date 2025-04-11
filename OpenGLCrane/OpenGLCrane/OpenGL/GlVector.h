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

#pragma once
#ifndef __OPENGL_VECTOR_H__
#define __OPENGL_VECTOR_H__


class CVector
{
private:
    class CLogCallback* m_log;

    GLuint                      m_vao;
    GLuint                      m_vbo;

    bool                        m_initialized;

    typedef struct _VBO_STRUCT {
        glm::vec3  origin;
        glm::vec3  direction;
        glm::vec4  color;
        float      length;

    } VBO_STRUCT;

public:
    CVector(class CLogCallback* logcallback)
    {
        m_log = logcallback;
        m_initialized = false;

        m_vao = 0;
        m_vbo = 0;
    }

    virtual ~CVector();

    BOOL update(glm::vec3& start, glm::vec3& direction, float length);

    BOOL setup(glm::vec3& start, glm::vec3& direction, float length, glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Call draw function of all meshes in m_meshes
    void Draw(glm::mat4& transform, class CShader& spriteShader);

};

#endif // __OPENGL_VECTOR_H__
