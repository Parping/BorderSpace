#ifndef ARM1_H_
#define ARM1_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class Arm1 : public GameObject {

    public:

        Arm1(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent_);


        //getters
        glm::vec3 GetPosition()override;
        void SetAlive(bool a)override { alive_ = a; }
        bool GetAlive()override { 
            
            return alive_;
        };
        bool GetColliable() override { 
            if (alive_ ) { return parent_->GetColliable(); }
            return false;
        };


        void Get_Collision(double delta_time)override;
        void Update(double delta_time) override;
        void become_angry()override;
        bool Ract_Circle_Collition(glm::vec3 position, float r, double deltatime) override;

        void Render(glm::mat4 view_matrix, double current_time);
        int getFrom() override { return from_id_; };

        void setTarget(glm::vec3 p) override { target_ = p; };

        void MovingTo(double delta_time) override;
        glm::mat4 GetTransformation() override;

        glm::mat4 GetLocalTransformation();
        void SetTOO(glm::vec2 to) override ;
        void SetMyTarget(glm::vec3 t)override;

        GameObject* GetParent() override { return parent_; };
        void N_Parent() override { parent_ = nullptr; };
    private:

        GameObject* parent_;
        int from_id_;
        Ract ract_;
        glm::vec3 target_;
        float speed;
        glm::vec2 toO_;
        bool alive_;




    }; // class lazer

} // namespace game

#endif // LAZER_H_
