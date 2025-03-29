#include "player_game_object.h"
#include "fortress_shooter.h"
#include <iostream>

namespace game {
    FortressShooter::FortressShooter(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture,GameObject* parent)
        : GameObject(position, geom, shader, texture) {
        parent_ = parent;

    }
    void FortressShooter::Update(double delta_time) {
        GameObject::Update(delta_time);
    }

    void FortressShooter::Render(glm::mat4 view_matrix, double current_time) {
        if (!alive_) { return; }
        // Set up the shader
        shader_->Enable();
        if (ghost) {
            shader_->SetUniform1i("ghost", 1);//if so, send 1 to ghost
        }
        else {
            shader_->SetUniform1i("ghost", 0);//if not, send 0 to ghost
        }
        shader_->SetUniform2f("frame", num_frame);
        shader_->SetUniform1i("current_frame", current_frame);
        shader_->SetUniform1f("percent", bar_percent);
        shader_->SetUniform1i("offset", offset);
        // Set up the view matrix
        shader_->SetUniformMat4("view_matrix", view_matrix);

        // Setup the scaling matrix for the shader
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

        // Setup the rotation matrix for the shader
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

        // Set up the translation matrix for the shader
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

        // Set up the parent transformation matrix
        glm::mat4 parent_rotation_matrix = glm::rotate(glm::mat4(1.0f), parent_->GetRotation(), glm::vec3(0.0, 0.0, 1.0));
        glm::mat4 parent_translation_matrix = glm::translate(glm::mat4(1.0f), parent_->GetPosition());
        glm::mat4 parent_transformation_matrix = parent_translation_matrix * parent_rotation_matrix;

        // Setup the transformation matrix for the shader
        glm::mat4 transformation_matrix = parent_transformation_matrix * translation_matrix * rotation_matrix * scaling_matrix;

        // Set the transformation matrix in the shader
        shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

        // Set the time in the shader
        shader_->SetUniform1f("time", current_time);

        // Set up the geometry
        geometry_->SetGeometry(shader_->GetShaderProgram());

        // Bind the particle texture
        glBindTexture(GL_TEXTURE_2D, texture_);

        // Draw the entity
        glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
    }

    float FortressShooter::GetRotation() {
        return angle_;
    }
    glm::mat4 FortressShooter::GetLocalTransformation() {
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));
        glm::mat4 T_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0, 0.0));
        glm::mat4 transformation_matrix = GetTransformation() * scaling_matrix * T_matrix;
        return transformation_matrix;
    }
    glm::mat4 FortressShooter::GetTransformation() {
        if (!alive_) { return glm::mat4(1.0f); }

        // Setup the rotation matrix for the shader
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

        // Set up the translation matrix for the shader
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);


        // Set up the parent transformation matrix
        glm::mat4 parent_transformation_matrix = parent_->GetTransformation();

        // Setup the transformation matrix for the shader
        glm::mat4 transformation_matrix = parent_transformation_matrix * translation_matrix * rotation_matrix;
        return transformation_matrix;
    }

}