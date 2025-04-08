#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>

#include <random>   // for srand() and rand()
#include <ctime>    // for time()

#include <path_config.h>

#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
#include "collectible_game_object.h"
#include "blue_game_object.h"
#include "expo_obj.h"
#include "enemy_game_object.h"
#include "fortress_object.h"
#include "fortress_shooter.h"
#include "projectile.h"
#include "game.h"
#include "monster_object.h"
#include "lazer.h"
#include "effect.h"
#include "arm1.h"
#include "particles.h"
#include "particle_system.h"

#include "bar.h"
#include "Value_Object.h"

#include "text_game_object.h"

#include "drawing_game_object.h"

#include "mini_map_Object.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Borderspace"; //change name to assignment 3
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;

const unsigned int world_scale = 100;
const float minX = -50.0f;
const float maxX = 50.0f;    
const float minY = -50.0f;
const float maxY = 50.0f;   



class Background : public Sprite {//add a new sprite setting for background
public:
    void CreateGeometry() override {

        GLfloat vertex[] = {
            -5.0f,  5.0f, 1.0f,0.0f,0.0f,  0.0f, 0.0f,
             5.0f,  5.0f, 0.0f,1.0f,0.0f,  10.0f, 0.0f,//make it tile 
             5.0f, -5.0f, 0.0f,0.0f,1.0f,  10.0f, 10.0f,
            -5.0f, -5.0f, 1.0f,1.0f,1.0f,  0.0f, 10.0f
        };

        // Two triangles referencing the vertices
        GLuint face[] = {
            0, 1, 2, // t1
            2, 3, 0  // t2
        };

        // Create buffer for vertices
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

        // Create buffer for faces (index buffer)
        glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

        // Set number of elements in array buffer (6 in this case)
        size_ = sizeof(face) / sizeof(GLuint);
        
    }
};
void Game::SetupGameWorld(void)
{

    // Setup the game world

    // **** Load all the textures that we will need
    //
    // Declare all the textures here
    std::vector<std::string> textures;
    enum {
        tiny_ship1 = 0,//change name
        //tex_green_ship = 1,
        tiny_ship17 = 1,//change name
        tex_stars = 2,
        tex_explo = 3,
        item = 4,//new texture for item
        tex_orb = 5,
        in_player = 6,//new texture for invincible player
        projectile = 7,//new texture for shooter
        healthy_bar = 8,
        number = 9,
        exp_bar = 10,
        energy_bar = 11,
        level_text = 12,
        exp_text = 13,
        tex_ex_minion = 14,
        tex_items = 15,
        tex_for = 16,
        tex_bbb = 17,
        tex_monster = 18,
        tex_for_s = 19,
        tex_boss = 20,
        tex_bomb = 21,
        tex_beam = 22,
        tex_shield = 23,
        tex_arm1 = 24,
        tex_arm2 = 25,
        tex_arm3 = 26,
        tex_font = 27,
        tex_test = 28,
        tex_lidar = 29,
        tex_node = 30,
        tex_level2=31,
        tex_wall=32,
        tex_path=33
    };
    textures.push_back("/textures/tiny_ship1.png"); //change file
    //textures.push_back("/textures/destroyer_green.png"); 
    textures.push_back("/textures/tiny_ship17.png");//change file
    textures.push_back("/textures/stars.png");
    textures.push_back("/textures/Explosion_Medium.png");//add exp picture
    textures.push_back("/textures/item.png");
    textures.push_back("/textures/orb.png");
    textures.push_back("/textures/in_player.png");
    textures.push_back("/textures/Shot_Blue.png");//add projectile shooter
    textures.push_back("/textures/HP_bar.png");
    textures.push_back("/textures/Number_font (8 x 8).png");

    textures.push_back("/textures/exp_bar.png");
    textures.push_back("/textures/Energy_bar.png");
    textures.push_back("/textures/level.png");
    textures.push_back("/textures/exp.png");
    textures.push_back("/textures/tiny_ship3.png");
    textures.push_back("/textures/IP+E+I+C (16 x 16).png");
    textures.push_back("/textures/Fortress.png");
    textures.push_back("/textures/blue_ship.png");
    textures.push_back("/textures/monster.png");
    textures.push_back("/textures/fortress_shooter.png");
    textures.push_back("/textures/boss.png");
    textures.push_back("/textures/bomb.png");
    textures.push_back("/textures/Beam_Big_Blue.png");
    textures.push_back("/textures/shield.png");
    textures.push_back("/textures/arm1.png");
    textures.push_back("/textures/arm2.png");
    textures.push_back("/textures/arm3.png");
    textures.push_back("/textures/font8x8.png");
    textures.push_back("/textures/test.png");
    textures.push_back("/textures/ridar.png");
    textures.push_back("/textures/node.png");
    textures.push_back("/textures/level2.png");
    textures.push_back("/textures/bricks.png");
    textures.push_back("/textures/road.png");

    


    // Load textures
    LoadTextures(textures);

    //setup timer for new enemy
    timer.Start(10.0);
    level_1_timer.Start(5.0);
    // **** Setup all the game objects in the world

    // Setup the player object (position, texture, vertex count,hp,circle)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 





    
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tiny_ship17],100,Circle()));//change texture, add hitpoint, circle

    
   // hud_ = new HUD(hp_spirt, num_spirt, energy_spirt, exp_spirt, exp_text_s, lv_text_s, game_objects_[0]);
    
    float pi_over_two = glm::pi<float>() / 2.0f;

    // Setup enemy objects
    game_objects_.push_back(new MonsterObject(glm::vec3(2.0f, -2.0f, 0.0f), sprite_, &number_shader_, tex_[tex_monster],Circle(),current_time_,0,95));//change texture,add hitpoint, circle, random number for different moving mode
    //game_objects_.push_back(new BlueGameObject(glm::vec3(1.0f, 2.5f, 0.0f), sprite_, &sprite_shader_, tex_[tex_bbb],Circle(),current_time_, 0,93));//change texture，add hitpoint, circle
    
    fortress_exist_ = true;
    fortress_ = new FortressObject(glm::vec3(6.0f, 10.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_for], Circle(), current_time_, 0, 94);

   // GameObject* fortress_shooter_ = new FortressShooter(glm::vec3(-0.9f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_for_s], fortress_);

    
    Generate_Fortress(fortress_);

    
    //setup item object
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(5.2f, 0.0f, 0.0f), sprite_, &number_shader_, tex_[tex_items], Circle(), 14));
    
    GameObject* shop_number_bar_= new Bar(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[28], game_objects_[0]);
    shop_number_bar_->SetScale(glm::vec2(0.5, 0.5));
    GameObject* node_sprite = new DrawingGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &drawing_shader_, tex_[tex_node]);

    GameObject* in_number = new Value_Object(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &number_shader_, tex_[9], 2, 0, shop_number_bar_);
    GameObject* in_p = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &number_shader_, tex_[15]);
    GameObject* text = new TextGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &text_shader_, tex_[27]);
    shop_.BuildShop(node_sprite, in_p, in_number, text);
    shop_.SetPlayer(game_objects_[0]);

    //GameObject*  shop= new DrawingGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &drawing_shader_, tex_[5]);
    //shop->SetScale(glm::vec2(5.0f, 4.0f));
    //shop->SetRGB(1.0, 1.0, 1.0, 1.0);
    //shop->SetShape_(2);
    //shop_bar_->AddChild(shop);
   // shop_bar_->SetGhost(true);

    //set circle radius for each
    for (int i = 0;i < game_objects_.size();i++) {
        game_objects_[i]->GetCircle()->SetRadius(game_objects_[i]->GetScale().x / 2);//set the circle radius as scale/2
        game_objects_[i]->SetRotation(pi_over_two);
        game_objects_[i]->SetPlayer(game_objects_[0]);//set player pointer for each enemy
    }

    GameObject* lazer_ = new Lazer(glm::vec3(1.0f, 0.0f, 0.0f), sprite_, &animate_shader_, tex_[tex_beam], game_objects_[0]);
    lazer_->SetNumFrame(glm::vec2(2, 1));
    game_objects_.push_back(lazer_);
    GameObject* shield = new Effect(glm::vec3(0.0f, 0.0f, -1.0f), sprite_, &animate_shader_, tex_[tex_shield], game_objects_[0], 31);
    shield->SetNumFrame(glm::vec2(5, 1));
    game_objects_.push_back(shield);
    game_objects_[4]->GetCircle()->SetRadius(game_objects_[4]->GetScale().x*0.6);

    game_objects_[0]->AddChild(lazer_);
    game_objects_[0]->AddChild(shield);

    GameObject* BarObj = new Bar(glm::vec3(0.0f, 0.0f, -1.0f), sprite_, &sprite_shader_, tex_[28], game_objects_[0]);
    game_objects_.push_back(BarObj);
    Setup_HUD_Bar(BarObj);
    game_objects_[0]->AddChild(BarObj);

    
    GameObject* minimap_bar = new Bar(glm::vec3(0.0f, 0.0f, -0.0f), sprite_, &sprite_shader_, tex_[28],game_objects_[0]);

    Setup_Mini_Map(minimap_bar);
    game_objects_.push_back(minimap_bar);
    // Setup background
    // In this specific implementation, the background is always the
    // last object
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), bg, &sprite_shader_, tex_[2]);
    background->SetScale(glm::vec2(12.0,12.0));
    game_objects_.push_back(background);

    //Setup Audio
    // Set position of listener
    am.SetListenerPosition(0.0, 0.0, 0.0);
    // Load first sound to be played
    std::string filename = std::string(RESOURCES_DIRECTORY).append(std::string("/audio/").append(std::string("Background.wav")));
    background_music_index = am.AddSound(filename.c_str());
    filename = std::string(RESOURCES_DIRECTORY).append(std::string("/audio/").append(std::string("Explosion.wav")));//set the explosion music
    explosion_music_index = am.AddSound(filename.c_str());
    // Set sound properties
    am.SetSoundPosition(background_music_index, -10.0, 0.0, 0.0);
    am.SetLoop(background_music_index, true);//set the background music looping until the gameworld need to be destoried
    am.PlaySound(background_music_index);//play music

    Setup_HUD_Value();

    // Setup particle system
    GameObject* particles = new ParticleSystem(glm::vec3(-0.5f, 0.0f, 0.0f), particles_, &particle_shader_, tex_[tex_orb], game_objects_[0]);
    particles->SetScale(glm::vec2(0.2, 0.2));
    particles->SetRotation(-pi_over_two);
    game_objects_.push_back(particles);
}



void Game::Setup_Mini_Map(GameObject* m) {
    GameObject* minimap_bar = m;
    int width, height;
    glfwGetWindowSize(window_, &width, &height);

    GameObject* minimap = new Mini_map_Object(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[29], minimap_bar);
    DrawingGameObject* circle = new DrawingGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &drawing_shader_, tex_[5]);

    hud_objects_.push_back(minimap);
    hud_objects_.push_back(circle);

    minimap->SetPlayerSp(circle);
    minimap_bar->AddChild(minimap);
    minimap->SetPlayer(game_objects_[0]);
    minimap_ = minimap;
    minimap_bar->SetScale(glm::vec2(1.0, 1.0));
    minimap_bar->SetPlace_Screen(0.5, 0.5);
    minimap_bar->SetTOO(glm::vec2(0, 0));
    minimap->SetTOO(glm::vec2(0.5, 0.5));
    minimap->SetScale(glm::vec2(2.35, 1.875));
    minimap->SetPlace_Screen(0.0, 1.0);
    minimap_bar->SetWindowHeight(600);
    minimap_bar->SetWindowWidth(800);
   // minimap->SetWindowHeight(200);
  //  minimap->SetWindowWidth(150);
    
}
void Game::Setup_HUD_Bar(GameObject* h) {
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    GameObject* BarObj = h;

    
    GameObject* hp_ui_bar = new Bar(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[28], BarObj);
    GameObject* Exp_ui_bar = new Bar(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[28], BarObj);
    GameObject* text_bar = new Bar(glm::vec3(0.0f, 0.4f, 0.0f), sprite_, &sprite_shader_, tex_[28], BarObj);
    GameObject* number_bar = new Bar(glm::vec3(-1.9f, 0.25f, 0.0f), sprite_, &sprite_shader_, tex_[28], BarObj);

    GameObject* collaction_bar = new Bar(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[28], BarObj);



    GameObject* hp_sp = new Value_Object(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &bar_shader_, tex_[8], 1, 1, hp_ui_bar);
    GameObject* energy_sp = new Value_Object(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &bar_shader_, tex_[11], 1, 1, hp_ui_bar);
    GameObject* exp_text_s = new GameObject(glm::vec3(2.2f, 0.25f, 0.0f), sprite_, &sprite_shader_, tex_[13]);
    GameObject* lv_text_s = new GameObject(glm::vec3(-2.2f, 0.25f, 0.0f), sprite_, &sprite_shader_, tex_[12]);
    GameObject* level_number = new Value_Object(glm::vec3(0.2f, 0.0f, 0.0f), sprite_, &number_shader_, tex_[9], 2, 0, number_bar);
    GameObject* exp_sp = new Value_Object(glm::vec3(0.0f, -0.2f, 0.0f), sprite_, &bar_shader_, tex_[10], 1, 1, Exp_ui_bar);


    

    GameObject* in_p = new GameObject(glm::vec3(-1.7f, -0.195f, 0.0f), sprite_, &number_shader_, tex_[15]);
    in_p->SetScale(glm::vec2(0.5, 0.5));
    in_p->SetNumFrame(glm::vec2(4, 1));
    in_p->SetCurrentFrame(0);
    GameObject* in_i = new GameObject(glm::vec3(-1.7f, -0.595f, 0.0f), sprite_, &number_shader_, tex_[15]);
    in_i->SetScale(glm::vec2(0.5, 0.5));
    in_i->SetNumFrame(glm::vec2(4, 1));
    in_i->SetCurrentFrame(2);
    GameObject* in_c = new GameObject(glm::vec3(-1.7f, -0.995f, 0.0f), sprite_, &number_shader_, tex_[15]);
    in_c->SetScale(glm::vec2(0.5, 0.5));
    in_c->SetNumFrame(glm::vec2(4, 1));
    in_c->SetCurrentFrame(3);

    GameObject* in_b = new GameObject(glm::vec3(0.0, -0.5f, 0.0f), sprite_, &number_shader_, tex_[21]);
    in_b->SetScale(glm::vec2(0.5, 0.5));
    in_b->SetNumFrame(glm::vec2(2, 1));
    in_b->SetCurrentFrame(0);
    in_b->SetTOO(glm::vec2(0.5, 0.5));
    GameObject* bomb_number = new Value_Object(glm::vec3(0.5f, -0.5f, 0.0f), sprite_, &number_shader_, tex_[9], 2, 0, hp_ui_bar);
    bomb_number->SetTOO(glm::vec2(0.5, 0.5));
    GameObject* in_number_bar = new Bar(glm::vec3(-1.3f, -0.195f, 0.0f), sprite_, &sprite_shader_, tex_[28], BarObj);
    GameObject* iron_number_bar = new Bar(glm::vec3(-1.3f, -0.595f, 0.0f), sprite_, &sprite_shader_, tex_[28], BarObj);
    GameObject* coin_number_bar = new Bar(glm::vec3(-1.3f, -0.995f, 0.0f), sprite_, &sprite_shader_, tex_[28], BarObj);


    GameObject* in_number = new Value_Object(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &number_shader_, tex_[9], 2, 0, in_number_bar);
    GameObject* iron_number = new Value_Object(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &number_shader_, tex_[9], 2, 0, iron_number_bar);
    GameObject* coin_number = new Value_Object(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &number_shader_, tex_[9], 2, 0, coin_number_bar);


    hud_objects_.push_back(hp_ui_bar);
    hud_objects_.push_back(Exp_ui_bar);
    hud_objects_.push_back(text_bar);
    hud_objects_.push_back(number_bar);
    hud_objects_.push_back(collaction_bar);
    hud_objects_.push_back(hp_sp);
    hud_objects_.push_back(energy_sp);
    hud_objects_.push_back(exp_text_s);
    hud_objects_.push_back(lv_text_s);
    hud_objects_.push_back(level_number);
    hud_objects_.push_back(exp_sp);
    hud_objects_.push_back(in_p);
    hud_objects_.push_back(in_i);
    hud_objects_.push_back(in_c);
    hud_objects_.push_back(in_number_bar);
    hud_objects_.push_back(iron_number_bar);
    hud_objects_.push_back(coin_number_bar);
    hud_objects_.push_back(in_number);
    hud_objects_.push_back(iron_number);
    hud_objects_.push_back(coin_number);
    hud_objects_.push_back(in_b);
    hud_objects_.push_back(bomb_number);

    

    /*
    // 
    //  text->SetScale(glm::vec2(2.5/1.5, 0.5/1.5));
 // text->SetTOO(glm::vec2(0.5f, 0.5f));
//  text->SetText("LEVEL");
  //thequickbrownfoxjumpsoveralazydog
  // //THEQUICKBROWNFOXJUMPSOVERALAZYDOG
  //game_objects_.push_back(text);
  */
    hp_ui_bar->SetPlace_Screen(0, 0.1);
    hp_ui_bar->SetTOO(glm::vec2(0.5, 0.5));

    Exp_ui_bar->SetPlace_Screen(0.5, 1.0);
    Exp_ui_bar->SetTOO(glm::vec2(0.0, 0.5f));
    Exp_ui_bar->SetScale(glm::vec2(5, 1.0f));

    text_bar->SetScale(glm::vec2(5.0, 0.5));
    text_bar->SetPlace_Screen(0.0, 0.0);
    text_bar->SetTOO(glm::vec2(0.0f, 0.5f));

    collaction_bar->SetScale(glm::vec2(2.0f, 1.0f));
    collaction_bar->SetPlace_Screen(1.0f, 0.0f);
    collaction_bar->SetTOO(glm::vec2(-0.5f, -0.5f));



    hp_sp->SetTOO(glm::vec2(0.5, 0.4));
    hp_sp->SetScale(glm::vec2(2, 1));
    hp_sp->SetNumFrame(glm::vec2(2, 1));

    energy_sp->SetTOO(glm::vec2(0.5, 0.4));
    energy_sp->SetScale(glm::vec2(2, 1));
    energy_sp->SetNumFrame(glm::vec2(2, 1));

    exp_sp->SetScale(glm::vec2(5, 1));
    exp_sp->SetNumFrame(glm::vec2(2, 1));
    exp_sp->SetTOO(glm::vec2(0.0, 0.5));
    lv_text_s->SetScale(glm::vec2(0.5, 0.5));
    exp_text_s->SetScale(glm::vec2(0.75, 0.5));

    level_number->SetScale(glm::vec2(0.5, 0.5));
    level_number->SetNumFrame(glm::vec2(5, 2));

    in_number->SetScale(glm::vec2(0.4, 0.4));
    in_number->SetNumFrame(glm::vec2(5, 2));

    iron_number->SetScale(glm::vec2(0.4, 0.4));
    iron_number->SetNumFrame(glm::vec2(5, 2));

    coin_number->SetScale(glm::vec2(0.4, 0.4));
    coin_number->SetNumFrame(glm::vec2(5, 2));

    bomb_number->SetScale(glm::vec2(0.4, 0.4));
    bomb_number->SetNumFrame(glm::vec2(5, 2));

    BarObj->SetPlace_Screen(0.5, 0.5);
    BarObj->SetTOO(glm::vec2(0, 0));

    hp_ui_bar->AddChild(hp_sp);
    hp_ui_bar->AddChild(energy_sp);
    hp_ui_bar->AddChild(in_b);
    hp_ui_bar->AddChild(bomb_number);
    Exp_ui_bar->AddChild(exp_sp);
    Exp_ui_bar->AddChild(text_bar);
    text_bar->AddChild(number_bar);
    text_bar->AddChild(lv_text_s);
    text_bar->AddChild(exp_text_s);


    number_bar->AddChild(level_number);
    number_bar->SetTOO(glm::vec2(0.5, 0.0));
    number_bar->SetScale(glm::vec2(1.0f, 0.5));

    in_number_bar->AddChild(in_number);
    in_number_bar->SetTOO(glm::vec2(0.5, 0.0));
    in_number_bar->SetScale(glm::vec2(1.2f, 0.4));

    iron_number_bar->AddChild(iron_number);
    iron_number_bar->SetTOO(glm::vec2(0.5, 0.0));
    iron_number_bar->SetScale(glm::vec2(1.2f, 0.4));

    coin_number_bar->AddChild(coin_number);
    coin_number_bar->SetTOO(glm::vec2(0.5, 0.0));
    coin_number_bar->SetScale(glm::vec2(1.2f, 0.4));

    collaction_bar->AddChild(in_p);
    collaction_bar->AddChild(in_i);
    collaction_bar->AddChild(in_c);
    collaction_bar->AddChild(in_number_bar);
    collaction_bar->AddChild(iron_number_bar);
    collaction_bar->AddChild(coin_number_bar);



    BarObj->AddChild(hp_ui_bar);
    BarObj->AddChild(Exp_ui_bar);
    BarObj->AddChild(collaction_bar);
   // BarObj->AddChild(minimap_bar);




    BarObj->SetWindowHeight(height);
    BarObj->SetWindowWidth(width);



}
void Game::Setup_HUD_Value() {
    GameObject* player = game_objects_[0];
    HUDValue_.hp = player->GetHP();
    HUDValue_.max_hp = player->Get_Max_Hp();
    HUDValue_.energy = player->Get_Energy();
    HUDValue_.max_energy = player->Get_Max_Energy();
    HUDValue_.exp = player->Get_Exper();
    HUDValue_.max_exp = player->Get_Max_Exp();
    HUDValue_.level = player->Get_Level();
    HUDValue_.money = player->Get_Coin();
    HUDValue_.ip = player->Get_INPoint();
    HUDValue_.iron = player->Get_Iron();

    HUDValue_.bomb = player->GetBomb();
    
}

void Game::Update_HUD_Value() {
    Setup_HUD_Value();
    float p = (float)HUDValue_.hp / HUDValue_.max_hp;
    int barO = game_objects_.size() - 4;
    game_objects_[barO]->UpdateValue(0,0,0, p);
    p = (float)HUDValue_.energy / HUDValue_.max_energy;
    p = (17.0f * p + 9.0f) / 32.0f;
    game_objects_[barO]->UpdateValue(0,1,0, p);

    p = (float)HUDValue_.exp / HUDValue_.max_exp;
    game_objects_[barO]->UpdateValue(1, 0, 0, p);

    int lev = HUDValue_.level;
    game_objects_[barO]->UpdateValue(1, 1, 0, lev);

    game_objects_[barO]->UpdateValue(2, 3, 0, HUDValue_.ip);
    game_objects_[barO]->UpdateValue(2, 4, 0, HUDValue_.iron);
    game_objects_[barO]->UpdateValue(2, 5, 0, HUDValue_.money);

    game_objects_[barO]->UpdateValue(0, 3, 0, HUDValue_.bomb);
   // collaction_bar->AddChild(in_number_bar);
  //  collaction_bar->AddChild(iron_number_bar);



}
void Game::DestroyGameWorld(void)
{
    //shutdown audio
    am.ShutDown();
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }
    for (int i = 0; i < hud_objects_.size(); i++) {
        delete hud_objects_[i];
    }

}


void Game::HandleControls(double delta_time)
{
    // Get player game object
    GameObject *player = game_objects_[0];
    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();

    glm::vec3 velo = player->GetVelocity();

    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = player->GetSpeed();//get the unit of speed from the player
    float motion_increment = speed * (float)delta_time;//calculate the increment by delta_time
    float angle_increment = (glm::pi<float>() / 1800.0f)*speed;//calculate the increment by delta_time
    //std::cout << "speed!"<< speed << std::endl;//print gameover

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player->AddVelocity(dir * motion_increment);//velocity++
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->AddVelocity(-dir*motion_increment);//velocity--
    }
    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {

        player->AddVelocity(-player->GetRight() * motion_increment);//velocity++ and change the direction of the velocity
    }
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {

        player->AddVelocity(player->GetRight() * motion_increment);//velocity++ and change the direction of the velocity
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->SetRotation(angle - angle_increment);//change direction only
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->SetRotation(angle + angle_increment);//change direction only
    }
    if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
     //   player->SetPosition(curpos - motion_increment*player->GetRight());
    }
    if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
      //  player->SetPosition(curpos + motion_increment*player->GetRight());
    }
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    if (glfwGetKey(window_, GLFW_KEY_J) == GLFW_PRESS) {//press J to shoot
        if(shooter_timer.Finished()){//cooldown 
            game_objects_.insert(game_objects_.begin() + 1, new Projectile(player->GetPosition(), sprite_, &sprite_shader_, tex_[7], Circle(), angle,1,51));
            //the projectile is start at the player current position, and faster velocity with same direction as player
            game_objects_[1]->GetCircle()->SetRadius(game_objects_[1]->GetScale().x / 2);//set the circle radius
            shooter_timer.Start(0.5f);//cooldown
        }
    }
    if (glfwGetKey(window_, GLFW_KEY_K) == GLFW_PRESS) {//press K to shoot bomb
        if (shooter_timer.Finished()) {//cooldown 
            if (player->GetBomb() > 0) {
                game_objects_.insert(game_objects_.begin() + 1, new Projectile(player->GetPosition(), sprite_, &animate_shader_, tex_[21], Circle(), angle, 1, 53));
                //the projectile is start at the player current position, and faster velocity with same direction as player
                game_objects_[1]->GetCircle()->SetRadius(game_objects_[1]->GetScale().x / 2);//set the circle radius
                player->deleteBomb();
                shooter_timer.Start(0.5f);//cooldown
               
            }
        }
    }
    if (glfwGetKey(window_, GLFW_KEY_L) == GLFW_PRESS) {//press J to shoot
        //std::cout << "x: "<<player->GetPosition().x <<" y:  "<<player->GetPosition().y << std::endl;;
        player->Set_Lazer_On(true);
        
    }
    if (glfwGetKey(window_, GLFW_KEY_L) == GLFW_RELEASE) {
        player->Set_Lazer_On(false);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {//press J to shoot
        player->Set_Shield_On(true);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        player->Set_Shield_On(false);
    }
    if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS) {
        if (map_zoom_ <= 0.25) {
            map_zoom_ += 0.0005f;
        }
    }
    if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (map_zoom_ >= 0.01f) {
            map_zoom_ -= 0.0005f;
        }
    }
    if (glfwGetKey(window_, GLFW_KEY_ENTER) == GLFW_PRESS) {
        setStop(!getStop());
    }
    if (glfwGetKey(window_, GLFW_KEY_M) == GLFW_PRESS) {
        setMap(!getMap());
    }
    if (glfwGetKey(window_, GLFW_KEY_P) == GLFW_PRESS) {
        shop_.OpenShop();
    }
}
void Game::setMap(bool b) {
    map_ = b;
}
void Game::set_up_maze() {
    maze_setup = true;
    GameObject* wall_sp = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[32]);

    wall_sp->SetScale(glm::vec2(1.5, 1.5));
    // Setup sprite used as graph edge
    GameObject* path_sp = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[33]);
    path_sp->SetScale(glm::vec2(1.5, 1.5));
    Maze temp;
    temp.BuildGrid(17, 13, 3.0,3.0, 0, 0, 4, wall_sp, path_sp);
    maze_.BuildEmptyMaze(wall_sp, path_sp);
    temp.BuildMaze(maze_);
    maze_.BuildWall();
    std::vector<Node*> temp_wall = maze_.GetAllWall();
    Node* current_node;
    glm::vec3 wall_position;
    GameObject* brick;
    for (int i = 0;i < temp_wall.size()-2;i++) {
        current_node = temp_wall[i];
        wall_position = glm::vec3(current_node->GetX(), current_node->GetY(), 0.0);
        if (current_node->IsWall()) {
            brick = new Wall(wall_position, sprite_, &sprite_shader_, tex_[32]);
            brick->SetScale(glm::vec2(1.5, 1.5));
            game_objects_.insert(game_objects_.begin() + 1, brick);
        }
    }
    
    
    
}
void Game::BossRoom() {
    GameObject* background = game_objects_[game_objects_.size() - 1];
    background->SetTexture(tex_[31]);

}

void Game::wakeup_monster() {
    for (int i = 0; i < game_objects_.size() - 4; i++) {
        GameObject* current_game_object = game_objects_[i];
        if (current_game_object->GetType() == 95) {
            current_game_object->wakeup();
        }
    }
}
void Game::destory_level_1() {

    for (int i = 1; i < game_objects_.size()-4; i++) {
        game_objects_[i]->SetAlive(false);
    }
    for (int i = 0;i < (game_objects_.size() - 3);i++) {
        if (game_objects_[i]->GetAlive() == false) {
            delete game_objects_[i];//delete the obj
            game_objects_.erase(game_objects_.begin() + i);//shrink vector
        }
    }

}
void Game::set_up_level_2() {
    GameObject* lazer_ = new Lazer(glm::vec3(1.0f, 0.0f, 0.0f), sprite_, &animate_shader_, tex_[22], game_objects_[0]);
    lazer_->SetNumFrame(glm::vec2(2, 1));
    game_objects_.insert(game_objects_.begin() + 1, lazer_);
    GameObject* shield = new Effect(glm::vec3(0.0f, 0.0f, -1.0f), sprite_, &animate_shader_, tex_[23], game_objects_[0], 31);
    shield->SetNumFrame(glm::vec2(5, 1));
    game_objects_.insert(game_objects_.begin() + 1, shield);
    game_objects_[0]->AddChild(lazer_);
    game_objects_[0]->AddChild(shield);
    game_objects_[0]->SetPosition(glm::vec3(0, 0, 0));

    set_up_maze();
    GameObject* boss = new MonsterObject(glm::vec3(-20.0f, -20.0f, 0.0f), sprite_, &animate_shader_, tex_[20], Circle(), current_time_, 0, 100);
    boss->SetNumFrame(glm::vec2(1, 1));
    boss->SetCurrentFrame(0);
    boss->GetCircle()->SetRadius(boss->GetScale().x / 2);//set the circle radius as scale/2
    boss->SetPlayer(game_objects_[0]);//set player pointer for each enemy
    game_objects_.insert(game_objects_.begin() + 1, boss);

    std::cout << "set up maze!!" << std::endl;
}

bool Game::check_level_2() {
    int count=0;
    for (int i = 0; i < game_objects_.size() - 4; i++) {
        GameObject* current_game_object = game_objects_[i];
        if (current_game_object->GetType() == 95) {
            if ( (abs(current_game_object->GetPosition().x) < 0.5) && (abs(current_game_object->GetPosition().y) < 0.5) ){
                count++;
            }
        }
    }
    
    if ( count > 0) {
        return true;
    }


    return false;

}

void Game::CollisionEvent(GameObject* object1, GameObject* object2,double delta_time) {
    //std::cout << "hit!!!!" << std::endl;
    glm::vec3 player, wall;
    glm::vec2 circleCenter, ractCenter, delta;
    float halfWidth, halfHeight;
    float bounceFactor = 0.2f;
    if ((object1->GetType() == 1) && object2->GetType() == 41) {
        player = object1->GetPosition();
        wall = object2->GetPosition();
        circleCenter = glm::vec2(player.x, player.y);
        ractCenter = glm::vec2(wall.x, wall.y);
        glm::vec3 w, v_0, v_p, v_out, v;
        glm::vec2 delta = circleCenter - ractCenter;

        v = object1->GetVelocity();

        halfWidth = object2->GetRact()->get_W() / 2;
        halfHeight = object2->GetRact()->get_H() / 2;

        float dx = delta.x;
        float dy = delta.y;

        float overlapX = halfWidth - std::abs(dx);
        float overlapY = halfHeight - std::abs(dy);
        if (overlapX < overlapY) {
            // 撞在左右边
            w = glm::vec3((dx > 0) ? 1.0f : -1.0f, 0.0f, 0);
        }
        else {
            // 撞在上下边
            w = glm::vec3(0.0f, (dy > 0) ? 1.0f : -1.0f, 0);
        }

        v_0 = glm::dot(v, w) * w;
        v_out = v - 2.0f * v_0;
        v = v_out * bounceFactor;
        object1->CollideWall(0.1, v_out);
    }

}

bool Game::collision_Check(GameObject* a, GameObject* b) {
    GameObject* current = a;
    GameObject* other = b;
    CircleCollider circle_collider=CircleCollider();
    RactCollider ract_collider = RactCollider();
    if (current->GetType() == 1&&other->GetType()==41) {
        circle_collider.SetRadius(current->GetCircle()->get_r());
        circle_collider.SetObject(current);
        ract_collider.SetWidth(other->GetRact()->get_W());
        ract_collider.SetHeight(other->GetRact()->get_H());
        ract_collider.SetObject(other);
        return circle_collider.TestCollision(ract_collider);
    }
    else if(current->GetType()==41&& other->GetType() == 1){
        circle_collider.SetRadius(other->GetCircle()->get_r());
        circle_collider.SetObject(other);
        ract_collider.SetWidth(current->GetRact()->get_W());
        ract_collider.SetHeight(current->GetRact()->get_H());
        ract_collider.SetObject(current);

        return circle_collider.TestCollision(ract_collider);

    }

}



void Game::Update(double delta_time)
{
<<<<<<< Updated upstream
    if (level_1_timer.Finished()) {
        if (!changing_level_) {
            changing_level_ = true;
            generateDifferentEnemy();
        } 
        wakeup_monster();
=======
    if (level_1_timer.Finished()&& changing_timer.Finished()) {
        if ((!changing_level_) &&(level_==1)) {
                changing_timer.Start(10);
                changing_level_ = true;
                generateDifferentEnemy();
                std::cout << "im here" << std::endl;
                wakeup_monster();
            }
>>>>>>> Stashed changes
    }
    //level_ = 2;

    if (changing_level_&&check_level_2()) {
        if (!maze_setup) {
            
            destory_level_1();
            BossRoom();

            set_up_level_2();
           // std::cout << "check 2 correct" << std::endl;
        }
    }
    if (changing_level_&&changing_timer.Finished()) {
        changing_level_ = false;
        level_ = 2;
    }


        if (getStop()) { return; }
        current_time_ += delta_time;
        GameObject* player = game_objects_[0];

        if (changing_level_) {
            for (int i = 1; i < game_objects_.size() - 4; i++) {
                GameObject* current_game_object = game_objects_[i];
                // Update the current game object
                current_game_object->Update(delta_time);
            }
        }

        else {
            // Update all game objects
            for (int i = 0; i < game_objects_.size() - 4; i++) {
                // Get the current game object
                GameObject* current_game_object = game_objects_[i];

                // Update the current game object
                current_game_object->Update(delta_time);


                // Check for collision with other game objects
                // Note the loop bounds: we avoid testing the last object since
                // it's the background covering the whole game world
                for (int j = i + 1; j < (game_objects_.size() - 4); j++) {
                    GameObject* other_game_object = game_objects_[j];

                    

                    // Compute distance between object i and object j
                    float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());
                    // If distance is below a threshold, we have a collision
                    if (other_game_object->GetColliable() && current_game_object->GetColliable()) {//if the two obj is colliable, then continue.
                        switch (current_game_object->GetType())//change if else to switch, make it more clear
                        {//player=1; colliable=2, enemy=3; projectile=4,defult=0
                        case 1://current type is player. player can colliable with collectible, enemy
                            if (distance < 0.8f * (current_game_object->GetCircle()->get_r() + other_game_object->GetCircle()->get_r())) {
                                if (other_game_object->GetType() > 90 && other_game_object->GetType() != 94)//if it is enemy
                                {
                                    //if the two obj has a distance smaller than 0.8 *(sum of the radius), then it there is a collisoin
                                    // This is where you would perform collision response between objects
                                    other_game_object->Get_Collision(delta_time);//current and other obj get collision.
                                    current_game_object->Get_Collision(delta_time);
                                    // Play the sound when collision
                                    if (!am.SoundIsPlaying(explosion_music_index)) {//if the sound is already playing, don't play it twice
                                        am.PlaySound(explosion_music_index);
                                    }
                                    else {//stop it first then replay it
                                        am.StopSound(explosion_music_index);
                                        am.PlaySound(explosion_music_index);
                                    }
                                }
                                else if (other_game_object->GetType() == 94 && other_game_object->GetState() == 9) {

                                    other_game_object->Get_Collision(delta_time);//current and other obj get collision.
                                    current_game_object->Get_Collision(delta_time);
                                    // Play the sound when collision
                                    if (!am.SoundIsPlaying(explosion_music_index)) {//if the sound is already playing, don't play it twice
                                        am.PlaySound(explosion_music_index);
                                    }
                                    else {//stop it first then replay it
                                        am.StopSound(explosion_music_index);
                                        am.PlaySound(explosion_music_index);
                                    }



                                }
                                else if ((other_game_object->GetType() > 10) && (other_game_object->GetType() < 20)) {//if it is collect
                                    other_game_object->Get_Collision(delta_time);//item change color
                                    current_game_object->CollectItem(other_game_object->GetType());//player item++
                                }
                               
                                else if ((other_game_object->GetType() > 50) && (other_game_object->GetType() < 60)) {//(other_game_object->GetType() > 50) && (other_game_object->GetType() < 60)
                                    if (other_game_object->getFrom() != 1) {
                                        other_game_object->Get_Collision(delta_time);//current and other obj get collision.
                                        current_game_object->Get_Collision_Pro(delta_time, other_game_object->GetType(), other_game_object->getFrom());
                                        // Play the sound when collision
                                        if (!am.SoundIsPlaying(explosion_music_index)) {//if the sound is already playing, don't play it twice
                                            am.PlaySound(explosion_music_index);
                                        }
                                        else {//stop it first then replay it
                                            am.StopSound(explosion_music_index);
                                            am.PlaySound(explosion_music_index);
                                        }
                                    }
                                }
                            }
                            if (other_game_object->GetType() == 41) {
                                if (collision_Check(current_game_object, other_game_object)) {
                                    CollisionEvent(current_game_object, other_game_object,delta_time);
                                }
                            }
                            break;
                        case 51://current type is projectile
                            if ((other_game_object->GetType() < 20) && (other_game_object->GetType() > 10)) {

                            }
                            else if ((other_game_object->GetType() > 80) && (other_game_object->GetType() < 90)) {
                                break;
                            }
                            else if (other_game_object->GetType() != current_game_object->getFrom()) {
                                if (current_game_object->RayToCircleCheck(other_game_object->GetPosition(), current_game_object->GetCircle()->get_r(), delta_time)) {//ray to circle
                                    //get enemy position, enemy circle radius, and the delta_time
                                    current_game_object->Get_Collision_Pro(delta_time, other_game_object->GetType(), other_game_object->getFrom());
                                    other_game_object->Get_Collision_Pro(delta_time, current_game_object->GetType(), current_game_object->getFrom());
                                    // Play the sound when collision
                                    if (!am.SoundIsPlaying(explosion_music_index)) {//if the sound is already playing, don't play it twice
                                        am.PlaySound(explosion_music_index);
                                    }
                                    else {//stop it first then replay it
                                        am.StopSound(explosion_music_index);
                                        am.PlaySound(explosion_music_index);
                                    }
                                }
                            }
                            break;
                        case 52:
                            if (other_game_object->GetType() != current_game_object->getFrom()) {
                                if (current_game_object->Ract_Circle_Collition(other_game_object->GetPosition(), other_game_object->GetCircle()->get_r(), delta_time)) {
                                    //get enemy position, enemy circle radius, and the delta_time
                                  //  std::cout << "Current ID: " << current_game_object->GetType() << " Attack: " << other_game_object->GetType() << std::endl;
                                    other_game_object->Get_Collision_Pro(delta_time, current_game_object->GetType(), current_game_object->getFrom());
                                    // Play the sound when collision
                                    if (!am.SoundIsPlaying(explosion_music_index)) {//if the sound is already playing, don't play it twice
                                        am.PlaySound(explosion_music_index);
                                    }
                                    else {//stop it first then replay it
                                        am.StopSound(explosion_music_index);
                                        am.PlaySound(explosion_music_index);
                                    }
                                }
                            }
                            else if ((other_game_object->GetType() > 80) && (other_game_object->GetType() < 90)) {
                                break;
                            }
                            break;
                        case 53:
                            if ((other_game_object->GetType() < 20) && (other_game_object->GetType() > 10)) {

                            }
                            else if (other_game_object->GetType() == 41) {
                                if (other_game_object->Ract_Circle_Collition(current_game_object->GetPosition(), current_game_object->GetCircle()->get_r(), delta_time)) {
                                   // std::cout << " attack wall!! " << std::endl;;
                                    current_game_object->Get_Collision_Pro(delta_time, other_game_object->GetType(), other_game_object->getFrom());
                                    other_game_object->Get_Collision_Pro(delta_time, current_game_object->GetType(), current_game_object->getFrom());
                                    if (!am.SoundIsPlaying(explosion_music_index)) {//if the sound is already playing, don't play it twice
                                        am.PlaySound(explosion_music_index);
                                    }
                                    else {//stop it first then replay it
                                        am.StopSound(explosion_music_index);
                                        am.PlaySound(explosion_music_index);
                                    }
                                }
                            }
                            else if (other_game_object->GetType() != current_game_object->getFrom()) {
                                if (current_game_object->RayToCircleCheck(other_game_object->GetPosition(), current_game_object->GetCircle()->get_r(), delta_time)) {//ray to circle
                                    //get enemy position, enemy circle radius, and the delta_time
                                    current_game_object->Get_Collision_Pro(delta_time, other_game_object->GetType(), other_game_object->getFrom());
                                    other_game_object->Get_Collision_Pro(delta_time, current_game_object->GetType(), current_game_object->getFrom());
                                    // Play the sound when collision
                                    if (!am.SoundIsPlaying(explosion_music_index)) {//if the sound is already playing, don't play it twice
                                        am.PlaySound(explosion_music_index);
                                    }
                                    else {//stop it first then replay it
                                        am.StopSound(explosion_music_index);
                                        am.PlaySound(explosion_music_index);
                                    }
                                }
                            }
                            else if ((other_game_object->GetType() > 80) && (other_game_object->GetType() < 90)) {
                                break;
                            }
                            break;
                        case 80:
                            break;
                        case 91:
                        case 92:
                        case 93:
                            if ((other_game_object->GetType() == 94) && (current_game_object->GetType() == 93)) {
                                if (current_game_object->getBack()) {
                                    if (distance < (current_game_object->GetCircle()->get_r() + other_game_object->GetCircle()->get_r())) {
                                        current_game_object->SetAlive(false);
                                    }

                                }
                            }
                        case 94:
                        case 95:
                        case 100:
                            if (other_game_object->GetType() == 52) {
                                if (current_game_object->GetType() != other_game_object->getFrom()) {
                                    if (other_game_object->Ract_Circle_Collition(current_game_object->GetPosition(), current_game_object->GetCircle()->get_r(), delta_time)) {
                                        //get enemy position, enemy circle radius, and the delta_time
                                        std::cout << "Current ID: " << current_game_object->GetType() << " Attack: " << other_game_object->GetType() << std::endl;
                                        current_game_object->Get_Collision_Pro(delta_time, other_game_object->GetType(), other_game_object->getFrom());
                                        // Play the sound when collision
                                        if (!am.SoundIsPlaying(explosion_music_index)) {//if the sound is already playing, don't play it twice
                                            am.PlaySound(explosion_music_index);
                                        }
                                        else {//stop it first then replay it
                                            am.StopSound(explosion_music_index);
                                            am.PlaySound(explosion_music_index);
                                        }
                                    }
                                }
                            }
                            break;

                        default:
                            break;
                        }


                    }

                }


            }
        }
        //check all object texture
        if (game_objects_[0]->GetState() == 1) {//if it is in invincible state
            game_objects_[0]->SetTexture(tex_[6]);//change texture
        }
        else {
            game_objects_[0]->SetTexture(tex_[1]);//set it normal
        }


        //delete all die object
        for (int i = 0;i < (game_objects_.size() - 4);i++) {
            GameObject* current_game_object = game_objects_[i];
            if ((!current_game_object->GetAlive())) {// (!current_game_object->GetAlive()) && (!current_game_object->GetCollectible())
                //we now cannot delete the collection
                if (i == 0) {//player die
                    map_ = false;
                    delete game_objects_[i];//delete player
                    game_objects_.erase(game_objects_.begin());//shrink the vector
                    std::cout << "Game Over!" << std::endl;//print gameover
                    glfwSetWindowShouldClose(window_, GLFW_TRUE);//set the window, it need to be shutdown
                    return;//jump out of the function
                }
                else if(changing_level_){
                    delete game_objects_[i];//delete the obj
                    game_objects_.erase(game_objects_.begin() + i);//shrink vector
                    continue;
                }
                else if ((current_game_object->GetType() > 10) && (current_game_object->GetType() < 20)) {
                    if (!current_game_object->GetCollectible()) {
                        delete game_objects_[i];//delete the obj
                        game_objects_.erase(game_objects_.begin() + i);//shrink vector
                        //  std::cout << "item delete!" << std::endl;//print delete
                        continue;
                    }
                }
                else if ((current_game_object->GetType() > 90) && (current_game_object->GetType() < 100)) {
                    switch (current_game_object->GetType())
                    {
                    case 91:
                        player->Add_Exp(Minion.exp_);
                        break;
                    case 92:
                        player->Add_Exp(EX_Minion.exp_);
                        break;
                    case 93:
                        if (current_game_object->getBack() == false) {
                            player->Add_Exp(BBB.exp_);
                        }

                        break;
                    case 94:
                        player->Add_Exp(Fortress.exp_);
                        break;
                    case 95:
                        player->Add_Exp(Monster.exp_);
                        break;
                    default:
                        break;
                    }
                    int type_enemy = current_game_object->GetType();
                    glm::vec3 posi = current_game_object->GetPosition();
                    GameObject* expo = new Expo_obj(posi, sprite_, &animate_shader_, tex_[3]);
                    expo->SetNumFrame(glm::vec2(8, 1));

                    

                    if (!current_game_object->getBack()) {
                        delete game_objects_[i];
                        game_objects_.erase(game_objects_.begin() + i);//shrink vector
                        game_objects_.insert(game_objects_.begin() + 1, expo);

                        generateEnemyDrops(posi, type_enemy);
                    }
                    else {
                        delete game_objects_[i];
                        game_objects_.erase(game_objects_.begin() + i);//shrink vector
                        game_objects_.insert(game_objects_.begin() + 1, expo);
                    }


                }
                else {//otherthing die

                    delete game_objects_[i];//delete the obj
                    game_objects_.erase(game_objects_.begin() + i);//shrink vector
                    std::cout << "delete!" << std::endl;//print delete
                    continue;
                }
            }
            /*
            else if (current_game_object->getBack()) {
                current_game_object->getFortress()->become_angry();
                delete game_objects_[i];//delete the obj
                game_objects_.erase(game_objects_.begin() + i);//shrink vector
                std::cout << "delete!" << std::endl;//print delete

                continue;
            }
    */

        }

        //new enemy
        if (timer.Finished()) {//if the timer is finished, then span new enemy  
            //std::cout <<"Create new !" << std::endl;
            if (!changing_level_) {
                generateDifferentEnemy();
                timer.Start(10.0);//start the timer again
            }
        }
        //enemy shoot
        for (int i = 0;i < (game_objects_.size() - 4);i++) {
            GameObject* current_game_object = game_objects_[i];
            if (current_game_object->GetType() > 90) {
                switch (current_game_object->GetType())
                {
                case 91:
                case 92:
                    if (current_game_object->getShoot()) {
                        game_objects_.insert(game_objects_.begin() + 1, new Projectile(current_game_object->GetPosition(), sprite_, &sprite_shader_, tex_[7], Circle(), current_game_object->GetRotation(), current_game_object->GetType(), 51));
                        game_objects_[1]->SetScale(glm::vec2(0.5, 0.2));
                        game_objects_[1]->GetCircle()->SetRadius(game_objects_[1]->GetScale().x / 2);//set the circle radius
                        current_game_object->setWant(false);
                    }
                    break;
                case 94:
                    if (current_game_object->getShoot() && current_game_object->GetState() == 9) {
                        glm::vec3 po = glm::vec3(current_game_object->getShooter().x, current_game_object->getShooter().y, current_game_object->getShooter().z);
                        float an = current_game_object->getShooter().w;
                        game_objects_.insert(game_objects_.begin() + 1, new Projectile(po, sprite_, &sprite_shader_, tex_[7], Circle(), an, current_game_object->GetType(), 51));
                        game_objects_[1]->SetScale(glm::vec2(1, 0.2));
                        game_objects_[1]->GetCircle()->SetRadius(game_objects_[1]->GetScale().x / 2);//set the circle radius
                        current_game_object->setWant(false);
                    }
                default:
                    break;
                }
            }
        }

        //blue back
        for (int i = 0;i < (game_objects_.size() - 4);i++) {
            GameObject* current_game_object = game_objects_[i];
            if (current_game_object->GetAlive()) {
                if (current_game_object->GetType() == 93) {
                    if (current_game_object->GetState() == 6) {
                        current_game_object->back();
                    }
                }
            }
        }

    
    
    
  

    Update_HUD(delta_time);
    shop_.Update(window_, 0.25f);

}

void Game::Update_HUD(double delta_time) {
    enemy_objects_.clear();
    for (int i = 1;i < (game_objects_.size() - 4);i++) {
        GameObject* current_game_object = game_objects_[i];
        if (current_game_object->GetType() > 90) {
            enemy_objects_.push_back(current_game_object);
        }
        if (enemy_objects_.size() >= 10) {
            break;
        }

    }
    int width, height;
    int barO = game_objects_.size() - 4;
    glfwGetWindowSize(window_, &width, &height);
    game_objects_[barO]->SetWindowHeight(height);
    game_objects_[barO]->SetWindowWidth(width);
    game_objects_[barO + 1]->SetWindowHeight(height);
    game_objects_[barO + 1]->SetWindowWidth(width);

    minimap_->SetAllChild(enemy_objects_);
    game_objects_[barO]->Update(delta_time);
    game_objects_[barO + 1]->Update(delta_time);
    Update_HUD_Value();
}


void Game::generateDifferentEnemy() {
   
   // if (level_ != 1) { return; }
    if (changing_level_) { 
        
        GameObject* new_enemy;
        glm::vec3 random_position = generateRandomPosition();//random position in the window
        float pi_over_two = glm::pi<float>() / 2.0f;
        new_enemy = new MonsterObject(random_position, sprite_, &number_shader_, tex_[18], Circle(), current_time_, 0, 95);
        new_enemy->GetCircle()->SetRadius(new_enemy->GetScale().x / 2);//set the circle radius
        new_enemy->SetPlayer(game_objects_[0]); // set player pointer
        new_enemy->SetRotation(pi_over_two);
        game_objects_.insert(game_objects_.begin() + 1, new_enemy);
        return; 
    }
    if (level_ == 2) { return; }
    if (game_objects_.size() > 100) { return; }
    glm::vec3 random_position = generateRandomPosition();//random position in the window
    int r1 = rand() % 100;//
    int r2 = rand() % 100;
    GameObject* new_enemy;
    float pi_over_two = glm::pi<float>() / 2.0f;
    if (r1<=70) {
        if ((r2+(int)current_time_) <= 70) {
            new_enemy = new EnemyGameObject(random_position, sprite_, &sprite_shader_, tex_[0], Circle(), current_time_, 0, 91);
        }
        else {
            new_enemy = new EnemyGameObject(random_position, sprite_, &sprite_shader_, tex_[14], Circle(), current_time_, 0, 92);
        }
        new_enemy->GetCircle()->SetRadius(new_enemy->GetScale().x / 2);//set the circle radius
        new_enemy->SetPlayer(game_objects_[0]); // set player pointer
        new_enemy->SetRotation(pi_over_two);
        game_objects_.insert(game_objects_.begin() + 1, new_enemy);//new enemy
    }
    else if(r1<=90) {
        new_enemy = new FortressObject(random_position, sprite_, &sprite_shader_, tex_[16], Circle(), current_time_, 0, 94);
        new_enemy->GetCircle()->SetRadius(new_enemy->GetScale().x *0.6);//set the circle radius
        new_enemy->SetPlayer(game_objects_[0]); // set player pointer
        new_enemy->SetRotation(pi_over_two);
       

        Generate_Fortress(new_enemy);

    }
    else {
        new_enemy = new MonsterObject(random_position, sprite_, &number_shader_, tex_[18], Circle(), current_time_, 0, 95);
        new_enemy->GetCircle()->SetRadius(new_enemy->GetScale().x / 2);//set the circle radius
        new_enemy->SetPlayer(game_objects_[0]); // set player pointer
        new_enemy->SetRotation(pi_over_two);
        game_objects_.insert(game_objects_.begin() + 1, new_enemy);
    }



    //  std::cout << "Create new GameObject at position ("//print function
     //     << random_position.x << ", "
      //    << random_position.y << ")" << std::endl;

}

void Game::generateEnemyDrops(glm::vec3 position, int type) {

    switch (type)
    {
    case 91:
        generateEnemyDrops(position, Minion);
        break;
    case 92:
        generateEnemyDrops(position, EX_Minion);
         break;
    case 93:
        generateEnemyDrops(position, BBB);
         break;
    case 94:
        generateEnemyDrops(position, Fortress);
        break;
    case 95:
        generateEnemyDrops(position, Monster);
         break;
    default:
        break;
    }
}

void Game::generateEnemyDrops(glm::vec3 position, EnemyDrop s) {
    int random = rand() % 100;
    for (int i = 0; i < s.ip_;i++) {
        if (random > i * s.percent_) {
            game_objects_.insert(game_objects_.begin() + 1, new CollectibleGameObject(glm::vec3(position.x + 0.2, position.y, position.z), sprite_, &number_shader_, tex_[15], Circle(), 11));
        }
    }
    for (int i = 0; i < s.energy_;i++) {
        if (random > i * s.percent_) {
            game_objects_.insert(game_objects_.begin() + 1, new CollectibleGameObject(glm::vec3(position.x - 0.2, position.y, position.z), sprite_, &number_shader_, tex_[15], Circle(), 12));
        }
    }
    for (int i = 0; i < s.iron_;i++) {
        if (random > i * s.percent_) {
            game_objects_.insert(game_objects_.begin() + 1, new CollectibleGameObject(glm::vec3(position.x, position.y + 0.1, position.z), sprite_, &number_shader_, tex_[15], Circle(), 13));
        }
    }
    for (int i = 0; i < s.coin_;i++) {
        if (random > i * s.percent_) {
            game_objects_.insert(game_objects_.begin() + 1, new CollectibleGameObject(glm::vec3(position.x, position.y + 0.1, position.z), sprite_, &number_shader_, tex_[15], Circle(), 14));
        }
    }
}
float  Game::randRange(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}
glm::vec3 Game::generateRandomPosition() {
    float x, y;
    int edge = rand() % 4; // 0=l, 1=r, 2=u, 3=d
    float x_p = game_objects_[0]->GetPosition().x;
    float y_p = game_objects_[0]->GetPosition().y;

    switch (edge) {
        case 0: // left
            x = randRange(-10, -5);
            y = randRange(-4, 4);
            x += x_p;
            y += y_p;
            break;
        case 1: // right
            x = randRange(5, 10);
            y = randRange(-4, 4);
            x += x_p;
            y += y_p;
            break;
        case 2: // up
            x = randRange(-5, 5);
            y = randRange(4, 10);
            x += x_p;
            y += y_p;
            break;
        case 3: // down
            x = randRange(-5, 5);
            y = randRange(-10, -4);
            x += x_p;
            y += y_p;
            break;
    }

    return glm::vec3(x, y, 0.0f);//return the position
}

void Game::Generate_Fortress(GameObject* f) {
    GameObject* new_enemy = f;

    GameObject* fortress_shooter_ = new FortressShooter(glm::vec3(-0.9f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[19], new_enemy);
    new_enemy->SetShooter(fortress_shooter_);
    game_objects_.insert(game_objects_.begin() + 1, new_enemy);//new enemy
    game_objects_.insert(game_objects_.begin() + 1, fortress_shooter_);

    GameObject* arm1 = new Arm1(glm::vec3(1.0f, 1.0f, -0.0f), sprite_, &sprite_shader_, tex_[24], new_enemy);
    arm1->SetScale(glm::vec2(1, 0.375));
    GameObject* arm2 = new Arm1(glm::vec3(1.0f, 0.0f, -0.0f), sprite_, &sprite_shader_, tex_[25], arm1);
    GameObject* arm3 = new Arm1(glm::vec3(1.0f, 0.0f, -0.0f), sprite_, &sprite_shader_, tex_[26], arm2);
    arm2->SetScale(glm::vec2(1, 0.375));
    arm3->SetScale(glm::vec2(0.5, 0.5));
    new_enemy->SetArm(arm3);
    new_enemy->SetChild1(arm1);
    new_enemy->SetChild2(arm2);
    game_objects_.insert(game_objects_.begin() + 1, arm3);
    game_objects_.insert(game_objects_.begin() + 1, arm2);
    game_objects_.insert(game_objects_.begin() + 1, arm1);
   
    
    GameObject* new_small_enemy;
    //    std::cout << "Create new Fortress at position ("//print function
    //<< random_position.x << ", "
    //<< random_position.y << ")" << std::endl;
    glm::vec3 random_position;
    for (int i = 0;i < 4; i++) {
        random_position = generateRandomPosition();
        new_small_enemy = new BlueGameObject(random_position, sprite_, &sprite_shader_, tex_[17], Circle(), current_time_, 0, 93);
        new_small_enemy->GetCircle()->SetRadius(new_small_enemy->GetScale().x / 2);//set the circle radius
        new_small_enemy->SetPlayer(game_objects_[0]); // set player pointer
        new_small_enemy->setFortress(new_enemy);
        game_objects_.insert(game_objects_.begin() + 1, new_small_enemy);
        // std::cout << "Create new BBB at position ("//print function
          //   << random_position.x << ", "
            // << random_position.y << ")" << std::endl;
    }

}
void Game::RenderMiniMap() {
    int width, height;
    float zoom_scale = 0.25f / map_zoom_;
    glfwGetWindowSize(window_, &width, &height);
    width /= zoom_scale;
    height /= zoom_scale;
    glm::mat4 window_scale_matrix;
    if (width > height) {
        float aspect_ratio = ((float)width) / ((float)height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / aspect_ratio, 1.0f, 1.0f));
    }
    else {
        float aspect_ratio = ((float)height) / ((float)width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f / aspect_ratio, 1.0f));
    }

    glm::vec3 player_position = game_objects_[0]->GetPosition();//get player position
   
    glm::mat4 camera_trans = glm::translate(glm::mat4(1.0f), -game_objects_[0]->GetPosition());//calculate the matrix
    float center_x = (game_objects_[game_objects_.size() - 3]->GetChild(0)->GetPosition().x + game_objects_[game_objects_.size() - 3]->GetChild(0)->GetScale().x / 2);
    float center_y = (game_objects_[game_objects_.size() - 3]->GetChild(0)->GetPosition().y + game_objects_[game_objects_.size() - 3]->GetChild(0)->GetScale().y / 2);
    
    glm::vec3 scale = glm::vec3 (center_x* zoom_scale,
        center_y * zoom_scale, game_objects_[game_objects_.size() - 3]->GetChild(0)->GetPosition().z);
     camera_trans = glm::translate(camera_trans, scale);
   // camera_trans = glm::rotate(glm::mat4(1.0f),game_objects_[0]->GetRotation(), glm::vec3(0,0,1))* camera_trans;
    // Set view to zoom out, centered by default at 0,0
    //float camera_zoom = 0.05f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(map_zoom_, map_zoom_, map_zoom_));
    glm::mat4  view_matrix = window_scale_matrix * camera_zoom_matrix * camera_trans;//add the new transformatrion

   // game_objects_[game_objects_.size()-2]->GetChild(0)->RenderMap(view_matrix, current_time_);
  //  minimap_->SetZoom(map_zoom_);
    minimap_->SetSizeZoom(zoom_scale);
    minimap_->RenderMap(view_matrix, current_time_);

    for (int i = 0; i < game_objects_.size()-3; i++) {
      //  game_objects_[i]->Render(view_matrix, current_time_);
    }


}
void Game::Render(void){

    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix;
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        float aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    glm::vec3 player_position = game_objects_[0]->GetPosition();//get player position
    

    glm::vec3 camera_position = glm::vec3(player_position.x, player_position.y, 1.0f);//camera center is the player

  

    glm::mat4 camera_trans = glm::translate(glm::mat4(1.0f),-game_objects_[0]->GetPosition());//calculate the matrix
    //camera_trans = glm::rotate(glm::mat4(1.0f),game_objects_[0]->GetRotation(), glm::vec3(0,0,1))* camera_trans;
    // Set view to zoom out, centered by default at 0,0
    float camera_zoom = 0.25f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));
    glm::mat4  view_matrix = window_scale_matrix * camera_zoom_matrix * camera_trans;//add the new transformatrion



    shop_.Render(window_scale_matrix * camera_zoom_matrix, current_time_);
    if (map_) {
        RenderMiniMap();
        game_objects_[game_objects_.size() - 3]->Render(view_matrix, current_time_);
    }
    // Render all game objects
    
    for (int i = 0; i < game_objects_.size(); i++) {
        if (i == game_objects_.size() - 3) { continue; }
       // if (i == game_objects_.size() - 1) {
            //maze_.Render(view_matrix, current_time_);
      //  }
        game_objects_[i]->Render(view_matrix, current_time_);
    }


}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        HandleControls(delta_time);

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();
    bg = new Background();
    bg->CreateGeometry();
    // Initialize particle geometry
    Particles* particles_temp = new Particles();
    particles_temp->CreateGeometry(4000); // Use 4000 particles
    particles_ = particles_temp;
    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/sprite_fragment_shader.glsl")).c_str());
    number_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/number_fragement_shader.glsl")).c_str());
    bar_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/bar_fragement_shader.glsl")).c_str());
    animate_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/animate_fragment_shader.glsl")).c_str());
    text_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/text_fragment_shader.glsl")).c_str());
    drawing_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/drawing_fragment_shader.glsl")).c_str());
    // Initialize particle shader
    particle_shader_.Init((resources_directory_g + std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/particle_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;
    map_zoom_ = 0.04;
    //Initialize Timer
    timer = Timer();
    shooter_timer = Timer();
    level_1_timer = Timer();
    stop_ = false;
    map_ = true;
    changing_level_ = false;
    maze_setup = false;
    level_ = 1;
    changing_timer = Timer();
    load_timer = Timer();
    //Initialize audio manager
    am.Init(NULL);
    
    //give a special random seed for each init world
    srand(time(NULL));




}


Game::~Game()
{
    // Free memory used by game world
    DestroyGameWorld();

    // Free rendering resources
    delete sprite_;
    delete particles_;
//    delete hud_;
    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::SetTexture(GLuint w, const char *fname,int type)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    SOIL_free_image_data(image);

    // Texture Wrapping
    if(type==2||type==31){//if it is background
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//change it to repeat for background
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//else not change
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


void Game::LoadTextures(std::vector<std::string> &textures)
{
    // Allocate a buffer for all texture references
    int num_textures = textures.size();
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+textures[i]).c_str(),i);
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}

} // namespace game
