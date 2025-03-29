#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Value_Object.h"

namespace game {
	Value_Object::Value_Object(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, int type, float value, GameObject* parent)
		:GameObject(position, geom, shader, texture) {
		parent_ = parent;
		type_ = type;
		value_ = value;
		Init();

	}
	Value_Object::~Value_Object() {
	
	}
	void Value_Object::Init() {
	
	}
//	void Value_Object::AddChild(GameObject* child) override;
	void Value_Object::Update(double delta_time) {
		
	}

	void Value_Object::Render(glm::mat4 view_matrix, double current_time) {

		switch (type_)
		{
		case 1:
			SetBar_Percent(value_);
			SetCurrentFrame(0);
			GameObject::Render(view_matrix,current_time);
			SetBar_Percent(1.0);
			SetCurrentFrame(1);
			GameObject::Render(view_matrix, current_time);
			break;
		case 2:
			RenderNumber(view_matrix, current_time, value_);
			break;
		default:
			break;
		}
	
	
	}

	void Value_Object::RenderNumber(glm::mat4 view_matrix, double current_time, int number) {
		glm::vec2 scale = GetScale();
		glm::vec2 p_scale = parent_->GetScale();

		int index = 0;
		int temp = number;
		int result;
		int n;
		while (temp != 0) {
			temp = floor(temp / 10);
			index++;
		}
		for (int i = 0;i < p_scale.x/scale.x;i++) {
			if ((p_scale.x / scale.x - i) > index) {
				this->SetCurrentFrame(9);
			}
			else {
				result = floor(number / pow((double)10, (double)(index - 1)));
				int n = chooseFrame(result);
				this->SetCurrentFrame(n);
				number -= result * pow((double)10, (double)(index - 1));
				index--;
			}
			GameObject::Render(view_matrix, current_time);
			view_matrix = glm::translate(view_matrix, glm::vec3(scale.x, 0, 0));

		}

	}
	glm::mat4 Value_Object::GetTransformation() {
		// Setup the rotation matrix for the shader
		glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));
		// Set up the translation matrix for the shader
		glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
		glm::mat4 parent_translation_matrix = glm::translate(glm::mat4(1.0f), parent_->GetPosition());
		//glm::mat4 parent_transformation_matrix = parent_->GetTransformation();
		glm::mat4 transformation_matrix = parent_translation_matrix * translation_matrix * rotation_matrix;
		return transformation_matrix;
	}
	glm::mat4 Value_Object::GetLocalTransformation() {
		glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));
		glm::mat4 T_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(toOrigin_.x, toOrigin_.y, 0.0));
		glm::mat4 transformation_matrix = GetTransformation() * scaling_matrix* T_matrix;

		return transformation_matrix;
	}

	int Value_Object::chooseFrame(int n) {
		switch (n)
		{
		case 1:
			return 0;

		case 2:
			return 1;

		case 3:
			return 2;

		case 4:
			return 3;

		case 5:
			return 4;

		case 6:
			return 5;

		case 7:
			return 6;

		case 8:
			return 7;

		case 9:
			return 8;

		default:
			return 9;

		}
		return 0;
	}

}