#include "player_game_object.h"
#include <iostream>
namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture,int hp,Circle circle)
	: GameObject(position, geom, shader, texture)
	 {//,collider_(scale_.x, this)
	hitpoint = hp;
	circle = circle;
	alive = true;
	colliable = true;
	item = 0;//internal item counter
	state = 0;//state for invicible
	timer_invi = Timer();
	type = 1;//type to distinguish different gameobject
	speed = 5;//speed of the player
	t_ = 0;//time passed
	max_velocity = 2.0f;//max velocity is 2 unit
	velocity_ = glm::vec3(0,0,0);
	collision_timer = Timer();
	invisible_point_=0;
	energy_=100;
	iron_=10;
	coin_=10;
	experience_=0;
	level_=1;
	max_exp=100;
	max_hp=hp;
	max_energy=100;
	lazer_act_ = false;
	shield_act_ = false;
	rest_ = false;
	bomb_ = 0;
	can_lazer_ = false;
	can_shield_ = false;
	accelerate = false;
	engine_active = false;
	collect_timer = Timer();
	collect_re_ = false;

}

//gettters and setters
Circle* PlayerGameObject::GetCircle()   {
	return &circle;
}
int PlayerGameObject::GetHP() {
	return hitpoint;
}
bool PlayerGameObject::GetAlive() {
	return alive;
}
bool PlayerGameObject::GetColliable() {
	return colliable;
}
void PlayerGameObject::SetColliable(bool a) {
	colliable = a;
}
void PlayerGameObject::clearChild() {
	chile_game_objects_.clear();
}
void PlayerGameObject::SetAlive(bool a) {
	alive = a;
	for (int i = 0; i < chile_game_objects_.size(); i++) {
		chile_game_objects_[i]->SetAlive(a);
		chile_game_objects_[i]->SetGhost(!a);
	}
	
}
void PlayerGameObject::AddChild(GameObject* child) {
	chile_game_objects_.push_back(child);
}
//updated getters and setters
int PlayerGameObject::GetItem() {
	return item;
}
int PlayerGameObject::GetState() {
	return state;
}
float PlayerGameObject::GetSpeed() {
	 return speed; 
}
//collision function
void  PlayerGameObject::Get_Collision(double delta_time) {
	////std::cout << " Get Collision" << std::endl;
	if (state != 1) {//normal state
		
		if (collision_timer.Finished()) {
			if (hitpoint > 0) {// get collision, hitpoint - 1
				if (shield_act_) { energy_-=1; }
				else {
					hitpoint -= 5;
				}
			}
			collision_timer.Start(delta_time*10);
		}
		//if (hitpoint <= 0) {//if it is <= 0, then it is dying and get explosion
		//	Explosion();
		//}
	}
}
void PlayerGameObject::CollideWall(double delta_time, glm::vec3 new_velocity) {
	if (!collision_timer.Finished()) { return; }
	collision_timer.Start(delta_time);
	if (glm::length(new_velocity) > max_velocity) {//if > max, then normalize it to save the direction
		velocity_ = glm::normalize(new_velocity) * max_velocity;
	}
	else {
		velocity_ = new_velocity;
	}

	////std::cout << "Update angle" << angle << std::endl;

}
void PlayerGameObject::Get_Collision_Pro(double delta_time, int pro_type,int attacker) {
	if (attacker == 1) { return; }
	if (state == 1) { return; }
	if (shield_act_) {
		energy_-=5;
		return;
	}
	if (hitpoint > 0) {// get collision, hitpoint - 1
		if (collision_timer.Finished()) {
			switch (pro_type)
			{
			case 51:
				hitpoint -= pro_base.killer_point * (attacker - 50);
				break;
			case 52:
				hitpoint -= floor(pro_lazer.killer_point * (attacker - 50) * 1.2);
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

//Explosion function
void PlayerGameObject::Explosion() {
	timer_exp.Start(5.0);//set the timer. the obj still need to be exist in 5s
	colliable = false;// when it's explosing, it's not colliable.
}
void PlayerGameObject::SetCollect_res(bool a) {

	collect_re_ = a;
}
bool PlayerGameObject::GetCollect_res() {
	return collect_re_;
}
bool PlayerGameObject::IsCollect_Need_render() {
	return collect_re_ && start_collect_;
}
void PlayerGameObject::collect_resource(GameObject* g) {
	float distance = glm::length(GetPosition() - g->GetPosition());
	if (collect_re_ == true) {
		if (!start_collect_) {
			if (distance <= 1.4f * (GetCircle()->get_r() + g->GetCircle()->get_r())) {
				
				start_collect_ = true;
				collect_timer.Start(2);
			}
		}
		else if (distance > 1.4f * (g->GetCircle()->get_r() + GetCircle()->get_r())) {//cannot continue collect
			start_collect_ = false;
		}
		else if (g->GetState() != 0){
			start_collect_ = false;
		}
		else if (start_collect_&&collect_timer.Finished()) {
			iron_ += 10;
			energy_ += 20;
			Add_Exp(30);
			start_collect_ = false;
			g->SetStatue(99);
		}
	}
}
float PlayerGameObject::Get_collect_timer(double t) {
	float time = (float)collect_timer.getEnd();
	float current_time = (float)t;
	if (time > current_time) {
		return (1 -(time - current_time) / 2.0f);
	}
	else return 0;
}
void PlayerGameObject::CollectItem(int type) {
	switch (type)
	{
	case 11:
		invisible_point_++;

		break;
	case 12:
		if ((energy_ + 10) > max_energy) {
			energy_ = max_energy;
		}
		else {
			energy_ += 10;
		}
		break;
	case 13:
		iron_++;
		break;
	case 14:
		coin_++;
		break;
	default:
		break;
	}
}
void PlayerGameObject::Invincible() {
	state = 1;//change state
	//std::cout << " Get Invincible" << std::endl;
	timer_invi.Start(10);//timer
}

void PlayerGameObject::Add_Exp(int a) {
	experience_ += a;
}
void PlayerGameObject::Level_up() {

	experience_ -= max_exp;
	level_++;
	max_exp += 50;
	max_hp += 50;
	max_energy += 50;
}
void PlayerGameObject::Set_Lazer_On(bool a)  { 
	if (energy_ <= 0 && a == true) {
		return;
	}
	if (!can_lazer_) { return; }
	lazer_act_ = a;
}
void PlayerGameObject::Set_Shield_On(bool a)  { 
	if (!can_shield_) { return; }
	shield_act_ = a; 
}

void PlayerGameObject::SetAcc(bool a) {
	if (energy_ <= 0 && a == true) {
		return;
	}
	accelerate = a;
}
// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {
	glm::vec3 P, d,T;
	P = GetPosition();
	d = GetBearing();
	if (accelerate) {
		max_velocity = 6.0f;
	}
	else if(!collect_re_){
		max_velocity = 2.0f;
	}
	else {
		max_velocity = 0.1f;
	}

	T = P + (float)delta_time * velocity_;

	SetPosition(glm::vec3(T.x, T.y, 0.0));
	t_ += delta_time;
	if (this->GetHP() < 1) {//if HP<1.get explosion
		if (this->GetAlive()&&this->GetColliable()) {
			//Explosion();
			this->SetAlive(false);
		}
		else {
			if (timer_exp.Finished()) {
				this->SetAlive(false);//if timer finished, died
			}
		}
	}
	if (this->GetState() == 1) {//if it is statue1,it is invisible
		if (timer_invi.Finished()) {//check the timer finished
			state = 0;//return to normal state
		}
	}
	if (this->Get_INPoint() >= 5) {//if it has 5 item then it changes it state
		Invincible();
		invisible_point_=0;//reset the item collision
	}
	if (Get_Exper() >= Get_Max_Exp()) {
		Level_up();
		hitpoint = max_hp;
	}


	if (rest_timer.Finished()) {
			if (energy_ < max_energy) {
				energy_++;
			}
	}
	if (shield_act_) {

		rest_timer.Start(5);
		if (energy_ <= 0) {
			shield_act_ = false;
		}
	}
	if (lazer_act_) {
		rest_timer.Start(5);
		if (act_timer.Finished()) {
			if (energy_ > 0) {
				energy_ -= 1;
			}
			else {
				lazer_act_ = false;
			}
			act_timer.Start(0.05);
		}
	}
	if (accelerate) {
		rest_timer.Start(5);
		if (energy_ > 0) {
			energy_ -= 1;
		}
		else {
			accelerate = false;
		}
	}
	if (!collect_re_) { start_collect_ = false; }

/*	// Special player updates go here
	if (alive && (!colliable)) {// if it is alive but it is not colliable, which means it is explosing
		if (timer_exp.Finished()) {// when the timer is finished, which means it died
			alive = false;
		}//if not, keep explosing
	}
	if (statue == 1) {
		if (timer_invi.Finished()) {
			statue = 0;
		}
	}*/
	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time);
}


void PlayerGameObject::SetVelocity(const glm::vec3& velocity) {

	if (glm::length(velocity) > max_velocity) {//if > max, then normalize it to save the direction
		velocity_ = glm::normalize(velocity) * max_velocity;
	}
	else {
		velocity_ = velocity;
	}


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
	//std::cout << "Update angle" << angle << std::endl;
	
}
void PlayerGameObject::AddVelocity(glm::vec3 a) {
	
	glm::vec3 new_velocity = a + velocity_;
	if (glm::length(new_velocity)> max_velocity) {//if > max, then normalize it to save the direction
		velocity_=glm::normalize(new_velocity) * max_velocity;
	}
	else {
		velocity_=new_velocity;
	}
}
void PlayerGameObject::getHeal() {
	hitpoint += 1;
}
glm::mat4 PlayerGameObject::GetTransformation() {
	

	// Setup the rotation matrix for the shader
	glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

	// Set up the translation matrix for the shader
	glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
	glm::mat4 transformation_matrix = translation_matrix * rotation_matrix;
	return transformation_matrix;
}
glm::mat4 PlayerGameObject::GetLocalTransformation() {
	glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));
	glm::mat4 transformation_matrix = GetTransformation() * scaling_matrix;
	return transformation_matrix;
}
} // namespace game
