#ifndef SHOP_H_
#define SHOP_H_
#include "game_object.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "node.h"
#include "shader.h"



namespace game {


    class Shop {
    public:
        // Lightweight constructor
        Shop(void);

        void BuildShop(GameObject* node_sprite, GameObject* item_sptite,GameObject* number_sprite,GameObject* text_sprite_);

        // Add a node to the graph
        Node* AddNode(int id, float x, float y);
        void CloseShop();
        void OpenShop();

        void Update(GLFWwindow *window, float zoom);
        Node* SelectNode(double x, double y, int window_width, int window_height, float camera_zoom);
        void ClickNode(Node* node);

        void SetPlayer(GameObject* player);
        void Render(glm::mat4 view_matrix, double current_time);

        void setGhost(bool g) { ghost_ = g; };

    private:
        GameObject* player_;
        std::vector<Node*> node_;
        Node* hover_node_;
        GameObject* item_sptite_;
        GameObject* node_sprite_;
        GameObject* number_sprite_;
        GameObject* text_sprite_;
        bool ghost_;
    };

}
#endif // SHOP_H_