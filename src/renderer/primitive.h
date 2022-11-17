#pragma once

#include "trimeshes/trimesh.h"
#include "utils/scenedata.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct MeshPrimitive {
    TrimeshData* trimesh;
    PrimitiveType type;
    SceneMaterial* material;
    glm::mat4 modelMatrix;
    glm::mat4 invTransposeModelMatrix;
};

