#include "shop_obj.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp> 
#include <queue>
#include <algorithm>

#include <stack>
namespace game {

    Shop::Shop(void) {
        player_ = NULL;
        hover_node_ = NULL;
        item_sptite_ = NULL;
        node_sprite_ = NULL;
        number_sprite_ = NULL;
        ghost_ = true;
    }

    void Shop::BuildShop(GameObject* node_sprite, GameObject* item_sptite, GameObject* number_sprite,GameObject* text_sprite) {
        node_sprite_ = node_sprite;
        item_sptite_ = item_sptite;
        number_sprite_ = number_sprite;
        text_sprite_ = text_sprite;
        node_sprite_->SetScale(glm::vec2(0.5, 0.5));
        node_sprite_->SetShape_(2);
        node_sprite_->SetTOO(glm::vec2(0.5f, 0.0f));
       // item_sptite_->SetTOO(glm::vec2(0.5f, 0.5f));
       // number_sprite_->SetTOO(glm::vec2(0.5f, 0.5f));
        text_sprite_->SetTOO(glm::vec2(0.5f, 0.0f));

        item_sptite_->SetScale(glm::vec2(0.5, 0.5));
        item_sptite_->SetNumFrame(glm::vec2(4, 1));

        number_sprite_->SetScale(glm::vec2(0.5, 0.5));
        number_sprite_->SetNumFrame(glm::vec2(5, 2));

        Node* n0 = AddNode(0, -2.0, 1.5);
        Node* n1 = AddNode(1, -2.0, 0.5);
        Node* n2 = AddNode(2, -2.0, -0.5);
        Node* n3 = AddNode(3, 2.0, 2.5);
       // Node* n4 = AddNode(4, -2.0, -4.0);
    }

    // Add a node to the graph
    Node* Shop::AddNode(int id, float x, float y) {

        // Create and add new node to the graph
        Node* node = new Node(id, x, y);
        node_.push_back(node);
        return node;
    
    }
    void Shop::CloseShop() {
        ghost_ = true;
    }
    void Shop::OpenShop() {
        ghost_ = false;
    }
    Node* Shop::SelectNode(double x, double y, int window_width, int window_height, float camera_zoom) {

        // If the mouse is outside the window, return NULL
        if (x < 0 || x > window_width || y < 0 || y > window_height) {
            return NULL;
        }

        // Get position in world coordinates corresponding to the mouse
        // pixel position on the window
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

        // Find node at the derived position
        // This is done in a brute-force manner by checking each node, and
        // can be improved with spatial query structures
        float node_scale = node_sprite_->GetScale().x;
        for (int i = 0; i < node_.size(); i++) {
            // Check if mouse is inside a ball of radius scale*scale
            glm::vec2 center(node_[i]->GetX(), node_[i]->GetY());
            glm::vec2 cursor(cursor_x_pos, cursor_y_pos);
            if (glm::length(cursor - center) < node_scale * node_scale) {
                return node_[i];
            }
        }

        // Return NULL by default (no node found)
        return NULL;
    }
    void Shop::Update(GLFWwindow* window, float zoom) {
        if (ghost_) { return; }
        // Get mouse pixel position in the window
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Get information about the window
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Find node at the given pixel position
        Node* n = SelectNode(xpos, ypos, width, height, zoom);

        // Set the hover node to this node
        // It is fine if the node is NULL
        hover_node_ = n;

        // Check mouse clicks
        
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            if (n != NULL) {
                ClickNode(n);
            }

        }
        

        //Check mouse clicks, only for click right.

    }
    void Shop::ClickNode(Node* node) {
        std::cout << "click:  "<<node->GetId() << std::endl;
        int c1, c2, c3, c4;
        c1 = player_->Get_Iron();
        c2 = player_->Get_Coin();
        switch (node->GetId())
        {

        case 0:
            if (!player_->Get_Can_lazer()) {
                if ((c1 >= 10) && (c2 >= 10)) {
                    player_->SetCan_Lazer(true);
                    player_->Set_Iron(c1 - 10);
                    player_->Set_Coin(c2 - 10);
                }
            }
            break;
        case 1:
            if (!player_->Get_Can_shield()) {

                if ((c1 >= 10) && (c2 >= 10)) {
                    player_->SetCan_Shield(true);
                    player_->Set_Iron(c1 - 10);
                    player_->Set_Coin(c2 - 10);
                }
            }
        case 2:
            break;
        case 3:
            CloseShop();
            break;
        
        default:
            break;
        }
    }

    void Shop::SetPlayer(GameObject* player) {
        player_ = player;
    }
    void Shop::Render(glm::mat4 view_matrix, double current_time) {
        if (ghost_) { return; }

        // First, render all the nodes in the graph so that they appear on
        // top of the edges
        //
        // Go through each node and render it using the provided game object
        for (int i = 0; i < node_.size(); i++) {

            // Get the current node to draw
            Node* current_node = node_[i];

            // Set the position of the sprite with the position of the node
            glm::vec3 pos(current_node->GetX(), current_node->GetY(), 0.0f);
            

            // Set the color of the node via the color modifier uniform
            // The default color is green
            

            // Change the color modifier uniform depending on whether the
            // node is the start or end node of the path, a node in the
            // middle of the path, or the mouse is hovering over the node

             if (current_node == hover_node_) {

                 switch (current_node->GetId()) {
                 case 0:
                     text_sprite_->SetPosition(pos+ glm::vec3(0.0, 0.5, 0.0f));
                     text_sprite_->SetScale(glm::vec2(4.0, 0.2));
                     text_sprite_->SetText("After unlock, press L to use lazer!!");
                     text_sprite_->Render(view_matrix, current_time);

                     text_sprite_->SetPosition(pos + glm::vec3(0.0, 0.3, 0.0f));
                     text_sprite_->SetScale(glm::vec2(4.2, 0.2));
                     text_sprite_->SetText("This is the best ship killer..anyway..");
                     text_sprite_->Render(view_matrix, current_time);

                     node_sprite_->SetScale(glm::vec2(4.2, 0.8));
                     node_sprite_->SetRGB(0.0f, 0.0f, 0.0f, 1.0f); // Dark 
                     node_sprite_->SetPosition(pos + glm::vec3(0.0, 0.4, 0.0f));
                     node_sprite_->Render(view_matrix, current_time);
                     break;
                 case 1:
                     text_sprite_->SetPosition(pos + glm::vec3(0.0, 0.5, 0.0f));
                     text_sprite_->SetScale(glm::vec2(4.5, 0.2));
                     text_sprite_->SetText("After unlock, press Space to use Shield!!");
                     text_sprite_->Render(view_matrix, current_time);
                     text_sprite_->SetPosition(pos + glm::vec3(0.0, 0.3, 0.0f));
                     text_sprite_->SetScale(glm::vec2(4.2, 0.2));
                     text_sprite_->SetText("You should buy lazer first, honestly..");
                     text_sprite_->Render(view_matrix, current_time);

                     node_sprite_->SetScale(glm::vec2(4.5, 0.8));
                     node_sprite_->SetRGB(0.0f, 0.0f, 0.0f, 1.0f); // Dark 
                     node_sprite_->SetPosition(pos + glm::vec3(0.0, 0.4, 0.0f));
                     node_sprite_->Render(view_matrix, current_time);
                     break;
                 case 2:
                     text_sprite_->SetPosition(pos + glm::vec3(0.0, 0.5, 0.0f));
                     text_sprite_->SetScale(glm::vec2(4.2, 0.2));
                     text_sprite_->SetText("This is the bomb, press K to use Bomb!");
                     text_sprite_->Render(view_matrix, current_time);
                     text_sprite_->SetPosition(pos + glm::vec3(0.0, 0.3, 0.0f));
                     text_sprite_->SetScale(glm::vec2(4.43, 0.2));
                     text_sprite_->SetText("The useless thing, shouldn't waste money.");
                     text_sprite_->Render(view_matrix, current_time);

                     node_sprite_->SetScale(glm::vec2(4.5, 0.8));
                     node_sprite_->SetRGB(0.0f, 0.0f, 0.0f, 1.0f); // Dark 
                     node_sprite_->SetPosition(pos + glm::vec3(0.0, 0.4, 0.0f));
                     node_sprite_->Render(view_matrix, current_time);
                     break;
                 }
                 node_sprite_->SetPosition(pos);
                 node_sprite_->SetRGB(1.0f, 0.6f, 1.0f, 1.0f); // Pink
                 node_sprite_->SetScale(glm::vec2(0.5, 0.5));
                 node_sprite_->Render(view_matrix, current_time);
            }

             node_sprite_->SetRGB(0.0f, 0.6f, 0.0f, 1.0f); // Dark green
             node_sprite_->SetPosition(pos);
             node_sprite_->SetScale(glm::vec2(0.5, 0.5));
             switch (current_node->GetId())
             {
             case 0:

                 text_sprite_->SetPosition(pos+ glm::vec3(-0.0, 0.5, 0.0f));
                 text_sprite_->SetScale(glm::vec2(2, 0.3));
                 text_sprite_->SetText("Unlock Lazer.");
                 text_sprite_->Render(view_matrix, current_time);


                 node_sprite_->Render(view_matrix, current_time);
                 pos += glm::vec3(1.0, 0.0, 0.0f);
                 item_sptite_->SetPosition(pos);
                 item_sptite_->SetCurrentFrame(2);
                 item_sptite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(1);
                 number_sprite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(0);
                 number_sprite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.7, 0.0, 0.0f);
                 item_sptite_->SetPosition(pos);
                 item_sptite_->SetCurrentFrame(3);
                 item_sptite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(1);
                 number_sprite_->Render(view_matrix, current_time);
                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(0);
                 number_sprite_->Render(view_matrix, current_time);

                 
                 break;
             case 1:
                 text_sprite_->SetPosition(pos + glm::vec3(-0.0, 0.5, 0.0f));
                 text_sprite_->SetScale(glm::vec2(2, 0.3));
                 text_sprite_->SetText("Unlock Shield.");
                 text_sprite_->Render(view_matrix, current_time);


                 node_sprite_->Render(view_matrix, current_time);
                 pos += glm::vec3(1.0, 0.0, 0.0f);
                 item_sptite_->SetPosition(pos);
                 item_sptite_->SetCurrentFrame(2);
                 item_sptite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(1);
                 number_sprite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(0);
                 number_sprite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.7, 0.0, 0.0f);
                 item_sptite_->SetPosition(pos);
                 item_sptite_->SetCurrentFrame(3);
                 item_sptite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(1);
                 number_sprite_->Render(view_matrix, current_time);
                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(0);
                 number_sprite_->Render(view_matrix, current_time);
                 break;
             case 2:
                 text_sprite_->SetPosition(pos + glm::vec3(-0.0, 0.5, 0.0f));
                 text_sprite_->SetScale(glm::vec2(4/3, 0.3));
                 text_sprite_->SetText("Buy Bomb.");
                 text_sprite_->Render(view_matrix, current_time);


                 node_sprite_->Render(view_matrix, current_time);
                 pos += glm::vec3(1.0, 0.0, 0.0f);
                 item_sptite_->SetPosition(pos);
                 item_sptite_->SetCurrentFrame(2);
                 item_sptite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(0);
                // number_sprite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(5);
                 number_sprite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.7, 0.0, 0.0f);
                 item_sptite_->SetPosition(pos);
                 item_sptite_->SetCurrentFrame(3);
                 item_sptite_->Render(view_matrix, current_time);

                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(0);
            //     number_sprite_->Render(view_matrix, current_time);
                 pos += glm::vec3(0.5, 0.0, 0.0f);
                 number_sprite_->SetPosition(pos);
                 number_sprite_->SetValue(5);
                 number_sprite_->Render(view_matrix, current_time);
             case 3:
                 node_sprite_->Render(view_matrix, current_time);

             default:
                 break;
             }


        }
        // Render the game object for the current node


    
    }


}