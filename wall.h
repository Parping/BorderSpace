#ifndef WALL_H_
#define WALL_H_

#include "game_object.h"


namespace game {

    // Inherits from GameObject
    class Wall : public GameObject {

    public:

        Wall(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);


        //getters

        void SetAlive(bool a)override { alive_ = a; }
        bool GetAlive()override {

            return alive_;
        };
        bool GetColliable() override {     
            return colliable_;
        };
        int GetHP() override { return hp_; };

        void SetScale(glm::vec2 scale) override;


        Ract* GetRact()override { return &ract_; };

        void Wall::Get_Collision_Pro(double delta_time, int pro_type, int attacker);

        void Update(double delta_time) override;

        bool Ract_Circle_Collition(glm::vec3 position, float r, double deltatime) override;

        glm::mat4 GetLocalTransformation();

    private:
        Ract ract_;
        bool alive_;
        bool colliable_;
        int hp_;




    }; // class lazer

} // namespace game

#endif // LAZER_H_
