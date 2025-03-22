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
    PVOID                       m_textureData;

    bool                        m_initialized;


    BOOL InitTextureBuffer(CRect& size, GLuint* id);
    BOOL setupTexturedRect(CRect& size);

public:
    CSprite(class CLogCallback* logcallback)
    {
        m_log = logcallback;
        m_initialized = false;

        m_vao = 0;
        m_vbo = 0;

        m_Texture = 0;
        m_textureData = NULL;
    }

    ~CSprite();


    BOOL setup(CRect& size, LPCTSTR type);

    // Call draw function of all meshes in m_meshes
    void Draw(glm::mat4 transform, class CShader& spriteShader);

};

#endif // __OPENGL_SPRITE_H__
