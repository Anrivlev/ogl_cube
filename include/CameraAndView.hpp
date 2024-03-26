#pragma once
#include <iostream>
#include <set>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "Texture.hpp"

typedef struct CameraUVN
{
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 left_;
    glm::vec3 position_;
} CameraUVN;

class CameraAndView
{
public:
    int windowWidth_;
    int windowHeight_;
    float fov_;
    float near_;
    float far_;

    float cameraDistance_;
    float theta_;
    float phi_;
    float cameraSpeed_;
    float cameraZoomSpeed_;
    float maxTheta_ = 80.0f;
    float minTheta_ = 0.0f;
    CameraUVN camera;
    glm::vec3 sceneCenter = glm::vec3(0.0f);

    GLuint transformationMatrix_;
    GLuint SamplerLocation;

    static int cameraPrevPosX;
    static int cameraPrevPosY;
    static int cameradx;
    static int camerady;

public:
    CameraAndView(int windowWidth,
                  int windowHeight,
                  float fov,
                  float near,
                  float far,
                  float cameraDistance_ = 5.0f,
                  float theta = 0.0f,
                  float phi = 0.0f) : windowWidth_{windowWidth},
                                      windowHeight_(windowHeight),
                                      fov_(fov),
                                      near_(near),
                                      far_(far),
                                      cameraDistance_(cameraDistance_),
                                      theta_(theta),
                                      phi_(phi)
    {
        cameraSpeed_ = 0.25f;
        cameraZoomSpeed_ = 0.25f;

        theta_ = std::max(theta_, minTheta_);
        theta_ = std::min(theta_, maxTheta_);
        camera = CalculateCameraUVN();
    };

    void OnSpecialKeyboard(int key, int x, int y);
    void OnKeyboard(unsigned char key, int mouse_x, int mouse_y);
    void OnMouseWheel(int button, int dir, int x, int y);
    void OnMouse(int button, int state, int x, int y);
    void OnMouseActiveMove(int x, int y);

    glm::mat4 ProjectionMat();
    glm::mat4 CameraMat();

    CameraUVN CalculateCameraUVN();

    ~CameraAndView()
    {
    }

private:
    
};
