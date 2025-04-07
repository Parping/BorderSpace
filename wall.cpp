
#include "wall.h"

#include <iostream>

namespace game {

	Wall::Wall(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) 
		: GameObject(position, geom, shader, texture)
        {//,collider_(this)
		type = 41;
		alive_ = true;
		colliable_ = true;
		ract_ = Ract();
        hp_ = 1;

	}
	void Wall::SetScale(glm::vec2 scale){
		scale_ = scale;
		float x = scale.x;
		float y = scale.y;
		ract_.SetWH(glm::vec2(x, y));
       // collider_.SetWidth(x);
       // collider_.SetHeight(y);
	}
	void Wall::Update(double delta_time) {
	
	}

    void Wall::Get_Collision_Pro(double delta_time, int pro_type, int attacker) {
        if (pro_type!=53) { return; }

        hp_ = 0;
        alive_ = false;

    }

    bool Wall::Ract_Circle_Collition(glm::vec3 position, float r, double deltatime) {

        if (ghost) { return false; }

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
          //   ract_.Print();
          //   std::cout << "Target: " << position.x << " " << position.y << std::endl;
            return true;
        }
        else {
            //std::cout << "fail!" << std::endl;
            return false;
        }

    }

    glm::mat4 Wall::GetLocalTransformation() {
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));//change to .x and .y

        // Setup the rotation matrix for the shader
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

        // Set up the translation matrix for the shader
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

        // Setup the transformation matrix for the shader
        glm::mat4 T_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(toOrigin_.x, toOrigin_.y, 0.0));

        glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix * T_matrix;
        return transformation_matrix;
    }
}