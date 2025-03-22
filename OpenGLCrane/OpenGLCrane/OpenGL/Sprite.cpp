#include "pch.h"
#include "shader.h"
#include "Sprite.h"

#include "glm/gtx/normal.hpp"


CSprite::~CSprite()
{

    if (m_Texture != 0)
    {
        glDeleteTextures(1, &m_Texture);

        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);

    }

    if (m_initialized)
    {
        free(m_textureData);
    }
}

BOOL
CSprite::InitTextureBuffer(CRect& size, GLuint* id)
{

    GLuint texture;
    //
    // generate texture
    //
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.Width(), size.Height(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, m_textureData);

    *id = texture;

    return TRUE;
}

BOOL
CSprite::setupTexturedRect(CRect &size)
{
    //-halfSize, -0.1f, halfSize, 0.0f, 1.0f, // Top-left
    //halfSize, -0.1f, halfSize, 1.0f, 1.0f, // Top-right
    //-halfSize, -0.1f, -halfSize, 0.0f, 0.0f, // Bottom-left
    //halfSize, -0.1f, -halfSize, 1.0f, 0.0f  // Bottom-right

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // texture coords
        0.0f,                (float)size.Height(), 0.0f,  0.0f,        0.0f,   // top left
        (float)size.Width(), (float)size.Height(), 0.0f,  (float)1.0f, 0.0f,   // top right
        0.0f,                0.0f,                 0.0f,  0.0f, (float)1.0f,   // bottom left
        (float)size.Width(), 0.0f,                 0.0f,  (float)1.0f, (float)1.0f,   // bottom right
    };


    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // texture coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);


    return TRUE;
}

// Load texture
BOOL
CSprite::setup(CRect& size, LPCTSTR type)
{
    BOOL   _return = FALSE;
    SIZE_T imageDataSize;

    imageDataSize = size.Width() * size.Height() * sizeof(DWORD);

    m_textureData = malloc(imageDataSize);

    if (!m_textureData)
        return FALSE;

    memset(m_textureData, 0xFF, imageDataSize);

    unsigned char* buffer = (unsigned char*)m_textureData;

    for (unsigned int j = 0; j < size.Height(); ++j) {

        for (unsigned int i = 0; i < size.Width(); ++i) {

            size_t index = j * size.Width() + i;

            buffer[4 * index + 0] = 0xFF;
            buffer[4 * index + 1] = 0xFF;
            buffer[4 * index + 2] = 0x00;
            buffer[4 * index + 3] = 0x30;
        }
    }

    for (unsigned int i = 0; i < size.Width(); ++i) {
        size_t index = 5 * size.Width() + i;

        buffer[4 * index + 0] = 0xFF;   // r
        buffer[4 * index + 1] = 0xFF;   // g
        buffer[4 * index + 2] = 0xFF;   // b
        buffer[4 * index + 3] = 0xFF;   // a

    }


    _return = setupTexturedRect(size);

    if (_return)
    {
        _return = InitTextureBuffer(size, &m_Texture);
    }

    if (_return)
    {
        m_initialized = TRUE;
    }

    return _return;
}

void
CSprite::Draw(glm::mat4 transform, class CShader& spriteShader)
{

    GLboolean blend = glIsEnabled(GL_BLEND);
    GLboolean cullface = glIsEnabled(GL_CULL_FACE);

    glBlendEquation(GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    //
    // draw zero
    //
    spriteShader.use();

    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);


    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, m_Texture);
    spriteShader.setMat4("model", transform);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);  // Unbind vao

    cullface ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}
