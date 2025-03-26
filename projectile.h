#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject

    class Projectile : public GameObject {

    public:
        Projectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle,float angle,int from,int ty);
        Circle* GetCircle()override;
        //getters



        void Init();//init the velocity

        void Get_Collision(double delta_time)override;//when collision, disappear
        void Get_Collision_Pro(double delta_time, int pro_type, int attacker)override;
        bool GetAlive()override;
        bool GetColliable();
        glm::vec3 Projectile::GetVelocity(void) override;
        // Update function for moving the player object around
        // Update status
        void Update(double delta_time) override;
        //check collision by ray to circle
        bool RayToCircleCheck(glm::vec3 position, float r, double deltatime) override;

        int getFrom() override { return from_id_; };
    private:
        int hitpoint;
        bool colliable;
        Circle circle;
        Timer exist_timer;//if it needs to die
        bool alive;
        float speed;
        double t_;//time passed
        glm::vec3 startPosition;//position that it start
        int from_id_;
        Timer collision_timer;



    }; // class CollectibleGameObject

} // namespace game

#endif // PROJECTILE_H_
