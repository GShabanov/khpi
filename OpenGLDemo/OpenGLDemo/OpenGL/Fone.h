#pragma once
#ifndef __OPENGL_FONE_H__
#define __OPENGL_FONE_H__

#include <vector>
#include "glm/glm.hpp"
#include "GlUtils.h"


class CFone
{
private:
    bool                m_initialized;


    glm::uint32        m_fbo;
    glm::uint32        m_texture;
    
    DWORD*             m_textureData;

    const int          m_width = 800;
    const int          m_height = 600;


protected:


public:

    CFone();
    ~CFone();

    bool setup();

    void Draw(CRect& drawRect);

};

#endif // __OPENGL_FONE_H__