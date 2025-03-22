/***************************************************************************************\
*   File:                                                                               *
*       Renderer.cpp                                                                    *
*                                                                                       *
*   Abstract:                                                                           *
*       Renderer implementation                                                         *
*                                                                                       *
*   Author:                                                                             *
*       GShabanov ()    7-Sep-2024                                                      *
*                                                                                       *
*   Revision History:                                                                   *
\***************************************************************************************/
#include "pch.h"

#include "LogWindow.h"

#include "GlUtils.h"
#include "Renderer.h"
#include "MathModel.h"



CRenderer::CRenderer(CLogCallback* log)
    : m_log(log)
    , m_parent(NULL)
    , m_DefaultShader(log)
    , m_ModelShader(log)
    , m_SpriteShader(log)
    , m_ArrowModel(log)
    , m_CraneArrowModel(log)
    , m_CrankModel(log)
    , m_RodModel(log)
    , m_arrowLabel(log)
    , m_hwndOpenGl(NULL)
    , m_glVerMajor(2)
    , m_glVerMinor(0)
    , m_mathModel(NULL)
{
    m_prev_x = 0.0;
    m_prev_y = 0.0;
    m_first_mouse = false;
    m_pGlWindow = NULL;

    m_baseMatrix = glm::mat4(1.0f);

    m_baseMatrix = glm::translate(m_baseMatrix, glm::vec3(0.0, -3.0, 0.0));
    m_baseMatrix = glm::scale(m_baseMatrix, glm::vec3(0.05f));

}

CRenderer::~CRenderer()
{
    if (m_pGlWindow != NULL)
    {
        glfwDestroyWindow(m_pGlWindow);
        glfwTerminate();
    }

}

void
CRenderer::GlMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message
    )
{
    bool showLog = true;

    if (source == GL_DEBUG_SOURCE_API)
    {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        {
            showLog = false;
        }
    }

    if (showLog)
    {
        LogMessage(_T("GLDEBUG: src %s, type %s, id 0x%X, severity %s"), GlSourceToStr(source), GlTypeToStr(type), id, GlSeverityToStr(severity));
        LogMessage(_T("GLDEBUG: message: %S"), message);
    }
}

void
CRenderer::LogMessage(const TCHAR* format, ...)
{
    if (m_log != NULL)
    {
        va_list args;
        va_start(args, format);

        m_log->LogMessage(CLogCallback::LOG_GL, format, args);


        va_end(args);
    }

}


BOOL
CRenderer::Init(_In_ HWND  parent, _In_ CMathModel* mathModel)
{
    GLenum err;
    GLFWwindow* pGlWindow = NULL;
    GLFWmonitor* monitor = NULL;

    m_parent = parent;
    m_mathModel = mathModel;

    CString  output = _T("Initializing GLFW...");

    if (!glfwInit())
    {
        output += _T("FAILED\n");
        LogMessage(output);
        return FALSE;
    }

    output += _T("OK\n");
    LogMessage(output);




    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_DECORATED, FALSE);
    glfwWindowHint(GLFW_FOCUSED, FALSE);
    glfwWindowHint(GLFW_RESIZABLE, FALSE);
    glfwWindowHint(GLFW_VISIBLE, FALSE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    pGlWindow = glfwCreateWindow(40, 40, "OpenGlDemo", monitor, NULL);

    if (!pGlWindow)
    {
        output += _T("FAILED");
        LogMessage(output);
        return FALSE;
    }

    output += _T("OK");
    LogMessage(output);

    glfwSetWindowUserPointer(pGlWindow, this);

    m_hwndOpenGl = glfwGetWin32Window(pGlWindow);

    const LONG nNewStyle = (GetWindowLong(m_hwndOpenGl, GWL_STYLE) & ~WS_POPUP) | WS_CHILDWINDOW;
    SetWindowLong(m_hwndOpenGl, GWL_STYLE, nNewStyle);

    ::SetParent(m_hwndOpenGl, parent);

    glfwMakeContextCurrent(pGlWindow);

    glGetIntegerv(GL_MAJOR_VERSION, &m_glVerMajor);
    glGetIntegerv(GL_MINOR_VERSION, &m_glVerMinor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, std::min(3, m_glVerMajor));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, std::min(1, m_glVerMinor));


    glfwSetScrollCallback(pGlWindow, reinterpret_cast<GLFWscrollfun>(&stOnScrollCallback));
    glfwSetCursorPosCallback(pGlWindow, reinterpret_cast<GLFWcursorposfun>(&stOnMouseCallback));

    err = glewInit();

    if (err != GLEW_OK) {

        LogMessage(_T("[-] Failed to init GLEW: %S\n"), glewGetErrorString(err));
        return FALSE;
    }

    //
    // During init, enable debug output
    //
    glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(&stGlMessageCallback), this);

    char* glVersion = (char*)glGetString(GL_VERSION);

    if (glVersion) {

        LogMessage(_T("[*] OpenGL version: %S\n"), glVersion);
    }

    // Set vsync
    //glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);   // Depth testing
    glEnable(GL_CULL_FACE);    // Rear face culling
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    //glPointSize(10.0f);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_MULTISAMPLE);  // MSAA
    glEnable(GL_BLEND);

    m_Camera.setSensitivities(0.3f, 0.5f, 0.4f);

    // Instantiate, compile and link shader
    if (m_DefaultShader.Init(
        _T("shaders/default/default.vert"),
        _T("shaders/default/default.frag"),
        NULL) != TRUE)
    {
        LogMessage(_T("[-] Failed to load sharers"));
    }

    if (m_ModelShader.Init(
        _T("shaders/model/model.vert"),
        _T("shaders/model/model.frag"),
        _T("shaders/model/model.geom")) != TRUE)
    {
        LogMessage(_T("[-] Failed to load sharers"));
    }

    if (m_SpriteShader.Init(
        _T("shaders/sprite/sprite.vert"),
        _T("shaders/sprite/sprite.frag"),
        NULL) != TRUE)
    {
        LogMessage(_T("[-] Failed to load sharers"));
    }

    m_ArrowModel.loadModel(_T("data/Arrow.3MF"));
    m_ArrowModel.setColor(RGB(128, 255, 128));

    m_CraneArrowModel.loadModel(_T("data/Crane_arrow.3MF"));
    m_CraneArrowModel.setColor(RGB(255, 128, 128));

    m_CrankModel.loadModel(_T("data/Crank.3MF"));
    m_CrankModel.setColor(RGB(128, 128, 255));

    m_RodModel.loadModel(_T("data/Rod.3MF"));
    m_RodModel.setColor(RGB(128, 255, 255));

    CRect  label(0, 0, 100, 10);
    m_arrowLabel.setup(label, NULL);


    m_pGlWindow = pGlWindow;

    glfwWindowHint(GLFW_VISIBLE, TRUE);    
    ::ShowWindow(m_hwndOpenGl, SW_SHOW);

    return TRUE;
}

void
CRenderer::setContextCurrent(BOOL set)
{
    if (!m_pGlWindow)
        return;

    if (set)
    {
        // Make the window's context current
        glfwMakeContextCurrent(m_pGlWindow);

    }
    else
    {
        // Make the window's context current
        glfwMakeContextCurrent(NULL);
    }
}

void
CRenderer::OnScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    if (!m_pGlWindow)
        return;

    UNREFERENCED_PARAMETER(window);

    m_Camera.zoom(y_offset);    
    ::InvalidateRect(this->m_parent, NULL, FALSE);
}


void
CRenderer::OnMouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{


    if (m_first_mouse)
    {
        m_prev_x = x_pos;
        m_prev_y = y_pos;
        m_first_mouse = false;
    }

    double x_offset = x_pos - m_prev_x;
    double y_offset = m_prev_y - y_pos; // reversed since y-coordinates go from bottom to top

    m_prev_x = x_pos;
    m_prev_y = y_pos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        m_Camera.orbit(x_offset, y_offset);
        ::InvalidateRect(this->m_parent, NULL, FALSE);
        return;
    }

}

void
CRenderer::SetSize(int cx, int cy)
{
    if (!m_pGlWindow)
        return;

    glfwSetWindowSize(m_pGlWindow, cx, cy);
    glViewport(0, 0, cx, cy);
    m_Camera.setScreenDimensions(cx, cy);

}


void
CRenderer::Draw()
{
    if (!m_pGlWindow)
        return;

    if (!m_mathModel)
        return;

    glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view_mat = m_Camera.getViewMatrix();
    glm::mat4 proj_mat = m_Camera.getProjMatrix();


    //
    // use shader
    //
    m_DefaultShader.use();

    //
    // Light lighting properties
    //
    glm::vec3 light_ambient = glm::vec3(0.7, 0.7, 0.7);
    glm::vec3 light_diffuse = glm::vec3(0.7, 0.7, 0.7);
    glm::vec3 light_specular = glm::vec3(0.6, 0.6, 0.6);
    m_DefaultShader.setVec3("light.ambient", light_ambient);
    m_DefaultShader.setVec3("light.diffuse", light_diffuse);
    m_DefaultShader.setVec3("light.specular", light_specular);

    m_DefaultShader.setMat4("view", view_mat);
    m_DefaultShader.setMat4("projection", proj_mat);

    //glm::vec3 light_position = m_Camera.getPosition();
    glm::vec3 light_position = m_Camera.getLightVector();
    m_DefaultShader.setVec3("light.position", light_position);


    glm::vec3 view_pos = m_Camera.getPosition();
    m_DefaultShader.setVec3("view_pos", view_pos);
    // ---------------------------------------------

    //
    // model shader
    //
    m_ModelShader.use();
    m_ModelShader.setVec3("light.ambient", light_ambient);
    m_ModelShader.setVec3("light.diffuse", light_diffuse);
    m_ModelShader.setVec3("light.specular", light_specular);
    m_ModelShader.setVec3("light.position", light_position);

    m_ModelShader.setMat4("view", view_mat);
    m_ModelShader.setMat4("projection", proj_mat);
    m_ModelShader.setVec3("view_pos", view_pos);


    m_ModelShader.use();
    m_ModelShader.setMat4("projection", proj_mat);
    m_ModelShader.setMat4("view", view_mat);

    m_SpriteShader.use();
    m_SpriteShader.setMat4("projection", proj_mat);
    m_SpriteShader.setMat4("view", view_mat);

    //
    // arrow
    //
    
    m_ArrowModel.Draw(m_baseMatrix * m_mathModel->getArrowMatrix(), m_ModelShader);

    //
    // crank
    //
    m_CrankModel.Draw(m_baseMatrix * m_mathModel->getCrank1Matrix(), m_ModelShader);
    m_CrankModel.Draw(m_baseMatrix * m_mathModel->getCrank2Matrix(), m_ModelShader);

    //
    // crane arrow
    //
    m_CraneArrowModel.Draw(m_baseMatrix * m_mathModel->getCraneMatrix(), m_ModelShader);


    //
    // crane rod
    //
    m_RodModel.Draw(m_baseMatrix * m_mathModel->getRod1Matrix(), m_ModelShader);
    m_RodModel.Draw(m_baseMatrix * m_mathModel->getRod2Matrix(), m_ModelShader);


    glm::mat4  arrowLabel = m_baseMatrix * m_mathModel->getCraneMatrix();
    arrowLabel = glm::translate(arrowLabel, glm::vec3(0.0f, 20.000f, 0.0f));

    m_arrowLabel.Draw(arrowLabel, m_SpriteShader);

    arrowLabel = m_baseMatrix * m_mathModel->getRod1Matrix();
    arrowLabel = glm::translate(arrowLabel, glm::vec3(0.0f, 20.000f, 0.0f));

    m_arrowLabel.Draw(arrowLabel, m_SpriteShader);


    glUseProgram(0);

    // Установка матриц проекции и вида
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(proj_mat));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(view_mat));

    glm::vec4  zerro = m_baseMatrix * glm::vec4(1.0f);

    glLineWidth(2.0f); // line thin
    glColor3fv(glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
    //
    glBegin(GL_LINES);
    glVertex4f(zerro.x, zerro.y, zerro.z - 5.0f, zerro.w);
    glVertex4f(zerro.x, zerro.y, zerro.z + 5.0f, zerro.w);
    glEnd();


    zerro =  m_baseMatrix * m_mathModel->getArrowMatrix() * glm::vec4(1.0f);

    glLineWidth(2.0f); // line thin
    glColor3fv(glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
    //
    glBegin(GL_LINES);
    glVertex4f(zerro.x, zerro.y, zerro.z - 3.0f, zerro.w);
    glVertex4f(zerro.x, zerro.y, zerro.z + 3.0f, zerro.w);
    glEnd();

    zerro = m_baseMatrix * m_mathModel->getCrank1Matrix() * glm::vec4(1.0f);

    glColor3fv(glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
    glBegin(GL_LINES);
    glVertex4f(zerro.x, zerro.y, zerro.z - 3.0f, zerro.w);
    glVertex4f(zerro.x, zerro.y, zerro.z + 3.0f, zerro.w);
    glEnd();
    glLineWidth(1.0f); // line thin

    glfwSwapBuffers(m_pGlWindow);
}

