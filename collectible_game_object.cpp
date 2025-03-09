#include "collectible_game_object.h"
#include <iostream>
namespace game {

	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle,int ty)
		: GameObject(position, geom, shader, texture) {
		circle = circle;
		collectible = true;
		colliable = true;
		type = ty;
		//std::cout << " collectible created!" << std::endl;
		ghost = false;
		num_frame = glm::vec2(4,1);
		switch (ty)
		{
		case 11:
			current_frame = 0;
			break;
		case 12:
			current_frame = 1;
			break;
		case 13:
			current_frame = 2;
			break;
		case 14:
			current_frame = 3;
			break;
		default:
			current_frame = 0;
			break;
		}
	}
	Circle* CollectibleGameObject::GetCircle() {
		return &circle;
	}
	//gettters and setters
	bool CollectibleGameObject::GetCollectible() {
		return collectible;
	}
	bool CollectibleGameObject::GetColliable() {
		return colliable;
	}
	void CollectibleGameObject::SetCollectible(bool a) {
		collectible = a;
	}



	void  CollectibleGameObject::Get_Collision(double delta_time) {
		//std::cout << " Get Collision" << std::endl;
		collectible = false;
		ghost = true;//set ghost mode
		colliable = false;

	}

	void  CollectibleGameObject::Update(double delta_time) {
		
	}
} // namespace game
