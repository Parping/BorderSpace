#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "game_object.h"

namespace game {


GameObject::GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture) 
{

    // Initialize all attributes
    position_ = position;
    scale_ = glm::vec2(1.0,1.0);
    angle_ = 0.0;
    geometry_ = geom;
    shader_ = shader;
    texture_ = texture;
    timer_exp = Timer();//timer for the obj explosion
    //the timer can be used in not-spaceship obj for other things, like prop, so I keep it in the GameObject
    type = 0;
    velocity_ = glm::vec3(0,0,0);
    ghost = false;
    num_frame = glm::vec2(1,1);
    current_frame = 0;
    bar_percent = 1.0;
}

glm::vec3 GameObject::GetBearing(void) const {

    glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
    return dir;
}


glm::vec3 GameObject::GetRight(void) const {

    float pi_over_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(angle_ - pi_over_two), sin(angle_ - pi_over_two), 0.0);
    return dir;
}

//getter for the texture
GLuint  GameObject::getTexture() {
    return texture_;
}

//setters
void GameObject::SetRotation(float angle){ 

    // Set rotation angle of the game object
    // Make sure angle is in the range [0, 2*pi]
    float two_pi = 2.0f*glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0){
        angle += two_pi;
    }
    angle_ = angle;
}
void GameObject::SetTexture(GLuint new_texture) {
    //std::cout << "try set new texture" << std::endl;
    texture_ = new_texture;
}
void GameObject::SetGhost(bool a) {
    ghost = a;
}
void GameObject::SetBar_Percent(float b) {
   // std::cout << "set bar_percent: "<<b << std::endl;
    bar_percent = b;
}
int GameObject::GetBar_Percent() { 
   // std::cout << "bar_percent: " << bar_percent << std::endl;
    return bar_percent; }
void GameObject::Update(double delta_time) {

        
}

void GameObject::Render(glm::mat4 view_matrix, double current_time){

    // Set up the shader
    shader_->Enable();
    //Set up the ghost
    if (ghost) {
        shader_->SetUniform1i("ghost", 1);//if so, send 1 to ghost
    }
    else {
        shader_->SetUniform1i("ghost", 0);//if not, send 0 to ghost
    }
    shader_->SetUniform2f("frame", num_frame);
    shader_->SetUniform1i("current_frame",current_frame);
    shader_->SetUniform1f("percent", bar_percent);
    shader_->SetUniform1i("offset", offset);

    shader_->SetUniform1f("time", (float)current_time);

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));//change to .x and .y

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
