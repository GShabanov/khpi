#pragma once

#ifndef __OPENGL_MESH_H__
#define __OPENGL_MESH_H__

#include <vector>
#include "glm\include\glm\glm.hpp"
#include "GlUtils.h"


class CMesh
{
private:
    bool                m_initialized;
    bool                m_updateNeeded;


    glm::uint32        m_vao;
    glm::uint32        m_vbo;
    glm::uint32        m_ebo;


    glm::vec4                 m_color;

protected:
    std::vector<Vertex>       m_vertices;
    std::vector<glm::uint32>  m_indices;


public:
    bool  setupMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    void  setColor(COLORREF  rgb);
    void  setColor(float r, float g, float b);
    void  setColor(float r, float g, float b, float a);

    bool  updateVertices(std::vector<Vertex>& vertices);

    CMesh();
    ~CMesh();
    
    void Draw(class CShader &shader);

    friend class CModel;
};

#endif // __OPENGL_MESH_H__