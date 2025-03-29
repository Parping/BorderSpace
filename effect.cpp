#include "player_game_object.h"
#include "effect.h"
#include <iostream>

namespace game {
    Effect::Effect(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent,int type_ef)
        : GameObject(position, geom, shader, texture) {
        parent_ = parent;
        type_effect_ = type_ef;
        scale_=glm::vec2(parent_->GetScale().x * 1.3, parent_->GetScale().y * 1.3);

    }
    void Effect::Update(double delta_time) {
        if (!alive_) { return; }
        switch (type_effect_)
        {
        case 31:
            if (parent_->Get_Shield_On()) {
                ghost = false;
            }
            else {
                ghost = true;
            }
        default:
            break;
        }

        GameObject::Update(delta_time);
    }

    void Effect::Render(glm::mat4 view_matrix, double current_time) {
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
}