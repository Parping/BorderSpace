#ifndef LAZER_H_
#define LAZER_H_

#include "game_object.h"


namespace game {

    // Inherits from GameObject
    class Lazer : public GameObject {

    public:

        Lazer(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent_);


        //getters
        glm::vec3 GetPosition()override;
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

        Ract* GetRact()override { return &ract_; };

        void Update(double delta_time) override;

        bool Ract_Circle_Collition(glm::vec3 position, float r, double deltatime) override;

        void Render(glm::mat4 view_matrix, double current_time);
        int getFrom() override { return from_id_; };

        glm::mat4 GetTransformation() override;

        glm::mat4 GetLocalTransformation();

    private:

        GameObject* parent_;
        int from_id_;
        Ract ract_;
        bool alive_;
        glm::vec2 toO_;




    }; // class lazer

} // namespace game

#endif // LAZER_H_
