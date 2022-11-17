#pragma once
#include <vector>
#include <glm/glm.hpp>

#define HEIGHT_MIN -0.5f
#define HEIGHT_MAX 0.5f

#define EPSILON_OFF 0.001

/*
 * an abstract class representing a general trimesh
*/
class TrimeshImplicit {
    public:

        std::vector<float> m_vertexData; // the trimesh's vertex data
        int m_param1; // vertical tesselation
        int m_param2; // circular tesselation


        // static helper function to insert a vec3 into the vertex data
        static inline void insertVec3(std::vector<float> &data, glm::vec3 v) {
            data.push_back(v.x);
            data.push_back(v.y);
            data.push_back(v.z);
        }

        // called when params are updated, creates the trimesh
        void UpdateParams(int p1, int p2) {
            m_param1 = p1;
            m_param2 = p2;
            m_vertexData = std::vector<float>();
            SetVertexData();
        }

         // function for the shape's "side tile", using the shape-specific normal function
        inline void MakeSideTile(glm::vec3 topLeft,
                                 glm::vec3 topRight,
                                 glm::vec3 bottomLeft,
                                 glm::vec3 bottomRight) {
            auto tlNorm = ShapeNormal(topLeft);
            auto trNorm = ShapeNormal(topRight);
            auto blNorm = ShapeNormal(bottomLeft);
            auto brNorm = ShapeNormal(bottomRight);

            insertVec3(m_vertexData, topLeft);
            insertVec3(m_vertexData, tlNorm);
            insertVec3(m_vertexData, bottomLeft);
            insertVec3(m_vertexData, blNorm);
            insertVec3(m_vertexData, topRight);
            insertVec3(m_vertexData, trNorm);

            insertVec3(m_vertexData, topRight);
            insertVec3(m_vertexData, trNorm);
            insertVec3(m_vertexData, bottomLeft);
            insertVec3(m_vertexData, blNorm);
            insertVec3(m_vertexData, bottomRight);
            insertVec3(m_vertexData, brNorm);
        }

         // function for a shape's flat tile, using a flat normal
        inline void MakeFlatCapTile(glm::vec3 topLeft,
                                    glm::vec3 topRight,
                                    glm::vec3 bottomLeft,
                                    glm::vec3 bottomRight) {
          // t1 - tl, bl, tr
          // t2 - tr, bl, br
          auto tlNorm = glm::normalize(glm::cross(bottomLeft - topLeft, topRight - topLeft));
          auto trNorm = glm::normalize(glm::cross(topLeft - topRight, bottomLeft - topRight));
          auto blNorm = glm::normalize(glm::cross(topRight - bottomLeft, topLeft - bottomLeft));
          auto brNorm = glm::normalize(glm::cross(topRight - bottomRight, bottomLeft - bottomRight));

          insertVec3(m_vertexData, topLeft);
          insertVec3(m_vertexData, tlNorm);
          insertVec3(m_vertexData, bottomLeft);
          insertVec3(m_vertexData, blNorm);
          insertVec3(m_vertexData, topRight);
          insertVec3(m_vertexData, trNorm);

          insertVec3(m_vertexData, topRight);
          insertVec3(m_vertexData, trNorm);
          insertVec3(m_vertexData, bottomLeft);
          insertVec3(m_vertexData, blNorm);
          insertVec3(m_vertexData, bottomRight);
          insertVec3(m_vertexData, brNorm);
        }


        // VIRTUAL FUNCTIONS to be inherited by derived classes

        virtual void SetVertexData() = 0; // function set the vertex data, effectively rebuilding the trimesh

        virtual ~TrimeshImplicit() = default; // destructor

        virtual inline glm::vec3 ShapeNormal(glm::vec3& xyz) = 0; // the trimesh's custom normal function, if applicable


};
