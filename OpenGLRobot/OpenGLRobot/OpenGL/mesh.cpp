#include "pch.h"
#include "mesh.h"
#include "shader.h"


CMesh::CMesh()
    : m_initialized(false)
    , m_vao(0)
    , m_vbo(0)
    , m_ebo(0)
{
    m_color = glm::vec4(1.0);
}

CMesh::~CMesh()
{
    if (m_initialized)
    {
        glDeleteBuffers(1, &m_ebo);
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }
}

bool
CMesh::setupMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
    m_vertices = vertices;
    m_indices = indices;

    glGenVertexArrays(1, &m_vao);
    //
    // We allocate a GLuint in the global part of the program to store the handle of the vertex buffer object. 
    // You will see later that most (if not all) OpenGL objects are accessed via a variable of GLuint type.
    //
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);


    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    //
    // After binding our object we fill it with data. The call above takes the target name (same as what we used for binding),
    // the size of the data in bytes, address of the array of vertices and a flag that indicates the usage pattern for this data.
    // Since we are not going to change the buffer contents we specify GL_STATIC_DRAW. The opposite will be GL_DYNAMIC_DRAW.
    // While this is only a hint to OpenGL it is a good thing to give some thought as to the proper flag to use. The driver can rely
    // on it for optimization heuristics (such as what is the best place in memory to store the buffer). 
    //
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), 
                            &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
                            &m_indices[0], GL_STATIC_DRAW);

    //
    // Vertex positions
    //
    // In the shaders tutorial you will see that vertex attributes used in the shader (position, normal, etc) have an
    // index mapped to them that enable you to create the binding between the data in the C/C++ program and the attribute
    // name inside the shader. In addition you must also enable each vertex attribute index. In this tutorial we are not
    // yet using any shader but the vertex position we have loaded into the buffer is treated as vertex attribute index 0
    // in the fixed function pipeline (which becomes active when there is no shader bound). You must enable each vertex
    // attribute or else the data will not be accessible by the pipeline.
    //
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    //
    // Vertex normals
    //
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)FIELD_OFFSET(Vertex, Normal));

    //
    // vertex texture coords
    //
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)FIELD_OFFSET(Vertex, TexCoords));

    //
    // vertex tangent
    //
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)FIELD_OFFSET(Vertex, Tangent));

    //
    // vertex bitangent
    //
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)FIELD_OFFSET(Vertex, Bitangent));


    glBindVertexArray(0);

    m_initialized = TRUE;

    return TRUE;
}

void
CMesh::setColor(COLORREF  rgb)
{
    float r = GetRValue(rgb) * (1.0f / 255);
    float g = GetGValue(rgb) * (1.0f / 255);
    float b = GetBValue(rgb) * (1.0f / 255);

    m_color = glm::vec4(r, g, b, 1.0);
}

void
CMesh::setColor(float r, float g, float b)
{
    m_color = glm::vec4(r, g, b, 1.0);
}

bool
CMesh::updateVertices(std::vector<Vertex>& vertices)
{
    int j = 0;

    if (vertices.size() != m_vertices.size())
        return false;

    for (std::vector<Vertex>::iterator i = m_vertices.begin(); i != m_vertices.end(); ++i)
    {
        *i = vertices[j];
        j++;
    }

    m_updateNeeded = true;

    return true;
}

void
CMesh::setColor(float r, float g, float b, float a)
{
    m_color = glm::vec4(r, g, b, a);
}

void
CMesh::Draw(CShader &shader)
{
    if (m_updateNeeded)
    {
        m_updateNeeded = false;
        // bind VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        // get buffer with pixels
        Vertex* ptr = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

        if (ptr != NULL)
        {
            for (std::vector<Vertex>::iterator i = m_vertices.begin(); i != m_vertices.end(); ++i)
            {
                *ptr = *i;

                ptr++;
            }

            // release buffer
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    glBindVertexArray(m_vao);

    //
    // Set fragment shader uniforms
    // Material lighting properties
    //
    //glm::vec3 material_ambient = glm::vec3(0.6, 0.6, 0.6);
    glm::vec3 material_ambient = glm::vec3(m_color);
    shader.setVec3("material.ambient", material_ambient);

    //glm::vec3 material_diffuse = glm::vec3(m_color);
    glm::vec3 material_diffuse = glm::vec3(0.4, 0.4, 0.4);
    shader.setVec3("material.diffuse", material_diffuse);

    glm::vec3 material_specular = glm::vec3(0.8, 0.8, 0.8);
    //glm::vec3 material_specular = glm::vec3(m_color);
    shader.setVec3("material.specular", material_specular);

    float material_shininess = 32.0;
    shader.setFloat("material.shininess", material_shininess);


    glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);  // Unbind vao
}
