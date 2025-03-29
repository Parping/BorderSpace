#ifndef VALUE_OBJECT_H_
#define VALUE_OBJECT_H_
#include "game_object.h"
#include <vector>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

namespace game {
	//�洢��һ�����ݵ�СOBJECT
	//render��Ч�������������ݸ��¡�
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
		
		int type_;//��ʲô���͵� value���������õģ����֣����֡�1��������2���֣�3���֡�
		float value_;
		GameObject* parent_;//Ҳ���ڸ�ĸ��
		
	};//class VB

}
#endif //VALUE_OBJECT_H_