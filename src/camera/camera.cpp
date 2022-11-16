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
                u[0], v[0], w[0], 0,
                u[1], v[1], w[1], 0,
                u[2], v[2], w[2], 0,
                0, 0, 0, 1.f
            );

    auto viewM = Mrotate * Mtranslate;

    Camera::viewMatrix = viewM;
    Camera::invViewMatrix = glm::inverse(viewM);

    auto c = -Camera::nearPlane / Camera::farPlane;
    auto perspectiveMatrix = glm::mat4(
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1/(1+c), -1.f,
                    0.f, 0.f, -c/(1+c), 0.f
                 );

    Camera::projMatrix = glm::mat4(


                );
}

glm::mat4 Camera::getViewMatrix() const {
    return Camera::viewMatrix;
}

glm::mat4 Camera::getInvViewMatrix() const {
    return Camera::invViewMatrix;
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
