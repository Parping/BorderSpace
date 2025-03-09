#include "expo_obj.h"
#include <iostream>
namespace game {
	Expo_obj::Expo_obj(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {

		alive = true;
		colliable = false;
		type = -1;
		time.Start(5);
		ghost = false;

	}



	void  Expo_obj::Update(double delta_time) {
		if (time.Finished()) {
			//std::cout << "finished" << std::endl;
			alive = false;
		}
	}
} // namespace game
