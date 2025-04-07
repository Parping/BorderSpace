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

#include "shop_obj.h"

#include "maze.h"

#include "wall.h"

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

            //hp,energy,exp,level
            //money.score, max_hp,max_energy,max_exp
            //buff, collect
            struct HUDValue {
                int hp;
                int energy;
                int exp;
                int level;
                int money;
                int score;
                int max_hp;
                int max_energy;
                int max_exp;
                int ip;
                int iron;

                int bomb;
                
                bool buff;
                bool collect;

            };
            struct EnemyDrop {
                int ip_;
                int energy_;
                int iron_;
                int exp_;
                int coin_;
                float percent_;
            };
            EnemyDrop Minion = { 2,5,1,10,0,50 };
            EnemyDrop EX_Minion = { 5,5,3,20,0,70 };
            EnemyDrop BBB = { 5, 20, 20, 50,10,0 };
            EnemyDrop Fortress = { 20,5,5,200,10,0 };
            EnemyDrop Monster = { 5, 20, 20, 50,10,0 };


            glm::vec3 generateRandomPosition();

            //
            void generateDifferentEnemy();

            //when enemy dies, items drop at the position, different type drop different random items.
            //this function create new items and push them into the game_object_list
            void generateEnemyDrops(glm::vec3 position,int type);

            void generateEnemyDrops(glm::vec3 position,EnemyDrop s);
            float  randRange(float min, float max);
            void RenderMiniMap();

            void wakeup_monster();
            void destory_level_1();
            void set_up_level_2();

            bool check_level_2();

            void set_up_maze();
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
            Shader animate_shader_;
            // Shader for rendering text
            Shader text_shader_;
            Shader drawing_shader_;

            // References to textures
            // This needs to be a pointer
            GLuint *tex_;


            //add timer and audio
            Timer timer;
            Timer shooter_timer;
            Timer level_1_timer;
            Timer changing_timer;
            audio_manager::AudioManager am;
            //the music index, used fo audio manager
            int background_music_index, explosion_music_index;

            bool stop_;
            bool map_;

            // List of game objects
            std::vector<GameObject*> game_objects_;
            std::vector<GameObject*> hud_objects_;
            std::vector<GameObject*> enemy_objects_;

            std::vector<GameObject*> wall_objects_;

            GameObject* fortress_;
            bool fortress_exist_;

            GameObject* the_player_;

            Maze maze_;
            
            Shop shop_;
            float X_Ax_;
            float Y_Ax_;
            int level_;

            bool changing_level_;

            GameObject* minimap_;
            float map_zoom_;
            //hp,energy,exp,level
            //money.score, max_hp,max_energy,max_exp
            //buff, collect
            HUDValue HUDValue_;

            void setStop(bool b) { stop_ = b; }
            bool getStop() { return stop_; }

            void setMap(bool b);
            bool getMap() { return map_; }

            void BossRoom();

            void Setup_HUD_Value();
            void Update_HUD_Value();

            void Update_HUD(double delta_time);

            void Setup_Mini_Map(GameObject* m);
            void Setup_HUD_Bar(GameObject* h);

            void Generate_Fortress(GameObject* f);

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
