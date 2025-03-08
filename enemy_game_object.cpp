#include "player_game_object.h"
#include "enemy_game_object.h"
#include <iostream>
namespace game {
    EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, int hp, Circle circle,double t,int statue) 
        : GameObject(position, geom, shader, texture) {
            hitpoint = hp;
            circle = circle;
            alive = true;
            colliable = true;
            t_ = t;
            timer_react = Timer();
            speed = 2;//set unit of speed =2
            if ((statue % 5)!=0) {//random choose the starting moving mode
                Init(2);//each 50% 
            }
            else {
                Init(3);
            }



    }
    Circle* EnemyGameObject::GetCircle() {
        return &circle;
    }
    //getters
    bool EnemyGameObject::GetAlive() {
        return alive;
    }
    bool EnemyGameObject::GetColliable() {
        return colliable;
    }
    int EnemyGameObject::GetHP() {
        return hitpoint;
    }
    int EnemyGameObject::GetState() {
        return statue;
    }
    float EnemyGameObject::GetSpeed() {
        return speed;
    }
    //setters
    void EnemyGameObject::SetColliable(bool a) {
        colliable = a;
    }
    void EnemyGameObject::SetAlive(bool a) {
        alive = a;
    }
    void EnemyGameObject::SetStatue(int a) {
        statue = a;
    }
    void EnemyGameObject::SetPlayer(GameObject* a) {
        player_ = a;
    }
    void EnemyGameObject::SetCenter(glm::vec3& a) {
        center_ = a;
    }
    void EnemyGameObject::SetWidth(float a) {
        width = a;
    }
    void EnemyGameObject::SetHeight(float a) {
        height=a;
    }

    void EnemyGameObject::Init(int s) {

        if (s == 2) {
            statue = 2;//set the statue to patrolling
            type = 3;
            center_ = position_;//the center is the given position
            width = 1;
            height = 2;
            glm::vec2 P = glm::vec2(center_);
            glm::vec2 D = P + glm::vec2(
                (this->width / 2.0f) * cos(speed * t_),
                (this->height / 2.0f) * sin(speed * t_));
            //calculate the position on the ellipse with given width, height and center
            glm::vec3 position = glm::vec3(D.x, D.y, 0);
            position_ = position;//set the new position as the starting position
        }
        else {
            statue = 3;//just set the statue to intercepting
            type = 3;
            center_ = position_;
            width = 0.5;
            height = 0.5;

        }
    }
    void EnemyGameObject::Get_Collision() {//when collision, hitpoint --, if hitpoint to 0, explosion
        //std::cout << " Get Collision" << std::endl;

        if (hitpoint > 0) {// get collision, hitpoint - 1
            hitpoint--;
        }
        
    
    }
    void EnemyGameObject::Explosion() {//change texture and count 5 s
        timer_exp.Start(5.0);//set the timer. the obj still need to be exist in 5s
        colliable = false;// when it's explosing, it's not colliable.
    }

    void EnemyGameObject::patrolling() {
        glm::vec2 D = glm::vec2(
            -(this->width / 2.0f) * sin(speed*t_),
            (this->height / 2.0f) * cos(speed*t_));
        //calculate current velocity and adjust it by unit of speed
        
        SetVelocity(glm::vec3(speed*D,0));//set velocity and adjust it by unit of speed
    }
    bool EnemyGameObject::findPlayer(){
        glm::vec2 P, player;//get current position and the player position
        float distance;
        if (player_ != NULL) {
            player = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
            P = glm::vec2(position_.x, position_.y);
            distance = glm::length(player - P);//calculate the distance between the two obj
            if (distance < 2*(player_->GetCircle()->get_r()+this->GetCircle()->get_r())) {
                //if (player's circle radius+ this's circle radius)*2 > distance,then it is close enough for this obj see player
                return true;
            }
        }
        return false;
    }

    void EnemyGameObject::intercepting(double delta_time){
        glm::vec2 Myposition, Target, Player_position;
        glm::vec2 direction,velocity;        
        
        if (timer_react.Finished()) {//adjust velocity every 2s 
            Myposition = glm::vec2(GetPosition().x, GetPosition().y);
            Player_position = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
            velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
            velocity = speed * (Player_position - Myposition) / glm::length(Player_position - Myposition);
            //calculate the velocity and adjust it with unit of speed
            /*
            glm::vec2 a;
            a = Player_position - Myposition - velocity;
            velocity = velocity - a *(float)delta_time;
            if (glm::length(velocity) > speed) {
                velocity = speed * velocity / glm::length(velocity);
            }
            */
            SetVelocity(glm::vec3(velocity, 0));//set new velocity
           timer_react.Start(2);//restart the timer
        }

    }
    void EnemyGameObject::SetVelocity(const glm::vec3& velocity) {
        velocity_ = velocity;
        float dp = glm::dot(velocity_,glm::vec3(1,0,0));//dot product with velocity and x axis
        float dp2= glm::dot(velocity_, glm::vec3(0,1,0));//dot product with velocity and y axis
        dp /= glm::length(velocity_);//normalized
        dp2/= glm::length(velocity_);
        float angle = acos(dp);//calculate the angle, can only show 0~180
        if (dp2 >= 0) {//if it is in x>=0 
            SetRotation(angle);
        }
        else {//it is in x<=0
            SetRotation(-angle);//adjust the angle
        }
        //std::cout << "Update velocity"<< velocity_.x << std::endl;
    }
    // Update function for moving the player object around
    // Update status
    void EnemyGameObject::Update(double delta_time) {
        
        
        if (this->GetHP() < 1) {//same as player
            if (this->GetAlive() && this->GetColliable()) {
                Explosion();
            }
            else {
                if (timer_exp.Finished()) {
                    this->SetAlive(false);
                    return;
                }
            }
        }

        else {//if not died
            switch (this->GetState())//state changing
            {
            case 2://change velocity by patrolling
                patrolling();
                if (findPlayer()) {//if it see player, change state to intercepting
                    statue = 3;
                }
                break;
            case 3:
                intercepting(delta_time);//change velocity by intercepting
                break;

            default:
                break;
            }

            glm::vec2 Myposition, velocity, T;
            velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
            Myposition = glm::vec2(position_.x, position_.y);
            T = Myposition + (float)delta_time * velocity;//calculate the new position by new velocity
            SetPosition(glm::vec3(T.x, T.y, 0));//set the new position
        }



            t_ += delta_time;//update the t_

    }

}// namespace game