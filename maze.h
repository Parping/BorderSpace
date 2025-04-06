#ifndef MAZE_H_
#define MAZE_H_

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "node.h"

#include "shader.h"
#include "game_object.h"

namespace game{

    // A graph with connected nodes
    class Maze {

        public:
            // Lightweight constructor
            Maze(void);


            void BuildEmptyMaze(GameObject* wall_sp, GameObject* path_sp);
            // Add a node to the graph
            Node* AddNode(int id, float x, float y);
            Node* AddWall(int id, float x, float y);
            // Build a graph with a grid layout
            // Parameters: columns and rows of the grid, displacement of
            // each node along x and y, starting x and y position, viewport
            // height and sprite information
            void BuildGrid(int cols, int rows, float disp_x, float disp_y, float start_x, float start_y, float viewport_height, GameObject* node_sprite, GameObject* edge_sprite);

            void BuildWall();
            bool IsWallBetween(Node* a, Node* b);
            // Print out associated data for each node in the graph
            void PrintData(void);

            // Get mouse input, update start and end node, and compute
            // shortest path between the two nodes
            void Update(GLFWwindow* window, float zoom);
            Node* FindWall(float x, float y);

            // Return the node at the given index
            inline Node* GetNode(int index) { return node_[index]; }
            inline int GetNumNodes(void) { return node_.size(); }

            // Render all the nodes in the graph
            void Render(glm::mat4 view_matrix, double current_time);

            // Create and mark a path from start to end
            void FindPath(void);

            //get the next node that going to be moved
            Node* GetNextNode();

            // Getters
            inline Node* GetStartNode(void) { return start_node_; }
            inline Node* GetEndNode(void) { return end_node_; }


            // Setters
            inline void SetStartNode(Node* node) { start_node_ = node; }
            inline void SetEndNode(Node* node) { end_node_ = node; }

            // Maze generation
            // Take the current graph as input, generate a graph with a
            // maze, and return the new graph as the output
            void BuildMaze(Maze& output);

            //reset all node and edge after finished
            void ResetAll();

        private:

            GameObject* wall_obj_;
            GameObject* path_obj_;

            // Vector containing all the nodes in the graph
            std::vector<Node*> node_;
            std::vector<Node*> wall_;
            // Node that the mouse is hovering over
            Node* hover_node_;

            // Members for computing shortest paths in the graph

            // Start and end nodes of a path
            Node* start_node_;
            Node* end_node_;

            // Nodes in current shortest path
            std::vector<Node*> path_node_;
    };

} // namespace game

#endif // GRAPH_H_
