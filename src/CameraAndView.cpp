#include <cmath>
#include <algorithm>
#include "../include/CameraAndView.hpp"

int CameraAndView::cameraPrevPosX = -1;
int CameraAndView::cameraPrevPosY = -1;
int CameraAndView::cameradx = -1;
int CameraAndView::camerady = -1;

glm::mat4 CameraAndView::ProjectionMat()
{
    float r = (float)windowWidth_ / windowHeight_;
    float f = 1 / tanf(fov_ / 2);
    float a = (far_ + near_) / (far_ - near_);
    float b = -2 * far_ * near_ / (far_ - near_);

    glm::mat4 Projection(f / r, 0.0f, 0.0f, 0.0f,
                         0.0f, f, 0.0f, 0.0f,
                         0.0f, 0.0f, -a, b,
                         0.0f, 0.0f, -1.0, 0.0f);

    return Projection;
}

CameraUVN CameraAndView::CalculateCameraUVN()
{
    float sint = sinf(theta_ * M_PI / 180.f);
    float cost = cosf(theta_ * M_PI / 180.f);
    float sinp = sinf(phi_ * M_PI / 180.f);
    float cosp = cosf(phi_ * M_PI / 180.f);

    CameraUVN camerauvn;
    float y = cameraDistance_ * cost;
    float x = cameraDistance_ * sint * cosp;
    float z = cameraDistance_ * sint * sinp;
    camerauvn.position_ = this->sceneCenter + glm::vec3(x, y, z);

    camerauvn.front_ = glm::normalize(glm::vec3(-sint * cosp, -cost, -sint * sinp));
    camerauvn.up_ = glm::normalize(glm::vec3(-cost * cosp, sint, -cost * sinp));
    camerauvn.left_ = glm::normalize(glm::vec3(sinp, 0, -cosp));

    return camerauvn;
}

glm::mat4 CameraAndView::CameraMat()
{
    glm::vec3 N = camera.front_;
    glm::vec3 V = camera.up_;
    glm::vec3 U = camera.left_;

    glm::mat4 CameraMat(U.x, U.y, U.z, glm::dot(-U, camera.position_),
                        V.x, V.y, V.z, glm::dot(-V, camera.position_),
                        -N.x, -N.y, -N.z, glm::dot(N, camera.position_),
                        0.0f, 0.0f, 0.0f, 1.0f);

    return CameraMat;
}

void CameraAndView::OnSpecialKeyboard(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
    {
        glm::vec3 ey = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 dpos = glm::cross(ey, camera.left_) * cameraSpeed_;
        this->sceneCenter += dpos;
        camera = CalculateCameraUVN();
    }
    break;
    case GLUT_KEY_DOWN:
    {
        glm::vec3 ey = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 dpos = glm::cross(ey, camera.left_) * cameraSpeed_;
        this->sceneCenter -= dpos;
        camera = CalculateCameraUVN();
    }
    break;
    case GLUT_KEY_LEFT:
    {
        glm::vec3 dpos = (camera.left_ * cameraSpeed_);
        this->sceneCenter -= dpos;
        camera = CalculateCameraUVN();
    }
    break;
    case GLUT_KEY_RIGHT:
    {
        glm::vec3 dpos = (camera.left_ * cameraSpeed_);
        this->sceneCenter += dpos;
        camera = CalculateCameraUVN();
    }
    break;
    default:
        break;
    }
}

void CameraAndView::OnMouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
        cameraDistance_ += cameraZoomSpeed_;
    else
        cameraDistance_ = std::max(0.f, cameraDistance_ - cameraZoomSpeed_);

    camera = CalculateCameraUVN();
}

void CameraAndView::OnMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            cameraPrevPosX = x;
            cameraPrevPosY = y;
        }
        else
        {
            cameraPrevPosX = -1;
            cameraPrevPosY = -1;
        }
    }
}

void CameraAndView::OnMouseActiveMove(int x, int y)
{
    if (cameraPrevPosX != -1 && cameraPrevPosY != -1)
    {
        cameradx = x - cameraPrevPosX;
        camerady = y - cameraPrevPosY;
        cameraPrevPosX = x;
        cameraPrevPosY = y;

        if (cameradx != 0)
            phi_ += cameradx;

        if (camerady != 0 && theta_ - camerady < maxTheta_ && theta_ - camerady > minTheta_)
            theta_ -= camerady;

        camera = CalculateCameraUVN();
    }
}

void CameraAndView::OnKeyboard(unsigned char key, int mouse_x, int mouse_y)
{
    switch (key)
    {
    case 'q':
    case 27: // escape key code
    {
        glutLeaveMainLoop();
    }
    break;

        break;
    }
}
