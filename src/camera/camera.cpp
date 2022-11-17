#include <stdexcept>
#include "camera.h"
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

    auto c = -Camera::nearPlane / Camera::farPlane;
    auto unhingingMatrix = glm::mat4(
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1/(1+c), -1.f,
                    0.f, 0.f, -c/(1+c), 0.f);

    float tanHalfHeightAngle = glm::tan(Camera::camData.heightAngle / 2.f);

    // tan (theta_h / 2) = (height / 2) / k
    float k = static_cast<float>(Camera::height / 2.f) / tanHalfHeightAngle;

    // tan(theta_w / 2) = (width / 2) / k
    float tanHalfWidthAngle = static_cast<float>(Camera::width / 2.f) / k;

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
    Camera::viewProjMatrix = Camera::viewMatrix * projMatrix; // MVP = V * P * M

}

void Camera::updateViewPlanes(float farPlaneNew, float nearPlaneNew) {
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

glm::mat4 Camera::getViewProjMatrix() const {
    return Camera::viewProjMatrix;
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
