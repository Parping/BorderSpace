#ifndef FORTRESS_OBJECT_H_
#define FORTRESS_OBJECT_H_

#include "game_object.h"
#include "blue_game_object.h"
#include "fortress_shooter.h"

namespace game {

    // Inherits from GameObject
    class FortressObject : public GameObject {

    public:

        FortressObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle, double t_, int statue, int type);
       
        ~FortressObject() {
            child_1->N_Parent();
            child_2->N_Parent();
            arms_->N_Parent();
        }
        
        Circle* GetCircle()override;
        //getters
        bool GetAlive()override;
        bool GetColliable() override;
        int GetHP() override;
        int GetState()override;
        float GetSpeed()override;
        //setters
        void SetColliable(bool a)override;
        void SetAlive(bool a)override;
        void SetStatue(int a) override;
        void SetPlayer(GameObject* a)override;
        inline void SetCenter(glm::vec3& a)override;
        void SetWidth(float a)override;
        void SetHeight(float a)override;
        void SetVelocity(const glm::vec3& velocity) override;

        void SetShooter(GameObject* s)override { shooter = s; };

        glm::vec3 GetVelocity(void) override { return velocity_; };
        glm::vec4 getShooter()override ;

        void Get_Collision_Pro(double delta_time, int pro_type, int attacker)override;

        void Get_Collision(double delta_time)override;//when collision, hitpoint --, if hitpoint to 0, explosion
        void Explosion() override;//change texture and count 5 s

        void InitType(int t);
        void Init(int statue);//init randomly in different statue here.

        // Update function for moving the player object around
        // Update status
        void Update(double delta_time) override;

        void patrolling();//one mode for moving
        void intercepting(double delta_time);//another mode for moving
        bool findPlayer();//see if player is closer to this obj



        void setWant(bool s) override;
        bool getShoot() override;
        int Get_Shoot_Desire() { return shoot_desire; };



        bool getBack() override { return back_; }

        void become_angry()override;
        void heal()override;

        void SetArm(GameObject* a) override { arms_ = a; };
        glm::mat4 GetTransformation() override;
        glm::mat4 GetLocalTransformation();

        void SetChild1(GameObject* a) override { child_1 = a; };
        void SetChild2(GameObject* a) override { child_2 = a; };

        
    private:
        int hitpoint;
        int statue;//0 normal, 1 invincible, 2 patrolling, 3 intercepting
        bool alive;
        bool colliable;
        Circle circle;
        GameObject* player_;//player pointer
        glm::vec3 center_;//center for the obj patrolling
        float width;//ellipse width for patrolling
        float height;//ellipse  height for patrolling
        double t_;//time passed
        float speed;//unit speed for moving
        Timer timer_react;//timer to simulate their slow reaction tim
        float react;
        Timer collision_timer;
        bool want_shoot;
        int shoot_desire;
        Timer reload_timer;
        float reload;
        glm::vec3 target;

        int max_hp;
        bool back_;
        Timer get_angry;
        bool isAngry;
        GameObject* shooter;
        GameObject* arms_;
        GameObject* child_1;
        GameObject* child_2;





    }; // class Fortress_object

} // namespace game

#endif // FORTRESS_OBJECT_H_
