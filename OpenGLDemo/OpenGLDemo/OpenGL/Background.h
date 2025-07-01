#pragma once
#ifndef __OPENGL_BACKGROUND_H__
#define __OPENGL_BACKGROUND_H__

#include <vector>
#include "glm\include\glm/glm.hpp"
#include "GlUtils.h"


class CBackground
{
private:
    bool                m_initialized;

    HDC                m_hDcCompatible;
    HBITMAP            m_hBitmap;
    glm::uint32        m_fbo;
    glm::uint32        m_texture;
    
    unsigned int*      m_textureData;

    unsigned int       m_width;
    unsigned int       m_height;


protected:


public:

    CBackground();
    ~CBackground();

    bool setup();

    bool prepareFBOData();

    void Resize(unsigned int sX, unsigned int sY);

    void PreDraw();
    void Draw(CRect& drawRect);

};

#endif // __OPENGL_BACKGROUND_H__