#pragma once

#include <glm/glm.hpp>
#include "../utils/scenedata.h"


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
