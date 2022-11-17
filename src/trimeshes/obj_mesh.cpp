#include "obj_mesh.h"


/*
 *  get type
*/
PrimitiveType OBJMesh::GetType() {
    return PrimitiveType::PRIMITIVE_MESH;
}

bool OBJMesh::LoadOBJ(std::string filepath) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

    std::vector<glm::vec3> verticesRead;
    std::vector< glm::vec2> uvRead;
    std::vector<glm::vec3> normalsRead;

    // read in the obj file (i only know how to do this in C)
    FILE* file = fopen(filepath.c_str(), "r");
    if (file == NULL){
        std::cout <<  "Could not open OBJ file." << std::endl;
        return false;
    }


    return true;
}
