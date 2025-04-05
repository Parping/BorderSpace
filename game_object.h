#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "geometry.h"
#include "timer.h"
#include "circle.h"
#include "ract.h"
#include <string>

#include <vector>

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)

        explosion -1
        normal 0
        player 1
        collectible: 
            11 - Invisible Point
            12 - Energy
            13 - Iron
            l4 - Coin
        enemy:
            91 - minion
            92 - ex-minion
            93 - BBB
            94 - Fortress //specific function
            95 - Monster
            100 - BOSS
        projictile : -> from player? or other ones
            51 - basic projectile
            52 - lazer
            53 - bomb
        
    */






    class GameObject {

        public:
            // Constructor
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            struct E
            {
                int hp_;
                int type_;
                float speed_;
                float size_scale_;
                float react_;
                int shoot_desire_;
                float reload_;
            };

            E bbb = {
                10,
                93,
                2.1f,
                0.8,
                0.5,
                0,
                0
            };
            E fortress = {
                10,
                94,
                0,
                5.0,
                5,
                80,
                1.0f
            };
            E minion = {
                1,
                91,
                1.5f,
                1,
                2.0f,
                20,
                1.0f
            };
            E ex_minion = {
                5,
                92,
                1.8f,
                1.0,
                0.5f,
                50,
                0.7
            };
            E monster = {
                10,
                95,
                0.5f,
                2.0,
                0.5,
                80,
                10
            };
            E boss = {
                200,
                100,
                1,
                15.0f,
                50,
                10,
                10



            };
            struct Pro {
                int hp_;
                int speed_;
                glm::vec2 size_;
                float life_span_;
                int pro_type_;
                int killer_point;
            };
            Pro pro_base{ 1,15,glm::vec2(0.5,0.2),5,51,1 };
            Pro pro_lazer{ -1,15,glm::vec2(1,1),-1,52,5 };
            Pro pro_bomb{ 5,3,glm::vec2(1,0.8f),20,53,5 };

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject 
            virtual void Render(glm::mat4 view_matrix, double current_time);

            // Getters
            virtual glm::vec3 GetPosition(void)  { return position_; }
            inline glm::vec2 GetScale(void) const { return scale_; }//change it to vec2
            virtual float GetRotation(void) { return angle_; }
            int GetType() { return type; }
            virtual glm::vec3 GetVelocity(void) { return glm::vec3(0, 0, 0); };

            virtual void back() {};
            virtual void SetStatue(int a) {};
            virtual bool getBack() { return false; }
            // Get bearing direction (direction in which the game object
            // is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

            //Get the texture of the obj
            GLuint getTexture();

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(glm::vec2 scale) { scale_=scale; }//change it to vec2
            void SetRotation(float angle);
            virtual void SetVelocity(const glm::vec3& velocity) {};

            //change the texture
            void SetTexture(GLuint new_texture);
            void SetGhost(bool a);

            //virtual function return fixed value
            virtual Circle* GetCircle() { return &Circle(0.0f, 0.0f, 0.0f); };
            virtual bool GetAlive() { return false; };
            virtual bool GetColliable() { return false; };
            virtual int GetHP() { return 0; };
            virtual bool GetCollectible() { return false; };
            virtual int GetItem() { return 0; };
            virtual int GetState() { return -1; };
            virtual float GetSpeed() { return 0; };
            virtual int getFrom() { return -2; }

            virtual bool RayToCircleCheck(glm::vec3 position, float r,double deltatime) { return false; };
            virtual bool Ract_Circle_Collition(glm::vec3 position, float r, double deltatime) { return false; }
            
            //vitrual function, won't implimentation in this class
            virtual void Get_Collision(double delta_time) {};
            virtual void Explosion() {};
            virtual void SetColliable(bool a) {};
            virtual void SetAlive(bool a) {};
            virtual void SetCollectible(bool a) {};
            virtual void CollectItem(int type) {};
            virtual void Invincible() {};
            virtual void SetState() {};
            virtual void SetPlayer(GameObject* a) {};
            virtual void SetCenter(glm::vec3& a) {};
            virtual void SetWidth(float a) {};
            virtual void SetHeight(float a) {};
            virtual void AddVelocity(glm::vec3 a) {};

            virtual void SetNumFrame(glm::vec2 n) { num_frame = n; };
            virtual void SetCurrentFrame(int c) { current_frame = c; };
            virtual void SetBar_Percent(float b);
            int GetCurrentFrame() { return current_frame; }
            int GetBar_Percent();
            virtual void SetOffset(int o) { offset = o; }

            virtual void setWant(bool s) { };
            virtual bool getShoot() { return false; }

            virtual int Get_INPoint() { return -1; }
            virtual int Get_Energy() { return -1; }
            virtual int Get_Iron() { return -1; }
            virtual int Get_Coin() { return -1; }
            virtual int Get_Exper() { return -1; }
            virtual int Get_Level() { return -1; }

            virtual void Set_INPoint(int i) {};
            virtual void Set_Energy(int e) {};
            virtual void Set_Iron(int i) {};
            virtual void Set_Coin(int c) {};
            virtual void Set_Exper(int e) {};
            virtual void Set_Level(int l) {};

            virtual void Add_Exp(int a) {};
            virtual void Level_up() {};
            virtual int Get_Max_Exp() { return -1; }
            virtual int Get_Max_Hp() { return -1; }
            virtual int Get_Max_Energy() { return -1; }
            virtual void setFortress(GameObject* f) {};
            virtual void become_angry() {};

            virtual GameObject* getFortress() { return NULL; };
            virtual void heal() {};

            virtual void SetShooter(GameObject* s) {};
            virtual glm::vec4 getShooter() { return glm::vec4(0,0,0,0); };

            virtual void Get_Collision_Pro(double delta_time, int pro_type, int attacker) {};

            virtual bool Get_Lazer_On() { return false; };
            virtual bool Get_Shield_On() { return false; };
            virtual void Set_Lazer_On(bool a)  {};
            virtual void Set_Shield_On(bool a)  {};
            virtual void Set_Rest(bool a) {};

            virtual Ract* GetRact() { return &Ract(); };
            virtual void setTarget(glm::vec3 p) {};
            virtual void MovingTo(double delta_time) {};

            virtual glm::mat4 GetTransformation() { return glm::mat4(1.0f); };
            virtual void SetTOO(glm::vec2 to) { toOrigin_ = to; };
            glm::vec2 GetTOO() { return toOrigin_; };
            virtual void SetMyTarget(glm::vec3 t) {};

            virtual void SetArm(GameObject* a) {};
            virtual void SetChild1(GameObject* a) {};
            virtual void SetChild2(GameObject* a) {};
            virtual GameObject* GetParent() { return NULL; };
            virtual void N_Parent() {};

            virtual void getHeal() {};

            virtual void AddChild(GameObject* child) {};
            virtual void SetWindowHeight(int w) {};
            virtual void SetWindowWidth(int h) {};

            virtual void SetPlace_Screen(float a, float b) { place_screen_ = glm::vec2(a, b); };
            glm::vec2 GetPlace_Screen() { return place_screen_; };

            virtual void UpdateValue(int index_1, int index_2, int index_3, float value) {};

            virtual void SetValue(float a) {};
            virtual void SetText(std::string text) {}
            virtual void SetZoom(float f) {}
            virtual float getZoom() { return 0.25f; };

            virtual void RenderMap(glm::mat4 view_matrix, double current_time) {};
            virtual void emptyChild() {};
            virtual GameObject* GetChild(int i) { return NULL; };

            virtual void SetPlayerSp(GameObject* p) {};
            virtual void SetEnemySp(GameObject* p) {};

            virtual void SetRGB(float r, float g, float b, float a) {
                rgba_ = glm::vec4(r, g, b, a);
            }
            virtual void SetAllChild(std::vector<GameObject*> a) {};
            virtual void SetSizeZoom(float f) {};
            virtual void SetShape_(int s) {};

            virtual void AddBomb()  {  };
            virtual void deleteBomb() {};
            virtual void SetCan_Lazer(bool a) {};
            virtual void SetCan_Shield(bool a) {};

            virtual int GetBomb() { return 0; };
            virtual bool Get_Can_lazer()  { return false; }
            virtual bool Get_Can_shield() { return false; }

            virtual void wakeup() {};
        protected:
            // Object's Transform Variables
            glm::vec3 position_;

            glm::vec2 scale_;//change it to vec2
            float angle_;
            Timer timer_exp;
            int type;//type to distinguish different type of gameobject
            glm::vec3 velocity_;//velocity for object moving
            bool ghost;//if it is ghost state

            glm::vec2 num_frame;
            int current_frame;
            float bar_percent;

            int offset=0;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;

            //物体在屏幕上相对的位置
            glm::vec2 place_screen_;
            glm::vec4 rgba_;
            glm::vec2 toOrigin_;


            


    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
