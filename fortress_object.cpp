#include "player_game_object.h"
#include "fortress_object.h"
#include <iostream>
namespace game {






    FortressObject::FortressObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle, double t, int statue, int ty)
        : GameObject(position, geom, shader, texture) {
        InitType(ty);
        circle = circle;
        alive = true;
        colliable = true;
        t_ = t;
        timer_react = Timer();
        want_shoot = false;
        target = glm::vec3(0, 0, 0);
        back_ = false;
        get_angry = Timer();
        isAngry = false;
        

    }

    void FortressObject::InitType(int t) {
        int random = rand() % 100;
        switch (t)
        {



        case 94://0 = normal, 8 = heal, 9 = attack
            hitpoint = fortress.hp_;
            type = fortress.type_;
            speed = fortress.speed_;
            scale_ = glm::vec2(fortress.size_scale_, fortress.size_scale_);
            react = fortress.react_;
            shoot_desire = fortress.shoot_desire_;
            reload = fortress.reload_;
            max_hp = hitpoint;
            statue = 0;
            break;


        default:
            break;
        }

    }

    void FortressObject::Init(int s) {


    }



    Circle* FortressObject::GetCircle() {
        return &circle;
    }
    //getters
    bool FortressObject::GetAlive() {
        return alive;
    }
    bool FortressObject::GetColliable() {
        return colliable;
    }
    int FortressObject::GetHP() {
        return hitpoint;
    }
    int FortressObject::GetState() {
        return statue;
    }
    float FortressObject::GetSpeed() {
        return speed;
    }
    //setters
    void FortressObject::SetColliable(bool a) {
        colliable = a;
    }
    void FortressObject::SetAlive(bool a) {
        alive = a;
        shooter->SetAlive(a);
        arms_->SetAlive(a);
        child_1->SetAlive(a);
        child_2->SetAlive(a);
    }
    void FortressObject::SetStatue(int a) {
        statue = a;
    }
    void FortressObject::SetPlayer(GameObject* a) {
        player_ = a;
    }
    void FortressObject::SetCenter(glm::vec3& a) {
        center_ = a;
    }
    void FortressObject::SetWidth(float a) {
        width = a;
    }
    void FortressObject::SetHeight(float a) {
        height = a;
    }

    void FortressObject::heal() {
        //
        arms_->setTarget(player_->GetPosition());
        glm::vec2 P1, P2;
        P1 = glm::vec2(arms_->GetPosition().x, arms_->GetPosition().y);
        P2 = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
        if (glm::distance( P1,P2) <0.4f) {
            player_->getHeal();
            std::cout << " heal" << std::endl;
        }

    }


    void FortressObject::Get_Collision_Pro(double delta_time, int pro_type, int attacker) {

        if ((attacker > 50) && (type > attacker)) { return; }

        if (collision_timer.Finished()) {
            
            if (hitpoint > 0) {// get collision, hitpoint - 1
                collision_timer.Start((float)delta_time);
                switch (pro_type)
                {
                case 51:
                    hitpoint -= pro_base.killer_point;
                    break;
                case 52:
                    hitpoint -= floor(pro_lazer.killer_point * 1.2);
                    break;
                case 53:
                    hitpoint -= floor(pro_bomb.killer_point);
                default:
                    break;
                }
                collision_timer.Start(delta_time);
                become_angry();
            }
        }
    }


    void FortressObject::Get_Collision(double delta_time) {//when collision, hitpoint --, if hitpoint to 0, explosion
        if (collision_timer.Finished()) {
            std::cout << " Get Collision" << std::endl;
            collision_timer.Start((float)delta_time);
            if (hitpoint > 0) {// get collision, hitpoint - 1
                hitpoint--;
            }
            collision_timer.Start(delta_time);
            become_angry();
        }
    }
    void FortressObject::Explosion() {//change texture and count 5 s

        colliable = false;// when it's explosing, it's not colliable.
        alive = false;
    }

    void FortressObject::patrolling() {
        glm::vec2 D = glm::vec2(
            -(this->width / 2.0f) * sin(speed * t_),
            (this->height / 2.0f) * cos(speed * t_));
        //calculate current velocity and adjust it by unit of speed

        SetVelocity(glm::vec3(speed * D, 0));//set velocity and adjust it by unit of speed
    }
    bool FortressObject::findPlayer() {
        glm::vec2 P, player;//get current position and the player position
        float distance;
        if (player_ != NULL) {
            player = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
            P = glm::vec2(position_.x, position_.y);
            distance = glm::length(player - P);//calculate the distance between the two obj
            if (distance < 2 * (player_->GetCircle()->get_r() + this->GetCircle()->get_r())) {
                //if (player's circle radius+ this's circle radius)*2 > distance,then it is close enough for this obj see player
                return true;
            }
        }
        return false;
    }

    void FortressObject::intercepting(double delta_time) {
        glm::vec2 Myposition, Target, Player_position;
        glm::vec2 direction, velocity;

        switch (type)
        {

        default:
            break;
        }


    }
    void FortressObject::SetVelocity(const glm::vec3& velocity) {
        velocity_ = velocity;
        float dp = glm::dot(velocity_, glm::vec3(1, 0, 0));//dot product with velocity and x axis
        float dp2 = glm::dot(velocity_, glm::vec3(0, 1, 0));//dot product with velocity and y axis
        dp /= glm::length(velocity_);//normalized
        dp2 /= glm::length(velocity_);
        float angle = glm::atan(velocity_.y, velocity_.x);//calculate the angle, can only show 0~180

        if (dp2 >= 0) {//if it is in x>=0 
            shooter->SetRotation(angle- angle_);
        }
        else {//it is in x<=0
            shooter->SetRotation(angle- angle_);//adjust the angle
        }
        //std::cout << "Update velocity"<< velocity_.x << std::endl;
    }
    void FortressObject::setWant(bool s) {
        want_shoot = s;
        // std::cout << "set shoot :" << s << std::endl;
    }
    bool FortressObject::getShoot() {
        // std::cout << "want shoot :" << want_shoot << std::endl;
        return want_shoot;
    }

    void FortressObject::become_angry() {
        get_angry.Start(10);
        isAngry = true;
        GetCircle()->SetRadius(GetScale().x / 4);
    }

    glm::vec4 FortressObject::getShooter() {
        glm::vec3 P, F;
        glm::vec4 temp;
        float A;
        A = angle_ +shooter->GetRotation() ;
       
       // temp = glm::vec4(shooter->GetPosition(), 1);
        //temp = GetTransformation() * temp;
        P = shooter->GetPosition();
        //F = position_ + glm::vec3(0,P.x,0);
        return glm::vec4(P, A);
    }
    // Update function for moving the player object around
    // Update status
    void FortressObject::Update(double delta_time) {
        glm::vec2 Myposition, velocity, T, Player,V;
        Player = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
        Myposition = glm::vec2(position_.x, position_.y);
        if (this->GetHP() < 1) {//same as player
            if (this->GetAlive() && this->GetColliable()) {
                this->SetAlive(false);
                //Explosion();
            }
          //  else {
          //      if (timer_exp.Finished()) {
           //         this->SetAlive(false);
          //          return;
            //    }
         //   }
        }

        else {//if not died
            switch (this->GetState())//state changing
            {
            case 0:
                if (isAngry) {
                    statue = 9;
                }
                if (glm::length(Player - Myposition) < (player_->GetCircle()->get_r() + GetCircle()->get_r())) {
                    statue = 8;
                    break;
                }
                break;
            case 8:
                if (isAngry) {
                    statue = 9;
                    arms_->setTarget(position_);
                    break;
                }
                if (glm::length(Player-Myposition) > (player_->GetCircle()->get_r()+GetCircle()->get_r())) {
                    statue = 0;
                    arms_->setTarget(position_);
                    break;
                }
                
                break;
            case 9:
               // std::cout << " angry" << std::endl;               
                if (get_angry.Finished()) {
                    isAngry = false;
                    statue = 0;
                    GetCircle()->SetRadius(GetScale().x * 0.6);
                   // std::cout << "not angry" << std::endl;
                    break;
                }
                
                break;
            default:
                break;
            }
            switch (this->GetState())
            {
            case 0:
                break;
            case 8:
                heal();
                break;
            case 9:
                V = (Player - Myposition) / glm::length(Player - Myposition);

                SetVelocity(glm::vec3(V,0));
                if (reload_timer.Finished()) {
                    if (!getShoot()) {
                        int random =  rand() % 100;
                        if (random < shoot_desire) {
                            setWant(true);
                        }
                        reload_timer.Start(reload);
                    }

                }
            default:
                break;
            }



        }




        t_ += delta_time;//update the t_

    }

    glm::mat4 FortressObject::GetTransformation() {


        // Setup the rotation matrix for the shader
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

        // Set up the translation matrix for the shader
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
        glm::mat4 transformation_matrix = translation_matrix * rotation_matrix;
        return transformation_matrix;
    }
    glm::mat4 FortressObject::GetLocalTransformation() {
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));
        glm::mat4 transformation_matrix = GetTransformation() * scaling_matrix;
        return transformation_matrix;
    }

}// namespace game