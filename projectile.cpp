#include "projectile.h"
#include <iostream>
namespace game {
	Projectile::Projectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle,float angle,int from,int ty)
		: GameObject(position, geom, shader, texture) {
		circle = circle;
		colliable = true;
		exist_timer= Timer();
		//std::cout << " Projectile created!" << std::endl;
		alive = true;
		angle_ = angle;
		t_ = 0;		
		type = ty;
		from_id_ = from;
		startPosition = position;//initialy, currentposition=startposition
		Init();
	}
	Circle* Projectile::GetCircle() {
		return &circle;
	}
	//gettters and setters
	void Projectile::Init() {
		switch (type)
		{
		case 51:
			hitpoint = pro_base.hp_;
			scale_ = pro_base.size_;
			exist_timer.Start(pro_base.life_span_);//start counting lifespan
			speed = pro_base.speed_;
			velocity_ = speed * GetBearing();//init the velocity with the direction and speed
			break;
		case 52:
			hitpoint = pro_lazer.hp_;
			scale_ = pro_lazer.size_;
			exist_timer.Start(pro_lazer.life_span_);//start counting lifespan
			speed = pro_lazer.speed_;
			velocity_ = speed * GetBearing();//init the velocity with the direction and speed
			break;
		case 53:
			hitpoint = pro_bomb.hp_;
			scale_ = pro_bomb.size_;
			exist_timer.Start(pro_bomb.life_span_);//start counting lifespan
			speed = pro_bomb.speed_;
			velocity_ = speed * GetBearing();//init the velocity with the direction and speed
			num_frame = glm::vec2(2, 1);
			break;
		default:
			break;
		}

	}

	void  Projectile::Get_Collision(double delta_time) {
		
		alive = false;
		colliable = false;
	}

	void Projectile::Get_Collision_Pro(double delta_time, int pro_type, int attacker) {
		if ((from_id_ != 1) && (attacker != 1) ){return;}

		if (hitpoint > 0) {// get collision, hitpoint - 1
			if (collision_timer.Finished()) {
				if (pro_type != 52) {
					if (type <= pro_type) {
						hitpoint == 0;
						alive = false;
						colliable = false;
					}
					else if (type > pro_type) {
						hitpoint--;
					}
				}
				collision_timer.Start(delta_time);
			}
		}
	}


	bool Projectile::GetAlive() {
		return alive;
	}
	bool Projectile::GetColliable() {
		return colliable;
	}
	glm::vec3 Projectile::GetVelocity(void)  { return velocity_; };

	bool Projectile::RayToCircleCheck(glm::vec3 position, float r, double deltatime) {
		glm::vec2 P, velocity, C;
		velocity = glm::vec2(GetVelocity().x, GetVelocity().y);//projectile velocity
		P = glm::vec2(startPosition.x, startPosition.y);
		C= glm::vec2(position.x, position.y);//circle center

		float a, b, c;//follow the lecture
		a = glm::dot(velocity,velocity);
		b = glm::dot(2.0f * velocity, (P - C));
		c = glm::dot((P -C),(P - C))-r*r;
		//at2 + bt + c = 0

		//calculate t
		float delta = b * b - 4 * a * c;
		if (delta < 0) {//no real t
			return false;
		}
		float sqrt_delta = std::sqrt(delta);
		float t1 = (-b - sqrt_delta) / (2 * a);
		float t2 = (-b + sqrt_delta) / (2 * a);


		bool valid_t1 = (t1 >= 0);
		bool valid_t2 = (t2 >= 0);
		if (!valid_t1 && !valid_t2) {
			return false;//behind the startPoint, invalid
		}
		float current_t = t_;
		float last_t = t_ - deltatime;
		if (t1>t2) {
			if (t1 >= current_t && current_t >= t2) {//if current line is in the circle
				return true;
			}
			else if (current_t >= t1 && t2 >= last_t) {//if the cuttent line pass the circle
				return true;
			}
		}
		else {
			if (t2 >= current_t && current_t >= t1) {
				return true;
			}
			else if (current_t >= t2 && t1 >= last_t) {
				return true;
			}
		}
		return false;
	}
	void  Projectile::Update(double delta_time) {
		glm::vec2 Myposition, velocity, T,P;
		velocity = glm::vec2(GetVelocity().x, GetVelocity().y);
		P= glm::vec2(startPosition.x, startPosition.y);
		T = P + (float)t_ * velocity;//calculate the new position by new velocity
		SetPosition(glm::vec3(T.x, T.y, 0));//set the new position
		if (exist_timer.Finished()) {
			//std::cout << "shooter Died" << std::endl;
			alive = false;
			colliable = false;
		}
		t_ += delta_time;
	}
} // namespace game
