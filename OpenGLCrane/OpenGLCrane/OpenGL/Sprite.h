/***************************************************************************************\
*   File:                                                                               *
*       Sprite.h                                                                        *
*                                                                                       *
*   Abstract:                                                                           *
*       Sprite drawing implementation                                                   *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    28-Mar-2025                                                      *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/

#pragma once
#ifndef __OPENGL_SPRITE_H__
#define __OPENGL_SPRITE_H__


class CSprite
{
private:
    class CLogCallback* m_log;

    GLuint                      m_Texture;

    GLuint                      m_vao;
    GLuint                      m_vbo;
    PVOID                       m_defaultData;
    POINT                       m_dimensions;

    bool                        m_initialized;


    BOOL InitTextureBuffer(GLuint* id);
    BOOL setupRenderingObject(glm::vec2& imageSize);

public:
    CSprite(class CLogCallback* logcallback)
    {
        m_log = logcallback;
        m_initialized = false;

        m_vao = 0;
        m_vbo = 0;

        m_Texture = 0;
        m_defaultData = NULL;
        m_dimensions.x = 64;
        m_dimensions.y = 64;
    }

    virtual ~CSprite();

    void UpdateData(PVOID data, POINT& textureSize);


    BOOL setup(glm::vec2& imageSize, POINT& textureSize);

    // Call draw function of all meshes in m_meshes
    void Draw(glm::mat4 transform, class CShader& spriteShader);

};

#endif // __OPENGL_SPRITE_H__
