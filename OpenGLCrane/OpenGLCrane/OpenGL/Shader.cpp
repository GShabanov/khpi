/***************************************************************************************\
*   File:                                                                               *
*       Shader.cpp                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       Shader loader implementation                                                    *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    7-Sep-2024                                                      *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"
#include "Shader.h"

#include "GlUtils.h"
#include "LogWindow.h"


BOOL
CShader::Init(
    const TCHAR* vertex_path,
    const TCHAR* fragment_path,
    const TCHAR* geometry_path
    )
{
    // Check for errors
    int result;
    char error_log[512];
    error_log[0] = 0;
    BOOL _return = FALSE;

    // Compile
    unsigned int program = glCreateProgram();

    if (program == 0)
    {
        LogMessage(_T("CShader: failed to create program"));
        goto end;
    }

    unsigned int vertex_shader = 0;
    unsigned int fragment_shader = 0;
    unsigned int geometry_shader = 0;

    char* shader_cstr;


    if (vertex_path != NULL)
    {

        if (!ReadToBuffer(vertex_path, (unsigned char **)&shader_cstr))
            goto fail_delete_program;

        vertex_shader = compile(GL_VERTEX_SHADER, shader_cstr);

        delete[] shader_cstr;

        if (vertex_shader == 0)
        {
            LogMessage(_T("CShader: failed to compile vertex shader: %s"), vertex_path);
            goto fail_delete_program;
        }
    }

    if (fragment_path != NULL)
    {
        if (!ReadToBuffer(fragment_path, (unsigned char**)&shader_cstr))
            goto fail_delete_shaders;

        fragment_shader = compile(GL_FRAGMENT_SHADER, shader_cstr);

        delete[] shader_cstr;

        if (fragment_shader == 0)
        {
            LogMessage(_T("CShader: failed to compile fragment shader: %s"), fragment_path);
            goto fail_delete_shaders;
        }
    }

    if (geometry_path != NULL)
    {
        if (!ReadToBuffer(geometry_path, (unsigned char**)&shader_cstr))
            goto fail_delete_shaders;

        geometry_shader = compile(GL_GEOMETRY_SHADER, shader_cstr);

        delete[] shader_cstr;

        if (geometry_shader == 0)
        {
            LogMessage(_T("CShader: failed to compile geometry shader: %s"), geometry_path);

            goto fail_delete_shaders;
        }
    }

    // Link
    if (vertex_shader != 0)
        glAttachShader(program, vertex_shader);

    if (fragment_shader != 0)
        glAttachShader(program, fragment_shader);

    if (geometry_shader != 0)
        glAttachShader(program, geometry_shader);

    glLinkProgram(program);


    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {

        LogMessage(_T("CShader: failed to link program"));
        //std::cout << "FAILED TO LINK SHADERS: " << error_log << std::endl;

        glGetProgramInfoLog(program, 512, nullptr, error_log);
        

        goto fail_delete_shaders;
    }


    // Validate
    glValidateProgram(program);



    m_id = program;

    _return  = TRUE;

fail_delete_shaders:
    if (vertex_shader != 0) {

        glDetachShader(program, vertex_shader);
        glDeleteShader(vertex_shader);
    }

    if (fragment_shader != 0) {

        glDetachShader(program, fragment_shader);
        glDeleteShader(fragment_shader);
    }

    if (geometry_shader != 0) {

        glDetachShader(program, geometry_shader);
        glDeleteShader(geometry_shader);
    }

fail_delete_program:

    if (_return != TRUE)
    {
        glDeleteProgram(program);
    }

end:
    return _return;
}

void
CShader::LogMessage(const TCHAR* format, ...)
{
    va_list args;
    va_start(args, format);

    m_log->LogMessage(CLogCallback::LOG_GL, format, args);

    va_end(args);
}



CShader::~CShader()
{
    if (m_id != -1)
    {
        glDeleteProgram(m_id);
    }
}


unsigned int
CShader::compile(unsigned int type, const char* data)
{

    // Compile shader
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &data, nullptr);
    glCompileShader(id);

    // Check for errors
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        char error_log[512];
        glGetShaderInfoLog(id, 512, nullptr, error_log);
        glDeleteShader(id);

        LogMessage(_T("[-] CShader: %S"), error_log);
        //std::cout << "FAILED TO COMPILE SHADER: " << error_log << std::endl;
        return 0;
    }

    return id;
}

void
CShader::use(void)
{
    glUseProgram(m_id);
}

void
CShader::setVec3(const char* var_name, glm::vec3 &vector)
{
    int location = glGetUniformLocation(m_id, var_name);
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void
CShader::setMat4(const char* var_name, glm::mat4 &matrix)
{
    int location = glGetUniformLocation(m_id, var_name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void CShader::setFloat(const char* var_name, float value)
{
    int location = glGetUniformLocation(m_id, var_name);
    glUniform1f(location, value);
}
