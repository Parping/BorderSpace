#include "arm1.h"
#include "player_game_object.h"

#include <iostream>

namespace game {
    Arm1::Arm1(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent)
        : GameObject(position, geom, shader, texture) {
        parent_ = parent;
        scale_ = glm::vec2(1, 1);
        from_id_ = parent_->GetType();
        type = 80;
        ract_ = Ract(1, 1);
        target_ = glm::vec3(0, 0, 0);
        speed = 5;
        angle_ = 0;
        toO_ = glm::vec2(0.5,0);
        alive_ = true;
    }
    glm::vec3 Arm1::GetPosition() {
        glm::vec4 tran_pos = GetLocalTransformation() * glm::vec4(position_, 1.0f);
        glm::vec3 actual_pos_ = glm::vec3(tran_pos.x, tran_pos.y, -1.0f);
        return actual_pos_;
    }
    void Arm1::MovingTo(double delta_time) {

        glm::vec2 O, E, T,E_P;
        float target_angle, actual_angle;
        ract_.SetParent_Transformation_matrix(GetLocalTransformation());
        E_P = glm::vec2(parent_->GetPosition().x, parent_->GetPosition().y);//手臂原点位置,上一个终点位置
        O = ract_.get_point(4);
        E = ract_.get_point(5);    // 机械臂的顶点位置
        T = glm::vec2(target_.x, target_.y);  // 目标点位置


        glm::vec2 T_parent = T - glm::normalize(T - E_P)*scale_.x;
        parent_->setTarget(glm::vec3(T_parent,0));

        // 计算方向向量
        glm::vec2 arm_now = glm::normalize(E - O);
        glm::vec2 arm_expect = glm::normalize(T - O);

        float angle_now = glm::atan(arm_now.y, arm_now.x);
        float angle_expect = glm::atan(arm_expect.y, arm_expect.x);
        float angle_increment = (glm::pi<float>() / 1800.0f) * speed;//calculate the increment by delta_time

        float diff_angle;

        if (abs(angle_expect - angle_now) < 0.01f) {
           // SetRotation(angle_expect);
        }
        else if (angle_expect > angle_now) {
            SetRotation(angle_ + angle_increment);
        }
        else if(angle_expect<angle_now) {
            SetRotation(angle_ - angle_increment);

        }
      //  std::cout << "O: " << O.x << ", " << O.y << std::endl;
     //   std::cout << "E: " << E.x << ", " << E.y << std::endl;
       // std::cout << "T: " << T.x << ", " << T.y << std::endl;


    }

    void Arm1::Get_Collision(double delta_time) {
        parent_->become_angry();
    }

    void Arm1::Update(double delta_time) {
        if (!alive_) { return; }

        MovingTo(delta_time);
        GameObject::Update(delta_time);
    }
    bool Arm1::Ract_Circle_Collition(glm::vec3 position, float r, double deltatime) {


        glm::vec2 p1, p2, p3, p4;
        glm::vec2 Circle_Center, Ract_Center;
        ract_.SetParent_Transformation_matrix(GetLocalTransformation());
        p1 = ract_.get_point(0);
        p2 = ract_.get_point(1);
        p3 = ract_.get_point(2);
        p4 = ract_.get_point(3);

        Circle_Center = glm::vec2(position.x, position.y);
        Ract_Center = ract_.get_My_Center();

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




        // std::cout << "Ract_Center.x: " << Ract_Center.x << " Ract_Center.y: " << Ract_Center.y << std::endl;
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
    void Arm1::Render(glm::mat4 view_matrix, double current_time) {
        // Set up the shader
        if (!alive_) { return; }
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
      //  glm::mat4 parent_rotation_matrix = glm::rotate(glm::mat4(1.0f), parent_->GetRotation(), glm::vec3(0.0, 0.0, 1.0));
     //   glm::mat4 parent_translation_matrix = glm::translate(glm::mat4(1.0f), parent_->GetPosition());
        glm::mat4 parent_transformation_matrix =parent_->GetTransformation();
        glm::mat4 T_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(toO_.x, toO_.y, 0.0));
        // Setup the transformation matrix for the shader
        glm::mat4 transformation_matrix = GetLocalTransformation();

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
    glm::mat4 Arm1::GetLocalTransformation() {
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));
        glm::mat4 T_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(toO_.x, toO_.y, 0.0));
        glm::mat4 transformation_matrix = GetTransformation() * scaling_matrix * T_matrix;
        return transformation_matrix;
    }
    glm::mat4 Arm1::GetTransformation() {
        if (!alive_) { return glm::mat4(1.0f); }

        // Setup the rotation matrix for the shader
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

        // Set up the translation matrix for the shader
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);


        // Set up the parent transformation matrix
        glm::mat4 parent_transformation_matrix = parent_->GetTransformation();

        // Setup the transformation matrix for the shader
        glm::mat4 transformation_matrix = parent_transformation_matrix * translation_matrix * rotation_matrix ;
        return transformation_matrix;
    }
    void Arm1::SetMyTarget(glm::vec3 t) { target_ = t; }
    void Arm1::SetTOO(glm::vec2 to) {
        toO_ = to;
    }
}