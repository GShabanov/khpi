/***************************************************************************************\
*   File:                                                                               *
*       Shader.h                                                                        *
*                                                                                       *
*   Abstract:                                                                           *
*       Shader loader implementation                                                    *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    7-Sep-2024                                                      *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#pragma once

#ifndef __OPENGL_SHADER_H__
#define __OPENGL_SHADER_H__

#include "GlUtils.h"

class CShader
{
private:
    class CLogCallback* m_log;
    unsigned int m_id;

    unsigned int compile(unsigned int type, const char* data);

    void LogMessage(const TCHAR* format, ...);

public:
    CShader(class CLogCallback *logcallback)
    {
        m_log = logcallback;
        m_id = -1;
    }

    ~CShader();

    BOOL Init(const TCHAR* vertex_path, const TCHAR* fragment_path, const TCHAR* geometry_path);

    // Bind shader program to opengl
    void use(void);
    // Uniform setting functions
    void setVec3(const char *var_name, glm::vec3 &vector);
    void setMat4(const char* var_name, glm::mat4 &matrix);
    void setFloat(const char* var_name, float value);
    
    unsigned int get_id(void) { return m_id; }
};

#endif // __OPENGL_SHADER_H__