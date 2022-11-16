#include "transforms.h"

glm::mat4 getScalingMatrix(float sx, float sy, float sz) {
    // Task 1: return the transformation matrix for 3D scaling
    // Task 4: modify the function to return a 4x4 matrix
    return glm::mat4(sx, 0, 0, 0,
                     0, sy, 0, 0,
                     0, 0, sz, 0,
                     0, 0, 0, 1.f);
}

glm::mat4 getRotationMatrixX(float angle) {
    // Task 2: return the matrix for rotation about the x-axis
    auto theta = glm::radians(angle);
    // Task 4: modify the function to return a 4x4 matrix
    return glm::mat4(1.f, 0, 0, 0,
                     0, cos(theta), sin(theta), 0,
                     0, -1 * sin(theta), cos(theta), 0,
                     0, 0, 0, 1.f);
}

glm::mat4 getRotationMatrixY(float angle) {
    // Task 2: return the matrix for rotation about the y-axis
    // Task 4: modify the function to return a 4x4 matrix
    auto theta = glm::radians(angle);
    return glm::mat4(cos(theta), 0, -1 * sin(theta), 0,
                     0, 1.f, 0, 0,
                     sin(theta), 0, cos(theta), 0,
                     0,0,0,1.f);
}

glm::mat4 getRotationMatrixZ(float angle) {
    // Task 2: return the matrix for rotation about the z-axis
    // Task 4: modify the function to return a 4x4 matrix
    auto theta = glm::radians(angle);
    return glm::mat4(cos(theta), sin(theta), 0, 0,
                     -1 * sin(theta), cos(theta), 0, 0,
                     0, 0, 1.f, 0,
                     0, 0, 0, 1.f);
}

glm::mat4 getTranslationMatrix(float dx, float dy, float dz) {
    // Task 3: return the matrix for translation
    return glm::mat4(1.f, 0, 0, 0,
                     0, 1.f, 0, 0,
                     0, 0, 1.f, 0,
                     dx, dy, dz, 1);
}

