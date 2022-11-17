#include "realtime.h"

#define MAX_LIGHTS 8

/*
 * go through each mesh object struct and generate and bind the VBOs and VAOs
*/
void Realtime::InitializeBuffers() {
    this->makeCurrent();

    // generate VBO and VAO for every mesh
    for (MeshPrimitive& mesh : Realtime::objectMeshes) {
        // first, generate a VBO
        glGenBuffers(1, &mesh.vbo);

        // bind that VBO to the state machine
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

        // Send data to VBO
        glBufferData(GL_ARRAY_BUFFER, mesh.trimesh->m_vertexData.size() * sizeof(GLfloat), mesh.trimesh->m_vertexData.data(), GL_STATIC_DRAW);

        // Generate, and bind VAO
        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        // Enable and define attribute 0 to store vertex positions (vec3)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(0));

        // Enable and define attribute 1 to store vertex normals (vec3)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

        // Clean-up bindings for VBO and VAO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    this->doneCurrent();
}

/*
 * initialize the uniform associated with the lights in the scene that are separate from each individual object
*/
void Realtime::InitializeLightUniforms() {
    this->makeCurrent();

    // GLOBAL WEIGHTS

    // pass m_ka into the fragment shader as a uniform
    GLint ka_loc = glGetUniformLocation(Realtime::shader, "k_a");
    glUniform1f(ka_loc, Realtime::sceneRenderData.globalData.ka);

    // pass m_kd into the fragment shader as a uniform
    GLint kd_loc = glGetUniformLocation(Realtime::shader, "k_d");
    glUniform1f(kd_loc, Realtime::sceneRenderData.globalData.kd);

    // pass k_s as a uniform
    GLint ks_loc = glGetUniformLocation(Realtime::shader, "k_s");
    glUniform1f(ks_loc, Realtime::sceneRenderData.globalData.ks);

    int count = 0;
    // for each light data, up until 8
    for (auto& light : Realtime::sceneRenderData.lights) {
        if (count == MAX_LIGHTS) {
            break;
        }

        // light pos uniform
        GLint light_pos_loc = glGetUniformLocation(Realtime::shader, ("light_positions[" + std::to_string(count) + "]").c_str());
        glUniform3fv(light_pos_loc, 1, &light.pos[0]);

        // light dir uniform
        GLint light_dir_loc = glGetUniformLocation(Realtime::shader, ("light_dirs[" + std::to_string(count) + "]").c_str());
        glUniform3fv(light_dir_loc, 1, &light.dir[0]);

        // light color uniform
        GLint light_colors_loc = glGetUniformLocation(Realtime::shader, ("light_colors[" + std::to_string(count) + "]").c_str());
        glUniform3fv(light_colors_loc, 1, &light.color[0]);

        // light type uniform
        GLint light_type_loc = glGetUniformLocation(Realtime::shader, ("light_types[" + std::to_string(count) + "]").c_str());
        glUniform1i(light_type_loc, static_cast<std::underlying_type_t<LightType>>(light.type));
    }


    this->doneCurrent();
}

/*
 * initialize the uniform associated with the camera in the scene that are separate from each individual object
*/
void Realtime::InitializeCameraUniforms() {
    this->makeCurrent();

    // pass in VP matrix as a uniform (VP is already calculated in camera)
    GLint VP_mat_loc = glGetUniformLocation(Realtime::shader, "VP_matrix");
    glUniformMatrix4fv(VP_mat_loc, 1, GL_FALSE, &Realtime::sceneCamera.getViewProjMatrix()[0][0]);

    GLint cam_pos_loc = glGetUniformLocation(Realtime::shader, "cam_pos");
    glUniform3fv(cam_pos_loc, 1, &Realtime::sceneCamera.pos[0]);

    this->doneCurrent();
}

/*
 * go through each mesh object struct and draw them on the screen, initializing the correct uniform variables (per object)
*/
void Realtime::DrawBuffers() {
    this->makeCurrent();

    // initialize uniforms, draw the object
    for (MeshPrimitive& mesh : Realtime::objectMeshes) {
        // bind the object's vao
        glBindVertexArray(mesh.vao);

        // pass in model matrix as a uniform
        GLint model_mat_loc = glGetUniformLocation(Realtime::shader, "model_matrix");
        glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, &mesh.modelMatrix[0][0]);

        // pass in inv transpose model matrix as a uniform
        GLint inv_transpose_model_mat_loc = glGetUniformLocation(Realtime::shader, "inv_trans_model_matrix");
        glUniformMatrix4fv(inv_transpose_model_mat_loc, 1, GL_FALSE, &mesh.invTransposeModelMatrix[0][0]);

        // pass in shininess
        GLint shininess_loc = glGetUniformLocation(Realtime::shader, "shininess");
        glUniform1f(shininess_loc, mesh.material->shininess);

        // pass in cAmbient
        GLint cAmbient_loc = glGetUniformLocation(Realtime::shader, "cAmbient");
        glUniform3fv(cAmbient_loc, 1, &mesh.material->cAmbient[0]);

        // pass in cDiffuse
        GLint cDiffuse_loc = glGetUniformLocation(Realtime::shader, "cDiffuse");
        glUniform3fv(cDiffuse_loc, 1, &mesh.material->cDiffuse[0]);

        // pass in cSpecular
        GLint cSpecular_loc = glGetUniformLocation(Realtime::shader, "cSpecular");
        glUniform3fv(cSpecular_loc, 1, &mesh.material->cSpecular[0]);

        // draw command for this object
        glDrawArrays(GL_TRIANGLES, 0, mesh.trimesh->m_vertexData.size() / 6);
        // Unbind Vertex Array
        glBindVertexArray(0);

    }

    this->doneCurrent();
}
