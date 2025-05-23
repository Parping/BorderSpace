#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class EnemyGameObject : public GameObject {

    public:

        EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle,double t_,int statue, int type);
        Circle* GetCircle()override;
        //getters
        bool GetAlive()override;
        bool GetColliable() override;
        int GetHP() override;
        int Get_Max_Hp() override { return max_hp; }
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
        void SetVelocity(const glm::vec3& velocity) override ;

        glm::vec3 EnemyGameObject::GetVelocity(void) override{ return velocity_; };

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

        void back() override;

        void setWant(bool s) override;
        bool getShoot() override;
        int Get_Shoot_Desire() { return shoot_desire; };

        void Run(double delta_time);

        bool getBack() override { return back_; }
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




    }; // class EnemyGameObject

} // namespace game

#endif // ENEMY_GAME_OBJECT_H_
