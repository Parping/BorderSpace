#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "mini_map_Object.h"

namespace game {
	Mini_map_Object::Mini_map_Object(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent)
		:GameObject(position, geom, shader, texture) {
		parent_ = parent;
		Init();

	}
	Mini_map_Object::~Mini_map_Object() {
		for (int i = 0; i < chile_game_objects_.size(); i++) {
			delete chile_game_objects_[i];
		}
	}
	void Mini_map_Object::Init() {

	}
	void Mini_map_Object::AddChild(GameObject* child) {
		chile_game_objects_.push_back(child);
	}
	void Mini_map_Object::emptyChild() {
		chile_game_objects_.clear();
	}
	void Mini_map_Object::SetWindowHeight(int h) {
		window_height = h;

		for (int i = 0; i < chile_game_objects_.size(); i++) {
			//chile_game_objects_[i]->SetWindowHeight(this->GetPlace_Screen().y * window_height);
		}

	};
	void Mini_map_Object::SetWindowWidth(int h) {
		window_width = h;
		for (int i = 0; i < chile_game_objects_.size(); i++) {
			//chile_game_objects_[i]->SetWindowWidth(this->GetPlace_Screen().x * window_width);
		}

	};
	void Mini_map_Object::SetPlace_Screen(float a, float b) {
		place_screen_ = glm::vec2(a, b);
		for (int i = 0;i < chile_game_objects_.size();i++) {

		}
	}
	glm::mat4 Mini_map_Object::GetTransformation() {
		// Setup the rotation matrix for the shader
		glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));
		// Set up the translation matrix for the shader
		glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
		glm::mat4 parent_translation_matrix = glm::translate(glm::mat4(1.0f), parent_->GetPosition());
		//glm::mat4 parent_transformation_matrix = parent_->GetTransformation();
		glm::mat4 transformation_matrix = parent_translation_matrix * translation_matrix * rotation_matrix;
		return transformation_matrix;
	}
	glm::mat4 Mini_map_Object::GetLocalTransformation() {
		glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));
		glm::mat4 T_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(toOrigin_.x, toOrigin_.y, 0.0));
		glm::mat4 transformation_matrix = GetTransformation() * scaling_matrix * T_matrix;
		return transformation_matrix;
	}
	void Mini_map_Object::Update(double delta_time) {
		// Use aspect ratio to properly scale the window
		UpdatePosition(this, window_width, window_height, camera_zoom_);
		for (int i = 0; i < chile_game_objects_.size(); i++) {
			//	std::cout << chile_game_objects_[i]->getZoom()<<std::endl;
			UpdatePosition(chile_game_objects_[i], window_width, window_height, chile_game_objects_[i]->getZoom());
		}
	}

	void Mini_map_Object::SetTOO(glm::vec2 t) {
		toOrigin_ = t;
		for (int i = 0; i < chile_game_objects_.size(); i++) {
			//chile_game_objects_[i]->SetTOO(t);
		}
	}
	void Mini_map_Object::UpdateValue(int index_1, int index_2, int index_3, float value) {
		if (index_1 >= chile_game_objects_.size()) { return; }
		chile_game_objects_[index_1]->UpdateValue(index_2, index_3, 0, value);
		chile_game_objects_[index_1]->SetValue(value);
	}
	void Mini_map_Object::UpdatePosition(GameObject* N, int window_width, int window_height, float camera_zoom) {
		float x, y, z;
		x = N->GetPlace_Screen().x * window_width;
		y = N->GetPlace_Screen().y * window_height;
		z = N->GetPosition().z;
		if (camera_zoom == 0.5) {
			1;
		}
		if (GetPlace_Screen().x == GetPlace_Screen().y == 0) { return; }
		float w = window_width;
		float h = window_height;
		float cursor_x_pos = 0.0;
		float cursor_y_pos = 0.0;
		if (w > h) {
			float aspect_ratio = w / h;
			cursor_x_pos = ((2.0f * x - w) * aspect_ratio) / (w * camera_zoom);
			cursor_y_pos = (-2.0f * y + h) / (h * camera_zoom);
		}
		else {
			float aspect_ratio = h / w;
			cursor_x_pos = (2.0f * x - w) / (w * camera_zoom);
			cursor_y_pos = ((-2.0f * y + h) * aspect_ratio) / (h * camera_zoom);
		}
		N->SetPosition(glm::vec3(cursor_x_pos, cursor_y_pos, z));
	}

	
	void Mini_map_Object::SetZoom(float f) {
		camera_zoom_ = f;
	}

	void Mini_map_Object::SetPlayerSp(GameObject* p) {
		player_spirt = p;
	}
	void Mini_map_Object::SetEnemySp(GameObject* p) {
		enemy_spirt = p;
	}
	void Mini_map_Object::RenderMap(glm::mat4 view_matrix, double current_time) {
		if (ghost) {
			return; }
		glm::vec3 position;
		position = player_->GetPosition();
		player_spirt->SetPosition(position);
		player_spirt->SetRGB(0.0, 1.0, 0.0, 1.0);
		player_spirt->Render(view_matrix, current_time);

		float x_max, y_max;
		x_max = size_zoom_ * scale_.x / 2 - 2 * GetTOO().x;
		y_max = size_zoom_ * scale_.y / 2 - 2 * GetTOO().y;
		for (int i = 0; i < chile_game_objects_.size(); i++) {
			position = chile_game_objects_[i]->GetPosition();

			if ((abs(player_->GetPosition().y - position.y) <= y_max) &&(abs(player_->GetPosition().x-position.x) <= x_max)) {

				player_spirt->SetPosition(position);
				player_spirt->SetRGB(1.0, 0.0, 0.0, 1.0);
				player_spirt->Render(view_matrix, current_time);
				//chile_game_objects_[i]->Render(view_matrix, current_time);
			}



		}
		
	}



}//namespace game