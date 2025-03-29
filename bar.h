#ifndef BAR_H_
#define BAR_H_
#include "game_object.h"
#include <vector>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

namespace game {
	//��������UI��С���飬�ں��������С�������������ʲô���Ķ�����
	//Ȼ��ͬʱ���ݸ�ĸ�����ݽ��и��¡�
	class Bar :public GameObject {
	//use ghost to see or not see?, if ghost, then not render.
	public:
		Bar(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture,GameObject* parent);
		~Bar();
		void Init();
		void AddChild(GameObject* child) override;
	//��gameobject list��ĩβ�׶Σ�����ֱ�ӿ���for loop����׷�����Ƿ�alive��
	//ֻ��Ҫrender������
		void Update(double delta_time) override;

		void SetTOO(glm::vec2 t)override;

		void Render(glm::mat4 view_matrix, double current_time);
		glm::mat4 GetTransformation() override;
		glm::mat4 GetLocalTransformation();
		void SetWindowHeight(int h)override;
		void SetWindowWidth(int h)override;

		void SetPlace_Screen(float a, float b)override;

		void UpdatePosition(GameObject* N, int window_width, int window_height, float camera_zoom);
		void UpdateValue(int index_1, int index_2,int index_3, float value) override;

		void SetZoom(float f) override;
		float getZoom()override { return camera_zoom_; };
	private:
		std::vector<GameObject*> chile_game_objects_;//�ڲ�������С����
		GameObject* parent_;//Ҳ���ڸ�ĸ��
		int window_width;
		int window_height;
		float camera_zoom_=0.25f;
	};//class bar
	
}
#endif //BAR_H_