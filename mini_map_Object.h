#ifndef MINI_MAP_OBJECT_H_
#define MINI_MAP_OBJECT_H_
#include "Bar.h"

#include <vector>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

namespace game {
	//用来设置UI的小方块，内含所有这个小方块组合里面有什么样的东西。
	//然后同时根据父母的数据进行更新。
	class Mini_map_Object :public GameObject {
		//use ghost to see or not see?, if ghost, then not render.
	public:
		Mini_map_Object(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent);
		~Mini_map_Object();
		void Init();
		void AddChild(GameObject* child) override;

		void emptyChild() override;
		//在gameobject list的末尾阶段，可以直接控制for loop，不追踪它是否alive。
		//只需要render就行了
		void Update(double delta_time) override;

		void SetTOO(glm::vec2 t)override;

		void RenderMap(glm::mat4 view_matrix, double current_time) override;

	//	void Render(glm::mat4 view_matrix, double current_time) {};
		glm::mat4 GetTransformation() override;
		glm::mat4 GetLocalTransformation();
		void SetWindowHeight(int h)override;
		void SetWindowWidth(int h)override;

		void SetPlace_Screen(float a, float b)override;

		void UpdatePosition(GameObject* N, int window_width, int window_height, float camera_zoom);
		void UpdateValue(int index_1, int index_2, int index_3, float value) override;

		void SetZoom(float f) override;
		float getZoom()override { return camera_zoom_; };

		int GetWindowWidth() { return window_width; }
		int GetWindowHeight() { return window_height; }
		std::vector<GameObject*> GetAllChild() { return chile_game_objects_; }

		void SetPlayerSp(GameObject* p) override;
		void SetEnemySp(GameObject* p)override;

		void SetPlayer(GameObject* a) override { player_ = a; };
		void SetAllChild(std::vector<GameObject*> a) override { chile_game_objects_ = a; };

		void SetSizeZoom(float f)override { size_zoom_ = f; };

	private:
		std::vector<GameObject*> chile_game_objects_;//内部的所有小东西
		GameObject* player_;
		GameObject* parent_;//也存在父母。
		int window_width;
		int window_height;
		float camera_zoom_ = 0.25f;
		float size_zoom_;
		GameObject* player_spirt;
		GameObject* enemy_spirt;
	};//class bar

}
#endif //BAR_H_