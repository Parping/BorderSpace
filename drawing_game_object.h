#ifndef DRAWING_GAME_OBJECT_H_
#define DRAWING_GAME_OBJECT_H_

#include "game_object.h"
#include <vector>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
namespace game {

    // Inherits from GameObject
    class DrawingGameObject : public GameObject {

        public:
            DrawingGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Render function for the text


            void Render(glm::mat4 view_matrix, double current_time) override;

            void SetShape_(int s) override { shape_ = s; };

            
        private:
            int shape_;//1 circle 2 ract
            bool hover_;
            bool click_;

    }; // class DrawingGameObject

} // namespace game

#endif // DRAWING_GAME_OBJECT_H_
