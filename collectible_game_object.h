#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class CollectibleGameObject : public GameObject {

    public:
        CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle);
        Circle* GetCircle()override;
        //getters

        bool GetCollectible() override;

        bool GetColliable() override;
        //setters
        void SetCollectible(bool a)override;




        void Get_Collision()override;//when collision, disappear

        // Update function for moving the player object around
        // Update status
        void Update(double delta_time) override ;
    private:
        bool collectible;//it is collectible, when it get collision, it is not collectible anymore.
        Circle circle;
        bool colliable;


    }; // class CollectibleGameObject

} // namespace game

#endif // COLLECTIBLE_GAME_OBJECT_H_
