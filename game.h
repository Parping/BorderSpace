#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.h"
#include "game_object.h"
#include "HUD.h"
#include "audio_manager.h"

namespace game {

    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game world (scene, game objects, etc.)
            void SetupGameWorld(void);

            // Destroy the game world
            void DestroyGameWorld(void);

            // Run the game (keep the game active)
            void MainLoop(void); 
            //random position generator
            glm::vec3 generateRandomPosition();
        private:
            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Sprite geometry
            Geometry *sprite_;
            Geometry *bg;
            // Shader for rendering sprites in the scene
            Shader sprite_shader_;
            Shader number_shader_;
            Shader bar_shader_;

            // References to textures
            // This needs to be a pointer
            GLuint *tex_;

            HUD *hud_;
            //add timer and audio
            Timer timer;
            Timer shooter_timer;
            audio_manager::AudioManager am;
            //the music index, used fo audio manager
            int background_music_index, explosion_music_index;

            // List of game objects
            std::vector<GameObject*> game_objects_;


            // Keep track of time
            double current_time_;

            // Handle user input
            void HandleControls(double delta_time);

            // Update all the game objects
            void Update(double delta_time);
 
            // Render the game world
            void Render(void);

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname, int type);

            // Load all textures
            void LoadTextures(std::vector<std::string> &textures);
    }; // class Game

} // namespace game

#endif // GAME_H_
