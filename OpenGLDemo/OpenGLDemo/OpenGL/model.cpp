#include "pch.h"
#include "shader.h"
#include "mesh.h"
#include "model.h"

#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"



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

    buildVolumeCubeForModel();

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
        //vertex[0].Normal = CalcNormal(vertex[1].Position, vertex[0].Position, vertex[2].Position);
        vertex[1].Normal = glm::triangleNormal(vertex[1].Position, vertex[2].Position, vertex[0].Position);
        //vertex[1].Normal = CalcNormal(vertex[2].Position, vertex[1].Position, vertex[0].Position);
        vertex[2].Normal = glm::triangleNormal(vertex[2].Position, vertex[0].Position, vertex[1].Position);
        //vertex[2].Normal = CalcNormal(vertex[0].Position, vertex[2].Position, vertex[1].Position);

        vertices.push_back(vertex[0]);
        indices.push_back(i * 3 + 0);
        vertices.push_back(vertex[1]);
        indices.push_back(i * 3 + 1);
        vertices.push_back(vertex[2]);
        indices.push_back(i * 3 + 2);

    }

    if (scene->HasMaterials())
    {

        //cmesh->setColor(RGB(mesh->mMaterialIndex * 30 + 30, 100, 100));
        cmesh->setColor(RGB(0, 0, 0));
        //
        // process materials
        //
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


        //aiMaterial* material = scene->mMaterials[3];

        for (unsigned int i = 0; i < material->mNumProperties; i++)
        {
            aiMaterialProperty* prop = material->mProperties[i];

            if (prop->mType == aiPTI_String)
            {
                if (prop->mDataLength == sizeof(aiColor4D))
                {
                    aiColor4D* colors = (aiColor4D*)prop->mData;

                    //cmesh->setColor(colors->r, colors->g, colors->b, colors->a);
                }
            }
            else if (prop->mType == aiPTI_Float)
            {
                if (prop->mDataLength == sizeof(aiColor4D))
                {
                    aiColor4D* colors = (aiColor4D*)prop->mData;

                    cmesh->setColor(colors->r, colors->g, colors->b, colors->a);
                }
                else
                if (prop->mDataLength == sizeof(aiColor3D))
                {
                    
                    aiColor3D* colors = (aiColor3D*)prop->mData;

                    cmesh->setColor(colors->r, colors->g, colors->b);
                }
                else
                {
                    ASSERT(FALSE);
                }
            }
            else if (prop->mType == aiPTI_Integer)
            {
                /*if (prop->mDataLength == sizeof(COLORREF))
                {
                    const BYTE* colors = (const BYTE*)prop->mData;

                    cmesh->setColor(colors[0], colors[1], colors[2], colors[3]);
                }*/

            }
            else
            {
                ASSERT(FALSE);
            }
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
CModel::buildVolumeCubeForModel()
{
    if (m_meshes.size() < 1)
        return;

    //
    //     6 *------------*  5
    //      /|          / |
    //     / |         /  |
    //  2 / 7|      1 /   |
    //   *-----------*    *  4
    //   |           |   /
    //   |           |  /
    //   |           | /
    //  3*-----------*  0
    //
    glm::float32_t  minX, maxX;
    glm::float32_t  minY, maxY;
    glm::float32_t  minZ, maxZ;

    CMesh* pMesh;

    pMesh = *(m_meshes.begin());

    if (pMesh->m_vertices.size() < 1)
        return;

    minX = maxX = pMesh->m_vertices[0].Position.x;
    minY = maxY = pMesh->m_vertices[0].Position.y;
    minZ = maxZ = pMesh->m_vertices[0].Position.z;


    for (meshIterator i = m_meshes.begin(); i != m_meshes.end(); ++i)
    {

        CMesh* pMesh = *i;

        for (size_t j = 0, k = pMesh->m_vertices.size(); j < k; ++j)
        {
            glm::vec3 vertex = pMesh->m_vertices[j].Position;

            if (vertex.x < minX) minX = vertex.x;
            if (vertex.x > maxX) maxX = vertex.x;
            if (vertex.y < minY) minY = vertex.y;
            if (vertex.y > maxY) maxY = vertex.y;
            if (vertex.z < minZ) minZ = vertex.z;
            if (vertex.z > maxZ) maxZ = vertex.z;
        }
    }

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    Vertex   vertex;
    vertex.Position = glm::vec3(minX, minY, minZ);  // 0
    vertices.push_back(vertex);
    vertex.Position = glm::vec3(minX, maxY, minZ);  // 1
    vertices.push_back(vertex);
    vertex.Position = glm::vec3(maxX, maxY, minZ);  // 2
    vertices.push_back(vertex);
    vertex.Position = glm::vec3(maxX, minY, minZ);  // 3
    vertices.push_back(vertex);

    vertex.Position = glm::vec3(minX, minY, maxZ);  // 4
    vertices.push_back(vertex);
    vertex.Position = glm::vec3(minX, maxY, maxZ);  // 5
    vertices.push_back(vertex);
    vertex.Position = glm::vec3(maxX, maxY, maxZ);  // 6
    vertices.push_back(vertex);
    vertex.Position = glm::vec3(maxX, minY, maxZ);  // 7
    vertices.push_back(vertex);


    // front 
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    // back 
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(5);

    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(6);

    for (int i = 0; i < 4; i++)
    {
        int next = i + 1;
        if (next > 3)
            next = 0;

        indices.push_back(i);
        indices.push_back(i + 4);
        indices.push_back(next + 4);

        indices.push_back(next);
        indices.push_back(i);
        indices.push_back(next + 4);
    }


    m_volumeIndices  = indices;
    m_volumeVertices = vertices;

}


// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture>
CModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        /*bool skip = false;

        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }

        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }*/
    }

    return textures;
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



