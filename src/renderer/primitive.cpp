#include "realtime.h"
#include "primitive.h"
#include "settings.h"

#include "../trimeshes/cube.h"
#include "../trimeshes/cone.h"
#include "../trimeshes/sphere.h"
#include "../trimeshes/cylinder.h"

/*
 * turn the parsed shapes into scene primitive mesh structs.
 * assumes scene has been parsed,
 * populates field within Realtime
*/
void Realtime::CompilePrimitiveMeshes() {
    auto objs = Realtime::sceneRenderData.shapes;

    // clear existing vector for mesh data
    Realtime::objectMeshes.clear();

    // go through each object and create a trimesh for them
    for (auto& obj : objs) {

        MeshPrimitive prim = MeshPrimitive{}; // the primitive

        TrimeshImplicit * trimesh; // the trimesh abstract class

        switch (obj.primitive.type) {
        case PrimitiveType::PRIMITIVE_CUBE:
            trimesh = new Cube();
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            trimesh = new Cylinder();
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            trimesh = new Sphere();
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            trimesh = new Cone();
            break;
        default:
            trimesh = new Cube();
            break;
        }

        // this builds the trimesh.
        trimesh->UpdateParams(settings.shapeParameter1, settings.shapeParameter2);

        // set all remaining fields
        prim.trimesh = trimesh;
        prim.type = obj.primitive.type;
        prim.invTransposeModelMatrix = glm::inverse(glm::transpose(obj.ctm));
        prim.modelMatrix = obj.ctm;
        prim.material = &obj.primitive.material;

        Realtime::objectMeshes.emplace_back(prim);
    }

}

/*
 * update tesselation parameters for all the trimeshes.
*/
void Realtime::UpdateTesselations() {
    auto param1 = settings.shapeParameter1;
    auto param2 = settings.shapeParameter2;

    for (auto& trimesh : Realtime::objectMeshes) {
        trimesh.trimesh->UpdateParams(param1, param2);
    }
}

/*
 * free all allocated mesh objects
*/
void Realtime::DestroyMeshes() {
    for (auto& trimesh : Realtime::objectMeshes) {
        delete trimesh.trimesh;
    }
}
