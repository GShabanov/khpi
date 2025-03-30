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

public:
    CVector(class CLogCallback* logcallback)
    {
        m_log = logcallback;
        m_initialized = false;

        m_vao = 0;
        m_vbo = 0;
    }

    virtual ~CVector();


    BOOL setup(glm::vec3& start, glm::vec3& direction, float length);

    // Call draw function of all meshes in m_meshes
    void Draw(glm::mat4 transform, class CShader& spriteShader);

};

#endif // __OPENGL_VECTOR_H__
