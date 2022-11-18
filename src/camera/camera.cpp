#include <iostream>
#include <ostream>
#include <stdexcept>
#include "camera.h"
//#include "glm/ext/matrix_clip_space.hpp"
//#include "glm/ext/matrix_transform.hpp"
#include "transforms.h"

void Camera::setViewMatrices() {

    auto w = glm::normalize(-1.f * look);

    auto vTop = Camera::up - (glm::dot(Camera::up, w) * w);
    auto v =  glm::normalize(vTop);

    auto u = glm::cross(v, w);

    auto Mtranslate = getTranslationMatrix(-1 * Camera::pos[0], -1 * Camera::pos[1], -1 * Camera::pos[2]);
    auto Mrotate = glm::mat4(
                u[0], v[0], w[0], 0.f,
                u[1], v[1], w[1], 0.f,
                u[2], v[2], w[2], 0.f,
                0.f, 0.f, 0.f, 1.f
            );

    auto viewM = Mrotate * Mtranslate;

    Camera::viewMatrix = viewM;
    Camera::invViewMatrix = glm::inverse(viewM);


//    std::cout << Camera::farPlane << std::endl;
//    std::cout << Camera::nearPlane << std::endl;

//    std::cout << "set view matrices" << std::endl;

//    std::cout << farPlaneNew << std::endl;
//    std::cout << nearPlaneNew << std::endl;

    float c = -Camera::nearPlane / Camera::farPlane;
    auto unhingingMatrix = glm::mat4(
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1.f/(1.f+c), -1.f,
                    0.f, 0.f, -c/(1.f+c), 0.f);

//    std::cout << 1.f/(1.f+c) << std::endl;
//    std::cout << -c/(1.f+c) << std::endl;

    float tanHalfHeightAngle = glm::tan(Camera::camData.heightAngle / 2.f);

    // tan (theta_h / 2) = (height / 2) / k
    float k = static_cast<float>(Camera::height / 2.f) / tanHalfHeightAngle;

    // tan(theta_w / 2) = (width / 2) / k
    float tanHalfWidthAngle = static_cast<float>(Camera::width / 2.f) / k;

//    std::cout << tanHalfWidthAngle << "half width tan" << std::endl;

//    std::cout << Camera::farPlane << std::endl;

    auto scaleMatrix = glm::mat4(
                        1.f/(Camera::farPlane * tanHalfWidthAngle), 0.f, 0.f, 0.f,
                        0.f, 1.f/(Camera::farPlane * tanHalfHeightAngle), 0.f, 0.f,
                        0.f, 0.f, 1.f/Camera::farPlane, 0.f,
                        0.f, 0.f, 0.f, 1.f);

    glm::mat4 zRemapMatrix = glm::mat4(
                        1.f, 0.f, 0.f, 0.f,
                        0.f, 1.f, 0.f, 0.f,
                        0.f, 0.f, -2.f, 0.f,
                        0.f, 0.f, -1.f, 1.f);

    Camera::projMatrix = zRemapMatrix * unhingingMatrix * scaleMatrix;
//    Camera::projMatrix = glm::perspective(glm::degrees(Camera::camData.heightAngle), aspectRatio, Camera::nearPlane, Camera::farPlane);
//    Camera::viewMatrix = glm::lookAt(Camera::pos, Camera::look, Camera::up);
    Camera::projViewMatrix =  Camera::projMatrix * Camera::viewMatrix; // MVP = P * V * M
}

void Camera::updateViewPlanes(float farPlaneNew, float nearPlaneNew) {

    std::cout << Camera::farPlane << std::endl;
    std::cout << Camera::nearPlane << std::endl;

    std::cout << "passed" << std::endl;

    std::cout << farPlaneNew << std::endl;
    std::cout << nearPlaneNew << std::endl;

    Camera::farPlane = farPlaneNew;
    Camera::nearPlane = nearPlaneNew;

    Camera::setViewMatrices();
}

void Camera::updateAspectRatio(float aspectRatio) {
    Camera::aspectRatio = aspectRatio;

    setViewMatrices();
}

glm::mat4 Camera::getViewMatrix() const {
    return Camera::viewMatrix;
}

glm::mat4 Camera::getInvViewMatrix() const {
    return Camera::invViewMatrix;
}

glm::mat4 Camera::getProjMatrix() const {
    return Camera::projMatrix;
}

glm::mat4 Camera::getProjViewMatrix() const {
    return Camera::projViewMatrix;
}

float Camera::getAspectRatio() const {
    return Camera::aspectRatio;
}

float Camera::getHeightAngle() const {
    return Camera::camData.heightAngle;
}

float Camera::getFocalLength() const {
    return Camera::camData.focalLength;
}

float Camera::getAperture() const {
    return Camera::camData.aperture;
}
