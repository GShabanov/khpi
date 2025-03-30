/***************************************************************************************\
*   File:                                                                               *
*       CDiagram.h                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       Diagram drawing implementation                                                  *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    28-Mar-2025                                                     *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"
#include "shader.h"
#include "Sprite.h"
#include "CDiagram.h"


CDiagram::~CDiagram()
{

    if (m_initialized)
    {
        free(m_DiagramData);
    }
}

void
CDiagram::UpdateDiagram(POINT& textureSize)
{
    m_sprite.UpdateData(m_DiagramData, m_dimensions);
}

// Load texture
BOOL
CDiagram::setup(glm::vec2& imageSize, POINT& textureSize)
{
    BOOL   _return = FALSE;

    m_dimensions = textureSize;

    _return = m_sprite.setup(imageSize, textureSize);

    if (_return != TRUE)
        return FALSE;


    SIZE_T imageDataSize;

    imageDataSize = textureSize.x * textureSize.y * sizeof(DWORD);

    m_DiagramData = malloc(imageDataSize);

    if (!m_DiagramData)
        return FALSE;

    memset(m_DiagramData, 0xFF, imageDataSize);

    unsigned char* buffer = (unsigned char*)m_DiagramData;

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

    for (LONG i = 0; i < textureSize.x; ++i) {

        size_t yoff = (textureSize.y >> 1) * textureSize.x;

        buffer[4 * (yoff + i) + 0] = 0xFF;   // r
        buffer[4 * (yoff + i) + 1] = 0x00;   // g
        buffer[4 * (yoff + i) + 2] = 0x00;   // b
        buffer[4 * (yoff + i) + 3] = 0xFF;   // a
    }

    m_sprite.UpdateData(m_DiagramData, m_dimensions);

    if (_return)
    {
        m_initialized = TRUE;
    }

    return _return;
}

void
CDiagram::Draw(glm::mat4 transform, class CShader& spriteShader)
{

    m_sprite.Draw(transform, spriteShader);
}
