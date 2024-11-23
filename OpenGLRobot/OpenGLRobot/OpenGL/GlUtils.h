#pragma once
#ifndef __OPENGL_GLUTILS_H__
#define __OPENGL_GLUTILS_H__

#include <string>
//#include <vector>
#define GLEW_STATIC
#define GL_SILENCE_DEPRECATION
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


struct Vertex
{
    //
    // position
    //
    glm::vec3 Position;

    //
    // normal
    //
    glm::vec3 Normal;

    //
    // texture coordinates
    //
    glm::vec2 TexCoords;

    //
    // tangent
    //
    glm::vec3 Tangent;

    //
    // bitangent
    //
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


extern const TCHAR*
GlSourceToStr(GLenum source);

extern const TCHAR*
GlTypeToStr(GLenum type);

const TCHAR*
GlSeverityToStr(GLenum severity);

extern glm::vec3
CalcNormal(glm::vec3& vertex1, glm::vec3& vertex2, glm::vec3& vertex3);

extern bool 
rayIntersectsTriangle(
    glm::vec3 origin,
    glm::vec3 dir,
    glm::vec3 v0,
    glm::vec3 v1,
    glm::vec3 v2,
    float* intersection);


BOOL ReadToBuffer(const TCHAR* path, unsigned char** data, size_t *size = NULL);
BOOL FromResource(LPCTSTR resource, LPCTSTR type, char** data);

#endif // __OPENGL_GLUTILS_H__
