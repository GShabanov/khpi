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
#include "Sprite.h"
#include "CDiagram.h"
#include "GlVector.h"

class CForceVectors
{
private:
    //
    // External forces at key points
    //
    CVector         m_forceAtE;
    CVector         m_reactionAtE; // Beam reaction at point E

    CVector         m_forceAtC1;
    CVector         m_reactionAtC1;
    CVector         m_forceAtC2;
    CVector         m_reactionAtC2;

    // Reaction forces at joints and supports
    CVector         m_forceAtD1;
    CVector         m_reactionAtD1;
    CVector         m_forceAtD2;
    CVector         m_reactionAtD2;

    CVector         m_forceAtB;
    CVector         m_reactionAtB;

    const float     m_vectorScale = 0.4f;


public:
    CForceVectors(class CLogCallback* log);

    bool Init(class CMathModel* mathModel);
    void Update(CMathModel* mathModel);
    void Draw(CMathModel* mathModel, glm::mat4& relativeTransform, CShader& vectorShader);

};

class CRenderer
{
protected:
    class CLogCallback*       m_log;

    HWND                m_parent;
    HWND                m_hwndOpenGl;
    GLFWwindow*         m_pGlWindow;
    CModel              m_ArrowModel;
    CModel              m_CraneArrowModel;
    CModel              m_CrankModel;
    CModel              m_RodModel;
    CDiagram            m_arrowLabel;
    CDiagram            m_rodLabel;

    CForceVectors       m_vectors;

    CCamera             m_Camera;

    CShader             m_DefaultShader;
    CShader             m_ModelShader;
    CShader             m_SpriteShader;
    CShader             m_VectorShader;

    class CMathModel*   m_mathModel;

    double              m_prev_x;
    double              m_prev_y;
    bool                m_first_mouse;

    GLint               m_glVerMajor;
    GLint               m_glVerMinor;

    const float         m_worldScale = 0.05f;
    glm::mat4           m_worldTransform;

    void  LogMessage(const TCHAR* format, ...);

    void UpdateVectors();

public:
    CRenderer(class CLogCallback *log);
    ~CRenderer();

    BOOL   Init(_In_ HWND  parent, _In_ CMathModel* mathModel);
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
