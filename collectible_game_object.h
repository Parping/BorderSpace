#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class CollectibleGameObject : public GameObject {

    public:
        CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle,int type);
        Circle* GetCircle()override;
        //getters

        bool GetCollectible() override;

        bool GetColliable() override;
        //setters
        void SetCollectible(bool a)override;



        //when collision,the collection should disappear
        void Get_Collision(double delta_time)override;

        // Update function for moving the player object around
        // Update status
        void Update(double delta_time) override ;
    private:
        //it is collectible, when it get collision, it is not collectible anymore.
        bool collectible;
        Circle circle;
        //if true, it can be calculate in the colliision function
        bool colliable;
        


    }; // class CollectibleGameObject

} // namespace game

#endif // COLLECTIBLE_GAME_OBJECT_H_
