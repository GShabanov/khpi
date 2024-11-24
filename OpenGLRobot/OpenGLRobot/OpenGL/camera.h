#pragma once

#ifndef __OPENGL_CAMERA_H__
#define __OPENGL_CAMERA_H__

#include "glm/glm.hpp"

class CCamera
{
private:
    const glm::vec3 m_WORLD_UP_VEC = glm::vec3(0.0, 1.0, 0.0);

    // Camera view state attributes
    double        m_fov             = 45.0;
    double        m_distance        = 10.0;

    double        m_position_xangle = 90.0;
    double        m_position_yangle = 30.0;

    glm::vec3     m_position_coords = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3     m_up_vec          = glm::vec3(0.0, 1.0, 0.0);

    glm::vec3     m_lightVector;
    //
    // Sensitivities
    //
    double m_mouse_sensitivity = 0.3;
    double m_zoom_sensitivity = 0.5;
    double m_fov_sensitivity = 0.4;

    //
    // Screen
    //
    unsigned int m_screen_width = 800;
    unsigned int m_screen_height = 640;

    void updateCameraVectors(void);

public:
    CCamera(void);

    void orbit(double x_offset, double y_offset);
    void zoom(double y_offset);
    void widenFov();
    void narrowFov();

    void setScreenDimensions(unsigned int width, unsigned int height);
    void getScreenDimensions(unsigned int& width, unsigned int& height);

    void setSensitivities(float mouse_sensitivity, float zoom_sensitivity, float fov_sensitivity);

    glm::mat4 getViewMatrix(void);
    glm::mat4 getProjMatrix(void);
    glm::vec3 getPosition(void) { return m_position_coords; }
    glm::vec3 getLightVector(void) { return m_lightVector; }

    double getDistance() { return m_distance; };
};

#endif // __OPENGL_CAMERA_H__
