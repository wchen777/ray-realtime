#pragma once

#include <glm/glm.hpp>
#include "../utils/scenedata.h"
#include "transforms.h"


// A class representing a virtual camera.

// Feel free to make your own design choices for Camera class, the functions below are all optional / for your convenience.
// You can either implement and use these getters, or make your own design.
// If you decide to make your own design, feel free to delete these as TAs won't rely on them to grade your assignments.

class Camera {
public:

//    Camera(); // default constructor

    Camera(SceneCameraData camData, int height, int width, float farPlane, float nearPlane) :
       look{glm::vec3(camData.look)},
       pos{glm::vec3(camData.pos)},
       up{glm::vec3(camData.up)},
       camData{camData},
       aspectRatio{static_cast<float>(width) / static_cast<float>(height)},
       farPlane(farPlane),
       nearPlane(nearPlane),
       width(width),
       height(height)
    {
        setViewMatrices();
    }

    glm::vec3 up;
    glm::vec3 look;
    glm::vec3 pos;

    float aspectRatio; // this is width / height!!
    float farPlane;
    float nearPlane;

    int height;
    int width;

    SceneCameraData camData;

    glm::mat4 invViewMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    glm::mat4 projViewMatrix; // PV matrix

    void setViewMatrices(); // set the view matrix fields

    void updateViewPlanes(float farPlaneNew, float nearPlaneNew); // when the far and new planes are updated

    // Returns the view matrix for the current camera settings.
    // You might also want to define another function that return the inverse of the view matrix.
    glm::mat4 getViewMatrix() const;
    glm::mat4 getInvViewMatrix() const;
    glm::mat4 getProjMatrix() const;
    glm::mat4 getProjViewMatrix() const;

    glm::mat4 currentTranslation = glm::mat4(1.f); // current translation for rebuilding

    void updateViewMatricesTranslate(glm::mat4& translate); // helper for camera transformation

    // W: Translates the camera in the direction of the look vector
    void WPressed(float speed);

    // S: Translates the camera in the opposite direction of the look vector
    void SPressed(float speed);

    // A: Translates the camera in the left direction, perpendicular to the look and up vectors
    void APressed(float speed);

    // D: Translates the camera in the right direction, also perpendicular to the look and up vectors.
    // This movement should be opposite to that of pressing A
    void DPressed(float speed);

    // Space: Translates the camera along the world space vector (0,1,0)
    void SpacePressed(float speed);

    // Ctrl: Translates the camera along the world space vector (0,-1,0)
    void CtrlPressed(float speed);

    // update aspect ratio for resize
    void updateAspectRatio(float aspectRatio);

    // Returns the aspect ratio of the camera.
    float getAspectRatio() const;

    // Returns the height angle of the camera in RADIANS.
    float getHeightAngle() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture() const;
};
