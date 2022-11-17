#include "sphere.h"

/*
 *  get type
*/
PrimitiveType Sphere::GetType() {
    return PrimitiveType::PRIMITIVE_SPHERE;
}


/*
 * shape specific normal
*/
inline glm::vec3 Sphere::ShapeNormal(glm::vec3& xyz) {
    return glm::normalize(xyz);
}


/*
 * call the create sphere function
*/
void Sphere::SetVertexData(){
    Sphere::MakeSphere();
}


/*
 * create the sphere by filling m vertex data
*/
void Sphere::MakeSphere() {
    // per angle step
    float thetaStep = glm::radians(360.f / m_param2);

    // create a slice
    for (int t = 0; t < m_param2; ++t) {
        Sphere::MakeWedge(thetaStep * t, thetaStep * (t + 1));
    }

}

/*
 * createa single wedge of the sphere
*/
void Sphere::MakeWedge(float currentTheta, float nextTheta) {
    float phiStep = glm::radians(180.f / m_param1);

    for (int p = 0; p < m_param1; ++p) {

        auto startPhi = phiStep * p;
        auto endPhi = phiStep * (p+1);

        auto sinStartPhi = glm::sin(startPhi);
        auto sinEndPhi = glm::sin(endPhi);
        auto cosStartPhi = glm::cos(startPhi);
        auto cosEndPhi = glm::cos(endPhi);

        auto sinCurrentTheta = glm::sin(currentTheta);
        auto cosCurrentTheta = glm::cos(currentTheta);
        auto sinNextTheta = glm::sin(nextTheta);
        auto cosNextTheta = glm::cos(nextTheta);

        auto tlX = sinStartPhi * sinCurrentTheta;
        auto tlY = cosStartPhi;
        auto tlZ = sinStartPhi * cosCurrentTheta;

        auto trX = sinStartPhi * sinNextTheta;
        auto trY = cosStartPhi;
        auto trZ = sinStartPhi * cosNextTheta;

        auto blX = sinEndPhi * sinCurrentTheta;
        auto blY = cosEndPhi;
        auto blZ = sinEndPhi * cosCurrentTheta;

        auto brX = sinEndPhi * sinNextTheta;
        auto brY = cosEndPhi;
        auto brZ = sinEndPhi * cosNextTheta;

        // this uses the sphere's custom normal
        Sphere::MakeSideTile(glm::vec3(tlX, tlY, tlZ), glm::vec3(trX, trY, trZ), glm::vec3(blX, blY, blZ), glm::vec3(brX, brY, brZ));

    }
}
