#ifndef VALUE_OBJECT_H_
#define VALUE_OBJECT_H_
#include "game_object.h"
#include <vector>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

namespace game {
	//存储着一个数据的小OBJECT
	//render的效果会根据这个数据更新。
	class Value_Object :public GameObject {
		//use ghost to see or not see?, if ghost, then not render.
	public:
		Value_Object(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture,int type,float value, GameObject* parent);
		~Value_Object();
		void Init();
		//void AddChild(GameObject* child) override;
		void Update(double delta_time) override;

		void Render(glm::mat4 view_matrix, double current_time);

		int chooseFrame(int n);
		void RenderNumber(glm::mat4 view_matrix, double current_time, int number);
		glm::mat4 GetTransformation() override;
		glm::mat4 GetLocalTransformation();
		
		void SetValue(float a) override{ value_ = a; }
		
		int type_;//是什么类型的 value？进度条用的，数字，文字。1进度条，2数字，3文字。
		float value_;
		GameObject* parent_;//也存在父母。
		
	};//class VB

}
#endif //VALUE_OBJECT_H_