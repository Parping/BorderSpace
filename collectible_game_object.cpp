#include "collectible_game_object.h"
#include <iostream>
namespace game {
	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Circle circle)
		: GameObject(position, geom, shader, texture) {
		circle = circle;
		collectible = true;
		colliable = true;
		type = 2;
		//std::cout << " collectible created!" << std::endl;
		ghost = false;
		
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



	void  CollectibleGameObject::Get_Collision() {
		//std::cout << " Get Collision" << std::endl;
		collectible = false;
		ghost = true;//set ghost mode
		colliable = false;
	}

	void  CollectibleGameObject::Update(double delta_time) {
		
	}
} // namespace game
