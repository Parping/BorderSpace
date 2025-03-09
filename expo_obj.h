#ifndef EXPO_OBJ_H_
#define EXPO_OBJ_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class Expo_obj : public GameObject{

    public:
        Expo_obj(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);







        // Update function for moving the player object around
        // Update status
        bool GetAlive()override { return alive; }
        void Update(double delta_time) override;
    private:

        bool colliable;
        Timer time;
        bool alive;


    }; // class Expo_obj

} // namespace game

#endif // EXPO_OBJ_H_
