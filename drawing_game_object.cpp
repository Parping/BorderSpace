#include <glm/gtc/matrix_transform.hpp>

#include "drawing_game_object.h"

namespace game {

DrawingGameObject::DrawingGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture) : GameObject(position, geom, shader, texture) { 
    shape_ = 1;
    hover_ = false;
    click_ = false;
}


void DrawingGameObject::Render(glm::mat4 view_matrix, double current_time) {

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
    glm::mat4 T_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(toOrigin_.x, toOrigin_.y, 0.0));

    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix * T_matrix;
    // Setup the transformation matrix for the shader
   // glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;


    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);
    if (hover_) {
        shader_->SetUniform4f("rgb_color", glm::vec4(1.0f, 0.6f, 1.0f,1.0f));
    }
    else {
        shader_->SetUniform4f("rgb_color", rgba_);
    }

    shader_->SetUniform1i("shape_", shape_);
    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
