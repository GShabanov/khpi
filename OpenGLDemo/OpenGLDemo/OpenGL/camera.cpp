#include "pch.h"

#include "camera.h"

#include "GlUtils.h"

CCamera::CCamera(void)
{
    updateCameraVectors();
}

void CCamera::updateCameraVectors(void)
{
    m_position_coords = glm::vec3(
        glm::cos(glm::radians(m_position_xangle)) * glm::cos(glm::radians(m_position_yangle)),
        glm::sin(glm::radians(m_position_yangle)),
        glm::sin(glm::radians(m_position_xangle)) * glm::cos(glm::radians(m_position_yangle))
    );

    m_position_coords *= glm::vec3((float)m_distance);

    glm::vec3 front_vec = glm::normalize(m_position_coords) * glm::vec3(-1.0);
    glm::vec3 right_vec = glm::normalize(glm::cross(front_vec, m_WORLD_UP_VEC));

    m_up_vec = glm::normalize(glm::cross(right_vec, front_vec));

    // build light direction matrix
    /*glm::mat4 light_pos = glm::mat4(
        glm::vec4(right_vec, 0.0f),  // righ vector
        glm::vec4(m_up_vec, 0.0f),   // up vector
        glm::vec4(-front_vec, 0.0f),  // front vector
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    light_pos = glm::rotate(light_pos, 0.3f, glm::vec3(0.0, 1.0, 0.0));
    glm::vec4 light_dir_homogeneous = light_pos * glm::vec4(m_position_coords, 1.0f);
    m_lightVector = light_dir_homogeneous;*/

    glm::mat4 light_pos = glm::lookAt(m_position_coords, glm::vec3(0.0, 0.0, 0.0), m_up_vec);
    light_pos = glm::rotate(light_pos, 0.6f, glm::vec3(0.0, 1.0, 0.0));
    light_pos = glm::inverse(light_pos);    

    m_lightVector = glm::vec3(light_pos * glm::vec4(1.0));
}

void
CCamera::orbit(double x_offset, double y_offset)
{
    m_position_xangle += x_offset * m_mouse_sensitivity;
    m_position_yangle -= y_offset * m_mouse_sensitivity;

    while (m_position_xangle >= 360.0)
    {
        m_position_xangle -= 360.0;
    }

    while (m_position_xangle <= 0.0)
    {
        m_position_xangle += 360.0;
    }

    if (m_position_yangle >= 90.0)
    {
        m_position_yangle = 89.0;
    }

    if (m_position_yangle <= -90.0)
    {
        m_position_yangle = -89.0;
    }

    updateCameraVectors();
}

void
CCamera::zoom(double y_offset)
{
    m_distance -= (float)(y_offset * m_zoom_sensitivity);

    if (m_distance < 1.0)
    {
        m_distance = 1.0;
    }

    updateCameraVectors();
}

void
CCamera::widenFov(void)
{
    if (m_fov + m_fov_sensitivity <= 90.0f)
        m_fov += m_fov_sensitivity;
}

void
CCamera::narrowFov(void)
{
    if (m_fov - m_fov_sensitivity >= 30.0f)
        m_fov -= m_fov_sensitivity;
}

void
CCamera::setScreenDimensions(unsigned int width, unsigned int height)
{
    m_screen_width  = width;
    m_screen_height = height;
}

void
CCamera::getScreenDimensions(unsigned int &width, unsigned int &height)
{
    width = m_screen_width;
    height = m_screen_height;
}

void 
CCamera::setSensitivities(float mouse_sensitivity, float zoom_sensitivity, float fov_sensitivity)
{
    m_mouse_sensitivity = mouse_sensitivity;
    m_zoom_sensitivity  = zoom_sensitivity;
    m_fov_sensitivity   = fov_sensitivity;
}

glm::mat4 
CCamera::getViewMatrix(void)
{
    return glm::lookAt(m_position_coords, glm::vec3(0.0, 0.0, 0.0), m_up_vec);
}

glm::mat4
CCamera::getProjMatrix(void)
{
    return glm::perspective(glm::radians((float)m_fov), (float)m_screen_width/(float)m_screen_height, 0.1f, 100.0f);
}

