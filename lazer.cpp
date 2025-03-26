#include "player_game_object.h"
#include "lazer.h"
#include <iostream>

namespace game {
    Lazer::Lazer(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent)
        : GameObject(position, geom, shader, texture) {
        parent_ = parent;
        scale_ = glm::vec2(5, 1);
        from_id_ = parent_->GetType();
        type = 52;
    }
    void Lazer::Update(double delta_time) {

        if (parent_->Get_Lazer_On()) {
            ghost = false;
        }
        else {
            ghost = true;
        }
        GameObject::Update(delta_time);
    }
    bool Lazer::Ract_Circle_Collition(glm::vec3 position, float r, double deltatime) {
        if (!parent_->Get_Lazer_On()) { return false; }
        if (ghost) { return false; }

        float Up, Down, Left, Right;
        float w = 2.5f;  
        float h = 0.5f;  
        float px, py;
        px = parent_->GetPosition().x;
        py = parent_->GetPosition().y;
        glm::vec2 localPoints[4] = {
            {-w, -h}, {w, -h},  
            {w, h}, {-w, h}    
        };
        glm::vec2 worldPoints[4];
        float ang_ = parent_->GetRotation();
        float rotatedJX;
        float rotatedJY;
        for (int i = 0; i < 4; i++) {
            float x = localPoints[i].x;
            float y = localPoints[i].y;

            float rotatedX = x * cos(ang_) - y * sin(ang_);
            float rotatedY = x * sin(ang_) + y * cos(ang_);
            rotatedJX = position_.x * cos(ang_) - position_.y * sin(ang_);
            rotatedJY = position_.x * sin(ang_) + position_.y * cos(ang_);

            worldPoints[i].x = rotatedX + px + rotatedJX;//
            worldPoints[i].y = rotatedY + py + rotatedJY;// 
        }

        glm::vec2 p1, p2, p3, p4;
        p1 = worldPoints[0];
        p2 = worldPoints[1];
        p3 = worldPoints[2];
        p4 = worldPoints[3];
        glm::vec2 Ract_Min, Ract_Max, Closest, Circle_Center, Ract_Center;
        Ract_Center = glm::vec2(rotatedJX + px, rotatedJY + py);
        Circle_Center = glm::vec2(position.x, position.y);

        glm::vec2 xAxis = glm::normalize(p2 - p1);  // x ax
        glm::vec2 yAxis = glm::normalize(p4 - p1);  // y ax
        //ract half
        float halfWidth = glm::distance(p1, p2) / 2.0f;
        float halfHeight = glm::distance(p1, p4) / 2.0f;
        // 
        glm::vec2 relativeCircleCenter = Circle_Center - Ract_Center;
        float localX = glm::dot(relativeCircleCenter, xAxis);
        float localY = glm::dot(relativeCircleCenter, yAxis);

        float clampedX = glm::clamp(localX, -halfWidth, halfWidth);
        float clampedY = glm::clamp(localY, -halfHeight, halfHeight);
        glm::vec2 closestPoint = Ract_Center + clampedX * xAxis + clampedY * yAxis;
        float distance = glm::length(Circle_Center - closestPoint);

        /*
                std::cout << "Ract_Center.x: " << Ract_Center.x << " Ract_Center.y: " << Ract_Center.y << std::endl;
        std::cout << "Center.x: " << Circle_Center.x << " Center.y: " << Circle_Center.y << std::endl;
        Ract_Min = worldPoints[2];
        Ract_Max = worldPoints[0];
        Closest= glm::clamp(Circle_Center, Ract_Max,Ract_Min );
        float distance = glm::length(Closest-Circle_Center);
        */
        if (distance <= r) {
           // std::cout << "succ!" << std::endl;
            return true;
        }
        else {
            //std::cout << "fail!" << std::endl;
                return false;
        }

    }
    void Lazer::Render(glm::mat4 view_matrix, double current_time) {
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