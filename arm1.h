#ifndef ARM1_H_
#define ARM1_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class Arm1 : public GameObject {

    public:

        Arm1(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent_);

        Circle* GetCircle()override { return parent_->GetCircle(); };
        //getters
        bool GetAlive()override { return parent_->GetAlive(); };
        bool GetColliable() override { return parent_->GetColliable(); };
        int GetHP() override { return parent_->GetHP(); };
        int GetState()override { return parent_->GetState(); }
        float GetSpeed()override { return parent_->GetSpeed(); };


        void Update(double delta_time) override;

        bool Ract_Circle_Collition(glm::vec3 position, float r, double deltatime) override;

        void Render(glm::mat4 view_matrix, double current_time);
        int getFrom() override { return from_id_; };

        void setTarget(glm::vec3 p) override { target_ = p; };

        void MovingTo(double delta_time) override;
        glm::mat4 GetTransformation() override;

        glm::mat4 GetLocalTransformation();
        void SetTOO(glm::vec2 to) override ;
        void SetMyTarget(glm::vec3 t)override;
    private:

        GameObject* parent_;
        int from_id_;
        Ract ract_;
        glm::vec3 target_;
        float speed;
        glm::vec2 toO_;




    }; // class lazer

} // namespace game

#endif // LAZER_H_
