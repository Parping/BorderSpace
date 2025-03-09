#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture,int hp,Circle circle);
            Circle* GetCircle()override;
            //getters
             bool GetAlive()override ;
             bool GetColliable() override ;
             int GetHP() override ;
             int GetItem() override;
             //setters
             void SetColliable(bool a)override;
             void SetAlive(bool a)override;
             int GetState()override;
             float GetSpeed()override;
            
             void Get_Collision(double delta_time)override;//when collision, hitpoint --, if hitpoint to 0, explosion
             void Explosion() override;//change texture and count 5 s
             void CollectItem() override;
             void Invincible()override;
             void AddVelocity(glm::vec3 a)override;//add new vec3 to the current velocity


            // Update function for moving the player object around
            // Update status
            void Update(double delta_time) override;
            void SetVelocity(const glm::vec3& velocity)override;
    private:
        int hitpoint;
        int item;//internal item counter
        int state;//0 normal, 1 invincible
        bool alive;
        bool colliable;
        Circle circle;
        Timer timer_invi;//invincible timer
        float speed;//speed for moving
        double t_;//time passed
        float max_velocity;//max velocity it can reach



    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
