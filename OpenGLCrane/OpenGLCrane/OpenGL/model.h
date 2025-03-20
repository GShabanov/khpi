#pragma once
#ifndef __OPENGL_MODEL_H__
#define __OPENGL_MODEL_H__

#include "assimp/scene.h"
#include "GlUtils.h"

class CModel
{
private:
    class CLogCallback*            m_log;
    std::vector<class CMesh *>  m_meshes;


    typedef std::vector<class CMesh*>::iterator  meshIterator;

    bool                        m_initialized;


    // Recursively process each Node by calling processMesh on each node's 
    // meshes and adding them to m_meshes vector
    void processNode(const aiNode *node, const aiScene *scene, glm::mat4 &parentTransform);
    // Create mesh object with vertex and index daa
    class CMesh* processMesh(const aiMesh *mesh, const aiScene *scene);
    class CMesh* processMesh2(const aiMesh* mesh, const aiScene* scene, glm::mat4& relTransform);


public:
    CModel(class CLogCallback* logcallback)
    {
        m_log = logcallback;
        m_initialized = false;
    }

    ~CModel();

    void setColor(COLORREF  modelColor);

    // Load model and call processNode
    BOOL loadModel(const TCHAR* path);

    // Call draw function of all meshes in m_meshes
    void Draw(class  CShader& shader);

};

#endif // __OPENGL_MODEL_H__
