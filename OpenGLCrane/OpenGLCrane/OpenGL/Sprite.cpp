/***************************************************************************************\
*   File:                                                                               *
*       Sprite.cpp                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       Sprite drawing implementation                                                   *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    28-Mar-2025                                                     *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"
#include "shader.h"
#include "Sprite.h"


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
        free(m_defaultData);
    }
}

BOOL
CSprite::InitTextureBuffer(GLuint* id)
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_dimensions.x, m_dimensions.y, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, m_defaultData);

    *id = texture;

    return TRUE;
}

BOOL
CSprite::setupRenderingObject(glm::vec2& imageSize)
{

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions                                   // texture coords U/V
        0.0f,               (float)imageSize.y, 0.0f,  0.0f,        0.0f,   // top left
        (float)imageSize.x, (float)imageSize.y, 0.0f,  (float)1.0f, 0.0f,   // top right
        0.0f,               0.0f,               0.0f,  0.0f, (float)1.0f,   // bottom left
        (float)imageSize.x, 0.0f,               0.0f,  (float)1.0f, (float)1.0f,   // bottom right
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

void
CSprite::UpdateData(PVOID data, POINT& textureSize)
{
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x, textureSize.y, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// Load texture
BOOL
CSprite::setup(glm::vec2& imageSize, POINT& textureSize)
{
    BOOL   _return = FALSE;
    SIZE_T imageDataSize;

    imageDataSize = textureSize.x * textureSize.y * sizeof(DWORD);

    m_defaultData = malloc(imageDataSize);

    if (!m_defaultData)
        return FALSE;

    memset(m_defaultData, 0xFF, imageDataSize);

    unsigned char* buffer = (unsigned char*)m_defaultData;

    for (LONG j = 0; j < textureSize.y; ++j)
    {
        size_t yoff = j * textureSize.x;

        for (LONG i = 0; i < textureSize.x; ++i)
        {

            buffer[4 * (yoff + i) + 0] = 0xFF;   // r
            buffer[4 * (yoff + i) + 1] = 0xFF;   // g
            buffer[4 * (yoff + i) + 2] = 0x00;   // b
            buffer[4 * (yoff + i) + 3] = 0x30;   // a
        }
    }

    _return = setupRenderingObject(imageSize);

    if (_return)
    {
        _return = InitTextureBuffer(&m_Texture);
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
