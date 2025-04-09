#include "player_game_object.h"
#include "blue_game_object.h"
#include <iostream>
namespace game {

    BlueGameObject::BlueGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle, double t, int statue, int ty)
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

    }

    void BlueGameObject::InitType(int t) {
        int random = rand() % 100;
        switch (t)
        {
        case 93://4 = run, 5 = collect, 6 = back
            hitpoint = bbb.hp_;
            type = bbb.type_;
            speed = bbb.speed_;
            scale_ = glm::vec2(bbb.size_scale_, bbb.size_scale_);
            react = bbb.react_;
            shoot_desire = bbb.shoot_desire_;
            reload = bbb.reload_;
            max_hp = hitpoint;
            Init(2);//easy implement

            break;

        default:
            break;
        }

    }

    void BlueGameObject::Init(int s) {

        if (s == 2) {
            statue = 2;//set the statue to patrolling
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
            center_ = position_;
            width = 0.5;
            height = 0.5;

        }
    }



    Circle* BlueGameObject::GetCircle() {
        return &circle;
    }
    //getters
    bool BlueGameObject::GetAlive() {
        return alive;
    }
    bool BlueGameObject::GetColliable() {
        return colliable;
    }
    int BlueGameObject::GetHP() {
        return hitpoint;
    }
    int BlueGameObject::GetState() {
        return statue;
    }
    float BlueGameObject::GetSpeed() {
        return speed;
    }
    //setters
    void BlueGameObject::SetColliable(bool a) {
        colliable = a;
    }
    void BlueGameObject::SetAlive(bool a) {
        alive = a;
    }
    void BlueGameObject::SetStatue(int a) {
        statue = a;
    }
    void BlueGameObject::SetPlayer(GameObject* a) {
        player_ = a;
    }
    void BlueGameObject::SetCenter(glm::vec3& a) {
        center_ = a;
    }
    void BlueGameObject::SetWidth(float a) {
        width = a;
    }
    void BlueGameObject::SetHeight(float a) {
        height = a;
    }

    void BlueGameObject::Get_Collision_Pro(double delta_time, int pro_type, int attacker) {
        if (attacker != 1) { return; }

        if (hitpoint > 0) {// get collision, hitpoint - 1
            if (collision_timer.Finished()) {
                switch (pro_type)
                {
                case 51:
                    hitpoint -= pro_base.killer_point;
                    break;
                case 52:
                    hitpoint -= floor(pro_lazer.killer_point * 1.2);
                    break;
                case 53:
                    hitpoint -= floor(pro_bomb.killer_point * 0.2);
                default:
                    break;
                }
                collision_timer.Start(delta_time);
            }
        }
    }

    void BlueGameObject::Get_Collision(double delta_time) {//when collision, hitpoint --, if hitpoint to 0, explosion
        if (collision_timer.Finished()) {
            //std::cout << " Get Collision" << std::endl;
            collision_timer.Start((float)delta_time);
            if (hitpoint > 0) {// get collision, hitpoint - 1
                hitpoint--;
            }
            collision_timer.Start(delta_time);
        }


    }
    void BlueGameObject::Explosion() {//change texture and count 5 s

        colliable = false;// when it's explosing, it's not colliable.
        alive = false;
    }

    void BlueGameObject::patrolling() {
        glm::vec2 D = glm::vec2(
            -(this->width / 2.0f) * sin(speed * t_),
            (this->height / 2.0f) * cos(speed * t_));
        //calculate current velocity and adjust it by unit of speed

        SetVelocity(glm::vec3(speed * D, 0));//set velocity and adjust it by unit of speed
    }
    bool BlueGameObject::findPlayer() {
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

    void BlueGameObject::intercepting(double delta_time) {
        glm::vec2 Myposition, Target, Player_position;
        glm::vec2 direction, velocity;

        switch (type)
        {
        case 91:
            if (timer_react.Finished()) {//adjust velocity every 2s 
                Myposition = glm::vec2(GetPosition().x, GetPosition().y);
                Player_position = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
                velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
                velocity = speed * (Player_position - Myposition) / glm::length(Player_position - Myposition);
                SetVelocity(glm::vec3(velocity, 0));//set new velocity
                timer_react.Start(react);//restart the timer
            }
            break;
        case 92:
            if (timer_react.Finished()) {//adjust velocity every 
                Myposition = glm::vec2(GetPosition().x, GetPosition().y);
                Player_position = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
                velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
                //calculate the velocity and adjust it with unit of speed
                velocity = speed * (Player_position - Myposition) / glm::length(Player_position - Myposition);
                glm::vec2 a;
                a = Player_position - Myposition - velocity;
                velocity = velocity + a * (float)delta_time;
                if (glm::length(velocity) > speed) {
                    velocity = speed * velocity / glm::length(velocity);
                }

                SetVelocity(glm::vec3(velocity, 0));//set new velocity
                if (glm::dot(Player_position, velocity) == 0) {
                    timer_react.Start(react);//restart the timer
                }
            }
            break;
        default:
            break;
        }


    }
    void BlueGameObject::SetVelocity(const glm::vec3& velocity) {
        velocity_ = velocity;
        float dp = glm::dot(velocity_, glm::vec3(1, 0, 0));//dot product with velocity and x axis
        float dp2 = glm::dot(velocity_, glm::vec3(0, 1, 0));//dot product with velocity and y axis
        dp /= glm::length(velocity_);//normalized
        dp2 /= glm::length(velocity_);
        float angle = acos(dp);//calculate the angle, can only show 0~180
        if (dp2 >= 0) {//if it is in x>=0 
            SetRotation(angle);
        }
        else {//it is in x<=0
            SetRotation(-angle);//adjust the angle
        }
        ////std::cout << "Update velocity"<< velocity_.x << std::endl;
    }
    void BlueGameObject::setWant(bool s) {
        want_shoot = s;
        // //std::cout << "set shoot :" << s << std::endl;
    }
    bool BlueGameObject::getShoot() {
        // //std::cout << "want shoot :" << want_shoot << std::endl;
        return want_shoot;
    }

    void BlueGameObject::Run(double delta_time) {
        glm::vec2 Myposition, Target, Player_position;
        glm::vec2 direction, velocity;
        if (timer_react.Finished()) {//adjust velocity every 
            Myposition = glm::vec2(GetPosition().x, GetPosition().y);
            Player_position = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
            velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
            //calculate the velocity and adjust it with unit of speed
            velocity = speed * (Player_position - Myposition) / glm::length(Player_position - Myposition);
            glm::vec2 a;
            a = Player_position - Myposition - velocity;
            velocity = -(velocity + a * (float)delta_time);
            if (glm::length(velocity) > speed) {
                velocity = speed * velocity / glm::length(velocity);
            }

            SetVelocity(glm::vec3(velocity, 0));//set new velocity
            if (glm::dot(Player_position, -velocity) == 0) {
                timer_react.Start(react);//restart the timer
            }
        }
    }

    void BlueGameObject::back() {

        glm::vec2 Myposition, Target;
        glm::vec2 direction, velocity;
        Myposition = glm::vec2(GetPosition().x, GetPosition().y);
        Target = glm::vec2(target.x, target.y);
        velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
        velocity = speed * (Target - Myposition) / glm::length(Target - Myposition);
        SetVelocity(glm::vec3(velocity, 0));//set new velocity
        target = glm::vec3(Target, 0);
    }
    void BlueGameObject::setFortress(GameObject* f) {
        fortress_ = f;
        target = f->GetPosition();
    }

    // Update function for moving the player object around
    // Update status
    void BlueGameObject::Update(double delta_time) {
        glm::vec2 Myposition, velocity, T, Player;

        if (this->GetHP() < 1) {//same as player
            if (this->GetAlive() && this->GetColliable()) {
                this->SetAlive(false);
                //Explosion();
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
                if (findPlayer()) {
                    statue = 4;
                }
                if (max_hp > hitpoint) {
                    statue = 6;
                }
                break;

            case 4:                
                Myposition = glm::vec2(GetPosition().x, GetPosition().y);
                Player = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
                
                if (glm::length(Myposition - Player) > 5) {
                    statue = 2;
                    if (max_hp > hitpoint) {
                        statue = 6;
                    }
                }

                break;
            case 6:
                Myposition = glm::vec2(GetPosition().x, GetPosition().y);
                Player = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
                if (glm::length(Myposition - Player) < 2) {
                    if (timer_react.Finished()) {
                        timer_react.Start(react);
                        statue = 4;
                    }
                }
                break;

            default:
                break;
            }
            switch (this->GetState())
            {
            case 2:
                patrolling();
                break;
            case 4:
                Run(delta_time);
                break;
            case 6:
                back();
                Myposition = glm::vec2(GetPosition().x, GetPosition().y);
                if (glm::length(Myposition - glm::vec2(target.x, target.y)) <= 1.2) {
                    back_ = true;
                //    colliable = false;
                }
            default:
                break;
            }


            velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
            Myposition = glm::vec2(position_.x, position_.y);
            T = Myposition + (float)delta_time * velocity;//calculate the new position by new velocity
            SetPosition(glm::vec3(T.x, T.y, 0));//set the new position
        }




        t_ += delta_time;//update the t_

    }

}// namespace game