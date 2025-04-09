#include "Collider.h"
#include "player_game_object.h"
#include"wall.h"
#include <iostream>
namespace game {
    CircleCollider::CircleCollider(float r, GameObject* g) :Collider(g) {
        radius_ = r;

    }

    bool CircleCollider::TestCollision(Collider& other) {
        glm::vec3 center1 = this->GetObject()->GetPosition();
        glm::vec3 center2 = glm::vec3(other.GetObject()->GetPosition().x, other.GetObject()->GetPosition().y, other.GetObject()->GetPosition().z);
        float radius1 = radius_;
        glm::vec2 p1, p2, p3, p4;
        glm::vec2 Circle_Center, Ract_Center;
        
        float width = other.GetWidth();
        float height = other.GetHeight();

        p1 = glm::vec2(center2.x - width / 2, center2.y - height / 2);
        p2 = glm::vec2(center2.x + width / 2, center2.y - height / 2);
        p3 = glm::vec2(center2.x + width / 2, center2.y + height / 2);
        p4 = glm::vec2(center2.x - width / 2, center2.y + height / 2);
        
        Circle_Center = glm::vec2(center1.x, center1.y);
        Ract_Center = glm::vec2(center2.x, center2.y);

        glm::vec2 xAxis = glm::normalize(p2 - p1);  // x ax
        glm::vec2 yAxis = glm::normalize(p4 - p1);  // y axss
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


        if (distance <= radius1) {
            // //std::cout << "succ!" << std::endl;
          //   ract_.Print();
          //   //std::cout << "Target: " << position.x << " " << position.y << std::endl;
         //   //std::cout << "P1£º " << p1.x << " " << p1.y << std::endl;
          //  //std::cout << "P2£º " <<p2.x << " " << p2.y << std::endl;
          //  //std::cout << "P3£º " << p3.x << " " << p3.y << std::endl;
           // //std::cout << "P4£º " << p4.x << " " << p4.y << std::endl;
            return true;
        }
        else {
            ////std::cout << "fail!" << std::endl;
            return false;
        }
    }


    RactCollider::RactCollider(float w, float h, GameObject* g) :Collider(g) {
        width = w;
        height = h;
    }
    RactCollider::RactCollider(GameObject* g) :Collider(g) {}

    bool RactCollider::TestCollision(Collider& other) {
        glm::vec3 center1 = other.GetObject()->GetPosition();
        glm::vec3 center2 = this->GetObject()->GetPosition();
        float radius1 = other.GetRadius();
        glm::vec2 p1, p2, p3, p4;
        glm::vec2 Circle_Center, Ract_Center;
        p1 = glm::vec2(center2.x + other.GetWidth() / 2, center2.y + other.GetHeight() / 2);
        p2 = glm::vec2(center2.x - other.GetWidth() / 2, center2.y + other.GetHeight() / 2);
        p3 = glm::vec2(center2.x + other.GetWidth() / 2, center2.y - other.GetHeight() / 2);
        p4 = glm::vec2(center2.x - other.GetWidth() / 2, center2.y - other.GetHeight() / 2);

        Circle_Center = glm::vec2(center1.x, center1.y);
        Ract_Center = glm::vec2(center2.x, center2.y);

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


        if (distance <= radius1) {
            // //std::cout << "succ!" << std::endl;
          //   ract_.Print();
          //   //std::cout << "Target: " << position.x << " " << position.y << std::endl;
            return true;
        }
        else {
            ////std::cout << "fail!" << std::endl;
            return false;
        }
    }
}