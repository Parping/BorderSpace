#include "player_game_object.h"
#include "monster_object.h"
#include <iostream>
namespace game {


    MonsterObject::MonsterObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle, double t, int statue, int ty)
        : GameObject(position, geom, shader, texture) {
        InitType(ty);
        circle = circle;
        alive = true;
        colliable = true;
        t_ = t;
        timer_react = Timer();
        collision_timer = Timer();
        reload_timer = Timer();
        attacking_timer = Timer();

        want_shoot = false;
        target = glm::vec3(0, 0, 0);
        back_ = false;

    }

    //getters
    Circle* MonsterObject::GetCircle() {
        return &circle;
    }
    //getters
    bool MonsterObject::GetAlive() {
        return alive;
    }
    bool MonsterObject::GetColliable() {
        return colliable;
    }
    int MonsterObject::GetHP() {
        return hitpoint;
    }
    int MonsterObject::GetState() {
        return statue;
    }
    float MonsterObject::GetSpeed() {
        return speed;
    }
    //setters
    void MonsterObject::SetColliable(bool a) {
        colliable = a;
    }
    void MonsterObject::SetAlive(bool a) {
        alive = a;
    }
    void MonsterObject::SetStatue(int a) {
        statue = a;
    }
    void MonsterObject::SetPlayer(GameObject* a) {
        player_ = a;
    }
    void MonsterObject::SetCenter(glm::vec3& a) {
        center_ = a;
    }
    void MonsterObject::SetWidth(float a) {
        width = a;
    }
    void MonsterObject::SetHeight(float a) {
        height = a;
    }

    void MonsterObject::Get_Collision_Pro(double delta_time, int pro_type, int attacker) {
        if (attacker != 1) { return; }

        if (hitpoint > 0) {// get collision, hitpoint - 1
            if (collision_timer.Finished()) {
                switch (pro_type)
                {
                case 51:
                    hitpoint -= pro_base.killer_point;
                    break;
                case 52:
                    if (statue == 0) {
                        hitpoint -= floor(pro_lazer.killer_point * 0.2);
                    }
                    else {
                        hitpoint -= floor(pro_lazer.killer_point * 1.2);
                    }
                    break;
                case 53:
                    if (statue == 0) {
                        hitpoint -= floor(pro_bomb.killer_point * 1.2);
                    }
                    else {
                        hitpoint -= floor(pro_bomb.killer_point * 0.2);
                    }
                default:
                    break;
                }
                collision_timer.Start(0.5f);
            }
        }
    }

    void MonsterObject::Get_Collision(double delta_time) {//when collision, hitpoint --, if hitpoint to 0, explosion
        if (collision_timer.Finished()) {
            std::cout << " Get Collision" << std::endl;
            collision_timer.Start((float)delta_time);
            if (hitpoint > 0) {// get collision, hitpoint - 1
                hitpoint--;
            }
            collision_timer.Start(0.5f);
        }


    }
    void MonsterObject::Explosion() {//change texture and count 5 s

        colliable = false;// when it's explosing, it's not colliable.
        alive = false;
    }

    void MonsterObject::patrolling() {
        glm::vec2 D = glm::vec2(
            -(this->width / 2.0f) * sin(speed * t_),
            (this->height / 2.0f) * cos(speed * t_));
        //calculate current velocity and adjust it by unit of speed

        SetVelocity(glm::vec3(speed * D, 0));//set velocity and adjust it by unit of speed
    }
    bool MonsterObject::findPlayer() {
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
    void MonsterObject::wakeup() {
        current_frame = 1;
        statue = 4;
    
    }
    void MonsterObject::InitType(int t) {
        int random = rand() % 100;
        switch (t)
        {


        case 95://0=normal, 2, 3, 10 = planet
            hitpoint = monster.hp_;
            type = monster.type_;
            speed = monster.speed_;
            scale_ = glm::vec2(monster.size_scale_, monster.size_scale_);
            react = monster.react_;
            shoot_desire = monster.shoot_desire_;
            reload = monster.reload_;
            statue = 0;
            num_frame = glm::vec2(2, 1);
            current_frame = 0;
            max_hp = hitpoint;
            sleep_timer.Start(20.0f + (rand() / (float)RAND_MAX) * 9.0f);
            //
            center_ = position_;//the center is the given position
            width = 1;
            height = 2;
            break;
        case 100:
            hitpoint = boss.hp_;
            type = boss.type_;
            speed = boss.speed_;
            scale_ = glm::vec2(boss.size_scale_, boss.size_scale_);
            react = boss.react_;
            shoot_desire = boss.shoot_desire_;
            reload = boss.reload_;
            statue = 0;
            num_frame = glm::vec2(1, 1);
            current_frame = 0;
            max_hp = hitpoint;
            center_ = position_;//the center is the given position
        default:
            break;
        }

    }

    void MonsterObject::intercepting(double delta_time) {
        glm::vec2 Myposition, Target, Player_position;
        glm::vec2 direction, velocity;
        float current_speed=speed;
        if (timer_react.Finished()) {//adjust velocity every 
            Myposition = glm::vec2(GetPosition().x, GetPosition().y);
            Player_position = glm::vec2(player_->GetPosition().x, player_->GetPosition().y);
            velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
            //calculate the velocity and adjust it with unit of speed
            if (want_shoot) {
                attacking_timer.Start(5);
                want_shoot = false;
            }

            if(attacking_timer.Finished()){
                current_speed = speed ;
            }
            else {
                current_speed = speed*4.0;
                
            }
            velocity = current_speed * (Player_position - Myposition) / glm::length(Player_position - Myposition);
            glm::vec2 a;
            a = Player_position - Myposition - velocity;
            velocity = velocity + a * (float)delta_time;
            if (glm::length(velocity) > current_speed) {
                velocity = current_speed * velocity / glm::length(velocity);
            }
            SetVelocity(glm::vec3(velocity, 0));//set new velocity
            if (glm::dot(Player_position, velocity) == 0) {
                timer_react.Start(react);//restart the timer
            }
        }
    }
    void MonsterObject::SetVelocity(const glm::vec3& velocity) {
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
        //std::cout << "Update velocity"<< velocity_.x << std::endl;
    }
    
    void MonsterObject::setWant(bool s) {
        want_shoot = s;
        // std::cout << "set shoot :" << s << std::endl;
    }
    bool MonsterObject::getShoot() {
        // std::cout << "want shoot :" << want_shoot << std::endl;
        return want_shoot;
    }

    void MonsterObject::back() {

        glm::vec2 Myposition, Target;
        glm::vec2 direction, velocity;
        Myposition = glm::vec2(GetPosition().x, GetPosition().y);
        Target = glm::vec2(0,0);
        velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
        velocity = speed * (Target - Myposition) / glm::length(Target - Myposition);
        SetVelocity(glm::vec3(velocity, 0));//set new velocity
        target = glm::vec3(Target, 0);
    }


    // Update function for moving the player object around
    // Update status
    void MonsterObject::Update(double delta_time) {
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
            case 0:
                if (sleep_timer.Finished()) {
                    if (type == 95) {

                        current_frame = 1;
                        int random = rand() % 100;

                        if ((random % 5) != 0) {//random choose the starting moving mode
                            statue = 2;
                        }
                        else {
                            statue = 3;
                        }
                    }
                }
                if (type == 100) {
                    if (findPlayer()) {
                        statue = 3;
                        reload_timer.Start(reload);
                    }
                }
                break;
            case 2://change velocity by patrolling
                if (findPlayer()) {//if it see player, change state to intercepting
                    statue = 3;
                    reload_timer.Start(reload);
                }

                break;
            case 3:
                break;
            default:
                break;
            }
            switch (this->GetState())
            {

            case 2:
                patrolling();
                break;
            case 3:
                intercepting(delta_time);//change velocity by intercepting
                if (reload_timer.Finished()&& attacking_timer.Finished()) {
                    if (!getShoot()) {
                        int random = rand() % 100;
                        if (random < shoot_desire) {//
                            setWant(true);
                        }
                        reload_timer.Start(reload);
                    }

                }
                break;
            case 4:
                speed = 2.0;
                back();
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
    }




