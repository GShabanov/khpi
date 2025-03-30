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

#pragma once
#ifndef __OPENGL_DIAGRAM_H__
#define __OPENGL_DIAGRAM_H__


class CDiagram 
{
private:
    class CLogCallback* m_log;


    PVOID                       m_DiagramData;
    POINT                       m_dimensions;

    CSprite                     m_sprite;
    bool                        m_initialized;

public:
    CDiagram(class CLogCallback* logcallback)
        : m_sprite(logcallback)
    {
        m_log = logcallback;
        m_initialized = false;

        m_dimensions.x = 64;
        m_dimensions.y = 64;
    }

    virtual ~CDiagram();

    void UpdateDiagram(POINT& textureSize);


    BOOL setup(glm::vec2& imageSize, POINT& textureSize);

    // Call draw function of all meshes in m_meshes
    void Draw(glm::mat4 transform, class CShader& spriteShader);

};

#endif // __OPENGL_SPRITE_H__
