#ifndef EFFECT_H_
#define EFFECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class Effect : public GameObject {

    public:
        //31 shield
        Effect(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent_,int type_effect);

        Circle* GetCircle()override { return parent_->GetCircle(); };
        //getters
        bool GetAlive()override { return parent_->GetAlive(); };
        bool GetColliable() override { return false; };
        int GetHP() override { return parent_->GetHP(); };
        int GetState()override { return parent_->GetState(); }
        float GetSpeed()override { return parent_->GetSpeed(); };

        void Update(double delta_time) override;

        void Render(glm::mat4 view_matrix, double current_time);

    private:

        GameObject* parent_;
        int type_effect_;




    }; // class lazer

} // namespace game

#endif // LAZER_H_
