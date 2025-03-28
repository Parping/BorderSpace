#ifndef FORTRESS_SHOOTER_H_
#define FORTRESS_SHOOTER_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class FortressShooter : public GameObject {

    public:

        FortressShooter(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent_);
        glm::vec3 GetPosition()override {
            glm::vec4 tran_pos = GetLocalTransformation() * glm::vec4(position_, 1.0f);
            glm::vec3 actual_pos_ = glm::vec3(tran_pos.x, tran_pos.y, -1.0f);
            return actual_pos_;
        }
        float GetRotation()override;
        Circle* GetCircle()override { return parent_->GetCircle(); };
        //getters
        void SetAlive(bool a)override { alive_ = a; }
        bool GetAlive()override {

            return alive_;
        };
        bool GetColliable() override {
            if (alive_) { return parent_->GetColliable(); }
            return false;
        };
        int GetHP() override { return parent_->GetHP(); };
        int GetState()override { return parent_->GetState(); }
        float GetSpeed()override { return parent_->GetSpeed(); };

        void Update(double delta_time) override ;

        void Render(glm::mat4 view_matrix, double current_time);
        glm::mat4 GetTransformation() override;

        glm::mat4 GetLocalTransformation();
    private:

        GameObject* parent_;
        bool alive_;
        



    }; // class FortressShooter

} // namespace game

#endif // FORTRESS_SHOOTER_H_
