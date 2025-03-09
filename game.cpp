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
#include "expo_obj.h"
#include "enemy_game_object.h"
#include "projectile.h"
#include "game.h"


namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Assignment 3"; //change name to assignment 3
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;

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
           tex_explo =3,
            item= 4,//new texture for item
            tex_orb =5,
            in_player=6,//new texture for invincible player
            projectile=7,//new texture for shooter
            healthy_bar=8,
            number=9,
            exp_bar =10,
            energy_bar=11,
            level_text=12,
            exp_text=13
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

    // Load textures
    LoadTextures(textures);

    //setup timer for new enemy
    timer.Start(10.0);

    // **** Setup all the game objects in the world

    // Setup the player object (position, texture, vertex count,hp,circle)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 

    //Setup HUD stuff
    GameObject* hp_spirt = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &bar_shader_, tex_[healthy_bar]);
    GameObject* energy_spirt = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &bar_shader_, tex_[energy_bar]);
    GameObject* exp_spirt = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &bar_shader_, tex_[exp_bar]);
    GameObject* num_spirt = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &number_shader_, tex_[number]);
    GameObject* exp_text_s = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[exp_text]);
    GameObject* lv_text_s = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[level_text]);



    hud_ = new HUD(hp_spirt,num_spirt, energy_spirt,exp_spirt, exp_text_s, lv_text_s);
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tiny_ship17],3,Circle()));//change texture, add hitpoint, circle

    float pi_over_two = glm::pi<float>() / 2.0f;
    hud_->SetHP(game_objects_[0]->GetHP());
    hud_->SetMax_HP(game_objects_[0]->GetHP());
    hud_->SetEnergy(30);
    hud_->SetMax_Energy(100);
    hud_->SetExp(80);
    hud_->SetMax_Exp(100);
    // Setup enemy objects
    game_objects_.push_back(new EnemyGameObject(glm::vec3(2.0f, -2.0f, 0.0f), sprite_, &sprite_shader_, tex_[tiny_ship1],1, Circle(),0,rand()%100));//change texture,add hitpoint, circle, random number for different moving mode
    game_objects_.push_back(new EnemyGameObject(glm::vec3(1.0f, 2.5f, 0.0f), sprite_, &sprite_shader_, tex_[tiny_ship1],1, Circle(),0.0f, rand() % 100));//change texture£¬add hitpoint, circle
    game_objects_[2]->SetScale(glm::vec2(0.5,0.8));//set the scale



    //setup item object

    

    //set circle radius for each
    for (int i = 0;i < game_objects_.size();i++) {
        game_objects_[i]->GetCircle()->SetRadius(game_objects_[i]->GetScale().x / 2);//set the circle radius as scale/2
        game_objects_[i]->SetRotation(pi_over_two);
        game_objects_[i]->SetPlayer(game_objects_[0]);//set player pointer for each enemy
    }

    // Setup background
    // In this specific implementation, the background is always the
    // last object
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), bg, &sprite_shader_, tex_[tex_stars]);
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
        player->SetPosition(curpos - motion_increment*player->GetRight());
    }
    if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
        player->SetPosition(curpos + motion_increment*player->GetRight());
    }
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    if (glfwGetKey(window_, GLFW_KEY_J) == GLFW_PRESS) {//press J to shoot
        if(shooter_timer.Finished()){//cooldown 
            game_objects_.insert(game_objects_.begin() + 1, new Projectile(player->GetPosition(), sprite_, &sprite_shader_, tex_[7], Circle(), angle));
            //the projectile is start at the player current position, and faster velocity with same direction as player
            game_objects_[1]->GetCircle()->SetRadius(game_objects_[1]->GetScale().x / 2);//set the circle radius
            shooter_timer.Start(0.5f);//cooldown
        }
    }
}


void Game::Update(double delta_time)
{
    current_time_ += delta_time;

    // Update all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // Update the current game object
        current_game_object->Update(delta_time);

        // Check for collision with other game objects
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world
        for (int j = i + 1; j < (game_objects_.size()-1); j++) {
            GameObject* other_game_object = game_objects_[j];

            // Compute distance between object i and object j
            float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());
            // If distance is below a threshold, we have a collision
            if (other_game_object->GetColliable() && current_game_object->GetColliable()) {//if the two obj is colliable, then continue.
                switch (current_game_object->GetType())//change if else to switch, make it more clear
                {//player=1; colliable=2, enemy=3; projectile=4,defult=0
                case 1://current type is player. player can colliable with collectible, enemy
                    if (distance < 0.8f * (current_game_object->GetCircle()->get_r() + other_game_object->GetCircle()->get_r())) {
                        if (other_game_object->GetType() == 3)//if it is enemy
                        {
                            //if the two obj has a distance smaller than 0.8 *(sum of the radius), then it there is a collisoin
                            // This is where you would perform collision response between objects
                            other_game_object->Get_Collision();//current and other obj get collision.
                            current_game_object->Get_Collision();
                            // Play the sound when collision
                            if (!am.SoundIsPlaying(explosion_music_index)) {//if the sound is already playing, don't play it twice
                                am.PlaySound(explosion_music_index);
                            }else {//stop it first then replay it
                                am.StopSound(explosion_music_index);
                                am.PlaySound(explosion_music_index);
                            }
                        }
                        else if (other_game_object->GetType() == 2 ) {//if it is collect
                            other_game_object->Get_Collision();//item change color
                            current_game_object->CollectItem();//player item++
                        }
                    }

                    break;
                case 4://current type is projectile
                    if (other_game_object->GetType() == 3) {
                        if (current_game_object->RayToCircleCheck(other_game_object->GetPosition(), current_game_object->GetCircle()->get_r(), delta_time)) {//ray to circle
                            //get enemy position, enemy circle radius, and the delta_time
                            other_game_object->Get_Collision();//current and other obj get collision.
                            current_game_object->Get_Collision();
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
                default:
                    break;
                }


            }

        }
    }

    //check all object texture
    if (game_objects_[0]->GetState()==1) {//if it is in invincible state
        game_objects_[0]->SetTexture(tex_[6]);//change texture
    }
    else {
        game_objects_[0]->SetTexture(tex_[1]);//set it normal
    }


    //delete all die object
    for (int i = 0;i < (game_objects_.size()-1);i++) {
        GameObject* current_game_object = game_objects_[i];
        if ( (!current_game_object->GetAlive()) && (current_game_object->GetType()!=2)) {// (!current_game_object->GetAlive()) && (!current_game_object->GetCollectible())
            //we now cannot delete the collection
            if (i == 0) {//player die
                delete game_objects_[i];//delete player
                game_objects_.erase(game_objects_.begin());//shrink the vector
                std::cout << "Game Over!" << std::endl;//print gameover
                glfwSetWindowShouldClose(window_, GLFW_TRUE);//set the window, it need to be shutdown
                return;//jump out of the function
            }
            else if ((current_game_object->GetType() != 4) && (current_game_object->GetType() != -1)) {

                glm::vec3 posi = current_game_object->GetPosition();
                GameObject* expo = new Expo_obj(posi, sprite_, &animate_shader_, tex_[3]);
                expo->SetNumFrame(glm::vec2(8, 1));
                
                delete game_objects_[i];
                game_objects_.erase(game_objects_.begin() + i);//shrink vector
                game_objects_.insert(game_objects_.begin() + 1, expo);
                GameObject* c1 = new CollectibleGameObject(glm::vec3(posi.x+0.2,posi.y,posi.z), sprite_, &sprite_shader_, tex_[4], Circle());
                GameObject* c2 = new CollectibleGameObject(glm::vec3(posi.x - 0.2, posi.y, posi.z), sprite_, &sprite_shader_, tex_[4], Circle());
                GameObject* c3 = new CollectibleGameObject(glm::vec3(posi.x, posi.y+0.2, posi.z), sprite_, &sprite_shader_, tex_[4], Circle());
                GameObject* c4 = new CollectibleGameObject(glm::vec3(posi.x, posi.y-0.2, posi.z), sprite_, &sprite_shader_, tex_[4], Circle());
                c1->SetScale(glm::vec2(0.3, 0.3));
                c2->SetScale(glm::vec2(0.3, 0.3));
                c3->SetScale(glm::vec2(0.3, 0.3));
                c4->SetScale(glm::vec2(0.3, 0.3));
                game_objects_.insert(game_objects_.begin() + 1, c1);
                game_objects_.insert(game_objects_.begin() + 1, c2);
                game_objects_.insert(game_objects_.begin() + 1, c3);
                game_objects_.insert(game_objects_.begin() + 1, c4);

            }
            else {//otherthing die
                delete game_objects_[i];//delete the obj
                game_objects_.erase(game_objects_.begin() + i);//shrink vector
                std::cout << "delete!" << std::endl;//print delete
                continue;
            }
        }

    }

    if (timer.Finished()) {//if the timer is finished, then span new enemy  
        //std::cout <<"Create new !" << std::endl;
        glm::vec3 random_position= generateRandomPosition();//random position in the window

        game_objects_.insert(game_objects_.begin()+ 1, new EnemyGameObject(random_position, sprite_, &sprite_shader_, tex_[0], 1,Circle(),delta_time, rand() % 100));//new enemy
        //insert it just after the player in vector
        game_objects_[1]->GetCircle()->SetRadius(game_objects_[1]->GetScale().x / 2);//set the circle radius
        game_objects_[1]->SetPlayer(game_objects_[0]); // set player pointer
      //  std::cout << "Create new GameObject at position ("//print function
       //     << random_position.x << ", "
        //    << random_position.y << ")" << std::endl;
        timer.Start(10.0);//start the timer again
    }
        
    hud_->SetHP(game_objects_[0]->GetHP());
    hud_->Update(delta_time);
}

glm::vec3 Game::generateRandomPosition() {
    float x_num = (rand() % 100) / 100.0;//random number 0~1
    float y_num = (rand() % 100) / 100.0;//random number 0~1
    float X_value = x_num * 2 * 5 - 5.0;//calculate between -5~5
    float Y_value = y_num * 2 * 4 - 4.0;//calculate between -4~4


    return glm::vec3(X_value, Y_value, 0.0f);//return the position
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
    // Set view to zoom out, centered by default at 0,0
    float camera_zoom = 0.25f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));
    glm::mat4  view_matrix = window_scale_matrix * camera_zoom_matrix * camera_trans;//add the new transformatrion
    

    glm::mat4 HUD_matrix = window_scale_matrix * camera_zoom_matrix;
    hud_->Render(HUD_matrix, current_time_);
    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
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
    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/sprite_fragment_shader.glsl")).c_str());
    number_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/number_fragement_shader.glsl")).c_str());
    bar_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/bar_fragement_shader.glsl")).c_str());
    animate_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/animate_fragment_shader.glsl")).c_str());
    // Initialize time
    current_time_ = 0.0;

    //Initialize Timer
    timer = Timer();
    shooter_timer = Timer();
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
    delete hud_;
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
    if(type==2){//if it is background
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
