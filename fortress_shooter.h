#ifndef FORTRESS_SHOOTER_H_
#define FORTRESS_SHOOTER_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class FortressShooter : public GameObject {

    public:

        FortressShooter(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent_);

        Circle* GetCircle()override { return parent_->GetCircle(); };
        //getters
        bool GetAlive()override { return parent_->GetAlive(); };
        bool GetColliable() override { return parent_->GetCollectible(); };
        int GetHP() override { return parent_->GetHP(); };
        int GetState()override { return parent_->GetState(); }
        float GetSpeed()override { return parent_->GetSpeed(); };

        void Update(double delta_time) override ;

        void Render(glm::mat4 view_matrix, double current_time);

    private:

        GameObject* parent_;





    }; // class FortressShooter

} // namespace game

#endif // FORTRESS_SHOOTER_H_
