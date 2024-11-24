#include "pch.h"
#include "shader.h"
#include "mesh.h"
#include "model.h"

#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "glm/gtx/normal.hpp"



CModel::~CModel()
{

    while (!m_meshes.empty())
    {

        CMesh* cmesh = m_meshes.back();
        m_meshes.pop_back();

        delete cmesh;
    }
}

BOOL
CModel::loadModel(const TCHAR* path)
{

    unsigned char* meshData;
    size_t         meshSize;

    if (!ReadToBuffer(path, &meshData, &meshSize))
        return FALSE;

    Assimp::Importer import;
    const aiScene *scene = import.ReadFileFromMemory(meshData, meshSize, 0, "3mf");  //aiProcess_FlipUVs

    delete[] meshData;

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {

        //std::cout << "ASSIMP ERROR: " << import.GetErrorString() << std::endl;
        return FALSE;
    }

    glm::mat4  transform = glm::mat4(1.0);

    processNode(scene->mRootNode, scene, transform);

    m_initialized = true;

    return TRUE;
}

void
CModel::processNode(const aiNode *node, const aiScene *scene, glm::mat4& parentTransform)
{
    glm::mat4  relTransform = glm::mat4(
        node->mTransformation.a1, node->mTransformation.b1, node->mTransformation.c1, node->mTransformation.d1,
        node->mTransformation.a2, node->mTransformation.b2, node->mTransformation.c2, node->mTransformation.d2,
        node->mTransformation.a3, node->mTransformation.b3, node->mTransformation.c3, node->mTransformation.d3,
        node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c4, node->mTransformation.d4
    );

    glm::mat4  curTransform = relTransform * parentTransform;

    //
    // Process all the node's meshes (if any)
    //
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        CMesh *cmesh = processMesh2(mesh, scene, curTransform);

        if (cmesh != NULL)
        {
            
            m_meshes.push_back(cmesh);
        }
    }

    //
    // Do the same for each of its children
    //
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, relTransform);
    }
}

void
CModel::setColor(COLORREF  modelColor)
{
    for (std::vector<CMesh*>::iterator i = m_meshes.begin(); i != m_meshes.end(); i++)
    {
        (*i)->setColor(modelColor);
    }
        
}


CMesh *
CModel::processMesh(const aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    //
    // Load verticies
    //
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2  vec;
            glm::vec3  vector3;

            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            //
            // tangent
            //
            vector3.x = mesh->mTangents[i].x;
            vector3.y = mesh->mTangents[i].y;
            vector3.z = mesh->mTangents[i].z;
            vertex.Tangent = vector3;
            //
            // bitangent
            //
            vector3.x = mesh->mBitangents[i].x;
            vector3.y = mesh->mBitangents[i].y;
            vector3.z = mesh->mBitangents[i].z;

            vertex.Bitangent = vector3;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }


        vertices.push_back(vertex);
    }

    //
    // Load indices
    //
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace *face = &mesh->mFaces[i];

        for (unsigned int j = 0; j < face->mNumIndices; j++)
            indices.push_back(face->mIndices[j]);
    }

    if (scene->HasMaterials())
    {
        //
        // process materials
        //
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        for (unsigned int i = 0; i < material->mNumProperties; i++)
        {
            aiMaterialProperty *prop = material->mProperties[i];

            unsigned int semantic = prop->mSemantic;
        }


    }


    CMesh *cmesh = new CMesh();

    if (cmesh == NULL)
        return NULL;

    if (cmesh->setupMesh(vertices, indices) != TRUE)
    {
        delete cmesh;
        return NULL;
    }

    return cmesh;
}

CMesh*
CModel::processMesh2(const aiMesh* mesh, const aiScene* scene, glm::mat4 &relTransform)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    CMesh* cmesh = new CMesh();

    if (cmesh == NULL)
        return NULL;


    //
    // scan faces
    //
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace* face = &mesh->mFaces[i];

        ASSERT(face->mNumIndices == 3);

        if (face->mNumIndices != 3)
        {
            delete cmesh;
            return NULL;
        }

        glm::uint32 ind1 = face->mIndices[0];
        glm::uint32 ind2 = face->mIndices[1];
        glm::uint32 ind3 = face->mIndices[2];

        ASSERT(ind1 <= mesh->mNumVertices);

        if (ind1 > mesh->mNumVertices)
        {
            delete cmesh;
            return NULL;
        }

        ASSERT(ind2 <= mesh->mNumVertices);

        if (ind2 > mesh->mNumVertices)
        {
            delete cmesh;
            return NULL;
        }

        ASSERT(ind3 <= mesh->mNumVertices);

        if (ind3 > mesh->mNumVertices)
        {
            delete cmesh;
            return NULL;
        }

        aiVector3D* aiVertex[3];

        aiVertex[0] = &mesh->mVertices[ind1];
        aiVertex[1] = &mesh->mVertices[ind2];
        aiVertex[2] = &mesh->mVertices[ind3];

        Vertex vertex[3];

        //
        // copy to GLM representation
        //
        for (int j = 0; j < 3; j++)
        {
            vertex[j].Position.x = aiVertex[j]->x;
            vertex[j].Position.y = aiVertex[j]->y;
            vertex[j].Position.z = aiVertex[j]->z;
        }

        for (int j = 0; j < 3; j++)
        {
            vertex[j].Position = relTransform * glm::vec4(vertex[j].Position, 1);
        }

        //
        // build normals
        //
        vertex[0].Normal = glm::triangleNormal(vertex[0].Position, vertex[1].Position, vertex[2].Position);
        vertex[1].Normal = glm::triangleNormal(vertex[1].Position, vertex[2].Position, vertex[0].Position);
        vertex[2].Normal = glm::triangleNormal(vertex[2].Position, vertex[0].Position, vertex[1].Position);

        vertices.push_back(vertex[0]);
        indices.push_back(i * 3 + 0);
        vertices.push_back(vertex[1]);
        indices.push_back(i * 3 + 1);
        vertices.push_back(vertex[2]);
        indices.push_back(i * 3 + 2);

    }

    if (scene->HasMaterials())
    {

        cmesh->setColor(RGB(0, 0, 0));
        //
        // process materials
        //
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiColor4D color;

        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
        {
            cmesh->setColor(color.r * 1.5f, color.g * 1.5f, color.b * 1.5f, color.a);
        }
        else
        {
            cmesh->setColor(0.0, 0.0, 0.0, 1.0);
        }

    }

    if (cmesh->setupMesh(vertices, indices) != TRUE)
    {
        delete cmesh;
        return NULL;
    }

    return cmesh;
}



void
CModel::Draw(CShader &shader)
{

    if (!m_initialized)
        return;

    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i]->Draw(shader);
    }

}



