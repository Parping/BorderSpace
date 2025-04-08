#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include <vector>

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
             PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture,int hp,Circle circle);
             
             ~PlayerGameObject();
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

             void Get_Collision_Pro(double delta_time, int pro_type, int attacker)override;

             void Explosion() override;//change texture and count 5 s
             void CollectItem(int type) override;
             void Invincible()override;
             void AddVelocity(glm::vec3 a)override;//add new vec3 to the current velocity


            // Update function for moving the player object around
            // Update status
            void Update(double delta_time) override;
            void SetVelocity(const glm::vec3& velocity)override;

            int Get_INPoint() override { return invisible_point_; }
            int Get_Energy() override { return energy_; }
            int Get_Iron() override { return iron_; }
            int Get_Coin() override { return coin_; }
            int Get_Exper()override { return experience_; }
            int Get_Level() override { return level_; }

            void Set_INPoint(int i) override { invisible_point_ = i; }
            void Set_Energy(int e)override { energy_ = e; }
            void Set_Iron(int i) override { iron_ = i; }
            void Set_Coin(int c) override { coin_ = c; }
            void Set_Exper(int e) override { experience_ = e; }
            void Set_Level(int l)override { level_ = l; }
            
            void Add_Exp(int a) override;
            void Level_up() override;

            int Get_Max_Exp() override { return max_exp; }
            int Get_Max_Hp() override { return max_hp; }
            int Get_Max_Energy() override { return max_energy; }

            bool Get_Lazer_On() override { return lazer_act_; }
            bool Get_Shield_On() override { return shield_act_; }
            void Set_Lazer_On(bool a) override;
            void Set_Shield_On(bool a) override;

            void Set_Rest(bool a)override { rest_ = a; }
            glm::mat4 GetTransformation() override;
            glm::mat4 GetLocalTransformation();
            void getHeal() override;
            void AddChild(GameObject* child) override;

            void AddBomb() override { bomb_++; };
            void deleteBomb()override { if (bomb_ > 0) { bomb_--; } };
            void SetCan_Lazer(bool a) override { can_lazer_ = a; }
            void SetCan_Shield(bool a) override { can_shield_ = a; }

            int GetBomb() override { return bomb_; }
            bool Get_Can_lazer() override { return can_lazer_; }
            bool Get_Can_shield() override { return can_shield_; }

            void CollideWall(double delta_time, glm::vec3 new_velocity);
         //   CircleCollider* GetCircleCollider() override { return &collider_; }
          //  CircleCollider collider_;
            bool GetAcc() override { return accelerate; }
            bool GetEngine() override { return engine_active; }
            void SetAcc(bool a) override;
            void SetEng(bool a) override { engine_active = a; }

    private:
        int hitpoint;
        int item;//internal item counter
        int state;//0 normal, 1 invincible
        bool alive;
        bool colliable;
        bool accelerate;
        bool engine_active;
        Circle circle;
        Timer timer_invi;//invincible timer
        float speed;//speed for moving
        double t_;//time passed
        float max_velocity;//max velocity it can reach

        int invisible_point_;
        int energy_;
        int iron_;
        int coin_;
        int experience_;
        int level_;
        int max_exp;
        int max_hp;
        int max_energy;

        int bomb_;
        bool can_lazer_;
        bool can_shield_;

        Timer collision_timer;
        Timer rest_timer;
        Timer act_timer;

        bool lazer_act_;
        bool shield_act_;

        bool rest_;
        std::vector<GameObject*> chile_game_objects_;//内部的所有小东西



    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
