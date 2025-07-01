/***************************************************************************************\
*   File:                                                                               *
*       Renderer.h                                                                      *
*                                                                                       *
*   Abstract:                                                                           *
*       Renderer implementation                                                         *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    7-Sep-2024                                                      *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#pragma once
#ifndef __OPENGL_RENDERER_H__
#define __OPENGL_RENDERER_H__

#include "GlUtils.h"

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Background.h"
#include "winText.h"
#include "TeXtext.h"


class CRenderer
{
protected:
    class CLogCallback*       m_log;

    CWnd*               m_parent;
    HWND                m_hwndOpenGl;
    GLFWwindow*         m_pGlWindow;
    CModel              m_SampleModel;
    CWinText            m_sampleText;
    CTeXText            m_sampleLaTeX;

    CCamera             m_Camera;

    CShader             m_DefaultShader;
    CBackground         m_Background;

    double              m_prev_x;
    double              m_prev_y;
    bool                m_first_mouse;

    GLint               m_glVerMajor;
    GLint               m_glVerMinor;

    void  LogMessage(const TCHAR* format, ...);

    void drawCircle3f(float cx, float cy, float r, int num_segments);
    void drawLine3f(float cx, float cy, float cx2, float cy2);


public:
    CRenderer(class CLogCallback *log);
    ~CRenderer();

    BOOL   Init(_In_ CWnd  *parent);
    void   SetSize(int cx, int cy);
    void   Draw();

    void   setContextCurrent(BOOL set);


protected:
    void GlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message);
    void OnScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
    void OnMouseCallback(GLFWwindow* window, double x_pos, double y_pos);


    static void GLAPIENTRY
        stGlMessageCallback(
            GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            CRenderer* lpThis
        )
    {
        lpThis->GlMessageCallback(source, type, id, severity, length, message);
    }

    static void GLAPIENTRY
        stOnScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
    {
        CRenderer* lpThis = reinterpret_cast<CRenderer*>(
            glfwGetWindowUserPointer(window));

        ASSERT(lpThis != NULL);

        lpThis->OnScrollCallback(window, x_offset, y_offset);

    }

    static void GLAPIENTRY
        stOnMouseCallback(GLFWwindow* window, double x_pos, double y_pos)
    {
        CRenderer* lpThis = reinterpret_cast<CRenderer*>(
            glfwGetWindowUserPointer(window));

        ASSERT(lpThis != NULL);

        lpThis->OnMouseCallback(window, x_pos, y_pos);

    }

    friend class CModel;
};



#endif // __OPENGL_RENDERER_H__
