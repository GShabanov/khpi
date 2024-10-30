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



CRenderer::CRenderer(CLogCallback* log)
    : m_log(log)
    , m_parent(NULL)
    , m_DefaultShader(log)
    , m_SampleModel(log)
    , m_hwndOpenGl(NULL)
    , m_glVerMajor(2)
    , m_glVerMinor(0)

{
    m_prev_x = 0.0;
    m_prev_y = 0.0;
    m_first_mouse = false;
    m_pGlWindow = NULL;
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
CRenderer::Init(_In_ CWnd* parent)
{
    GLenum err;
    GLFWwindow* pGlWindow = NULL;
    GLFWmonitor* monitor = NULL;

    m_parent = parent;

    CString  output = _T("Initializing GLFW...");

    if (!glfwInit())
    {
        output += _T("FAILED");
        LogMessage(output);
        return FALSE;
    }

    output += _T("OK");
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

    CWnd *window = CWnd::FromHandle(m_hwndOpenGl);
    window->SetParent(parent);

    //SetWindowLong(glWindow.m_hWnd, )

    glfwMakeContextCurrent(pGlWindow);

    glGetIntegerv(GL_MAJOR_VERSION, &m_glVerMajor);
    glGetIntegerv(GL_MINOR_VERSION, &m_glVerMinor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, std::min(3, m_glVerMajor));
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, std::min(1, m_glVerMinor));


    glfwSetScrollCallback(pGlWindow, reinterpret_cast<GLFWscrollfun>(&stOnScrollCallback));
    glfwSetCursorPosCallback(pGlWindow, reinterpret_cast<GLFWcursorposfun>(&stOnMouseCallback));

    err = glewInit();

    if (err != GLEW_OK) {

        LogMessage(_T("[-] Failed to init GLEW: %S"), glewGetErrorString(err));
        return FALSE;
    }

    //
    // During init, enable debug output
    //
    glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(&stGlMessageCallback), this);

    char* glVersion = (char*)glGetString(GL_VERSION);

    if (glVersion) {

        LogMessage(_T("[*] OpenGL version: %S"), glVersion);
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

    //
    // Instantiate and load models
    //
    /*if (m_SampleModel.loadModel(_T("data/crank.obj")) != TRUE)
    {
        LogMessage(_T("[-] Failed to load model 0"));
    }*/


    glfwWindowHint(GLFW_VISIBLE, TRUE);
    
    window->ShowWindow(SW_SHOW);

    m_pGlWindow = pGlWindow;
    //glfwMakeContextCurrent(NULL);

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
    m_parent->Invalidate(FALSE);
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
        m_parent->Invalidate(FALSE);
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
CRenderer::drawCircle3f(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(cx, cy, 0.0f);  // Центр круга

    for (int i = 0; i <= num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);  // Угол
        float x = r * cosf(theta);  // Координата X
        float y = r * sinf(theta);  // Координата Y

        glVertex3f(x + cx, y + cy, 0.0f);  // Вершина на окружности
    }

    glEnd();
}

void
CRenderer::drawLine3f(float cx, float cy, float cx2, float cy2)
{
    glBegin(GL_LINES);

    glVertex3f(cx, cy, 0.0f);  // начало
    glVertex3f(cx2, cy2, 0.0f);  // конец

    glEnd();
}

void
CRenderer::Draw()
{
    if (!m_pGlWindow)
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
    glm::vec3 light_ambient = glm::vec3(0.4, 0.4, 0.4);
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



    m_DefaultShader.use();


    m_DefaultShader.setMat4("projection", proj_mat);
    m_DefaultShader.setMat4("view", view_mat);
    m_DefaultShader.setMat4("model", glm::mat4(1.0));

    this->drawCircle3f(0.0, 0.0, 0.5, 100);

    this->drawLine3f(0.5, 0.5, 0.8, 0.8);

    //m_SampleModel.Draw(m_DefaultShader);


    glfwSwapBuffers(m_pGlWindow);
}

