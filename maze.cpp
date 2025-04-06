#include <glm/gtc/matrix_transform.hpp> 
#include <queue>
#include <algorithm>
#include <iostream>   
#include <stack>

#include "maze.h"

namespace game {

    Maze::Maze(void) {

        // Initialize all members to default values
        wall_obj_ = NULL;
        path_obj_ = NULL;
        start_node_ = NULL;
        end_node_ = NULL;
        hover_node_ = NULL;
    }

    void Maze::BuildEmptyMaze(GameObject* wall_sp, GameObject* path_sp) {
        wall_obj_ = wall_sp;
        path_obj_ = path_sp;
    }
    Node* Maze::AddNode(int id, float x, float y) {

        // Create and add new node to the graph
        Node* node = new Node(id, x, y);
        node_.push_back(node);
        return node;
    }


    void Maze::BuildGrid(int cols, int rows, float disp_x, float disp_y, float start_x, float start_y, float viewport_height, GameObject* node_sprite, GameObject* edge_sprite) {

        // Set sprite game objects
        wall_obj_ = node_sprite;
        path_obj_ = edge_sprite;

        // Add nodes in a grid layout
        int id = 0;
        float x = start_x;
        float y = viewport_height - start_y;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                AddNode(id, x, y);
                id++;
                x += disp_x;
            }
            x = start_x;
            y -= disp_y;
        }

        // Connect neighbors
        int index = 0;
        int rand_weight = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // Only need to add neighbors to the right and bottom, as
                // the other neighbors are added automatically by
                // AddNeighbor

                // Right neighbor
                // Do not add neighbor if at the last column
                if (j < (cols - 1)) {
                    // Chose a random weight
                    // You can also try setting all weights to 1
                    rand_weight = 10 + (rand() % 6);
                    node_[index]->AddNeighbor(node_[index + 1], rand_weight);
                }

                // Bottom neighbor
                // Do not add neighbor if at the last row
                if (i < (rows - 1)) {
                    rand_weight = 10 + (rand() % 6);
                    node_[index]->AddNeighbor(node_[(i + 1) * cols + j], rand_weight);
                }

                // Increment index of current node
                index++;
            }
        }

        // Set default start and end nodes
        SetStartNode(node_[0]);
        SetEndNode(node_[rows * cols - 1]);

        // Find shortest path between nodes
      //  FindPath();
    }


    void Maze::PrintData() {

        // Loop through array and print out data for each node
        for (int i = 0; i < node_.size(); i++) {
            std::cout << "Node " << i << ": id: " << node_[i]->GetId() << ", x: " << node_[i]->GetX() << ", y: " << node_[i]->GetY() << ", number of neighbors: " << node_[i]->GetNumEdges() << std::endl;
        }
    }


    void Maze::Update(GLFWwindow* window, float zoom) {

        // Get mouse pixel position in the window
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Get information about the window
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        

    }

    Node* Maze::AddWall(int id, float x, float y) {
        Node* node = new Node(id, x, y);
        wall_.push_back(node);
        node->SetWall(true);
        return node;
    }
    Node* Maze::FindWall(float x, float y) {
        for (int i = 0; i < wall_.size(); i++) {
            // Check if mouse is inside a ball of radius scale*scale
            glm::vec2 center(wall_[i]->GetX(), wall_[i]->GetY());
            glm::vec2 cursor(x, y);
            if (glm::length(cursor - center) == 0) {//if the xy is closer enough, then we think the player is on this node
                return wall_[i];
            }
        }
        return NULL;
    }
    void Maze::BuildWall() {
        float size =3.0;
        float x = start_node_->GetX() - size/2 ;
        float y = start_node_->GetY() + size/2 ;

        float start_x = x;
        float down_edge_y = end_node_->GetY();
        glm::vec3 left(-size / 2, 0, 0);
        glm::vec3 right(size / 2, 0, 0);
        glm::vec3 up(-0.0, -size / 2, 0);
        glm::vec3 down(-0.0, size / 2, 0);
        int id = 0;
        for (int i = 0;i < 13 * 2 + 1;i++) {
            for (int j = 0;j < 17 * 2 + 1;j++) {
                AddWall(id, x, y);

                id++;
                x += size / 2;

            }
            x = start_x;
            y -= size / 2;
        }

        for (int i = 0; i < node_.size(); i++) {

            // Get the current node to draw
            Node* current_node = node_[i];
            glm::vec3 node_pos(current_node->GetX(), current_node->GetY(), 0);
            Node* path = FindWall(node_pos.x, node_pos.y);
            if (path != NULL) {
                path->SetWall(false);
            }
            // Render the edges of this node
            for (int i = 0; i < current_node->GetNumEdges(); i++) {
                // Get pointer to neighbor edge
                const Edge edge = current_node->GetEdge(i);
                Node* neigh = edge.n2;

                // Set the position of the edge sprite between the current
                // node and its neighbor
                glm::vec3 pos((current_node->GetX() + neigh->GetX()) / 2.0,
                    (current_node->GetY() + neigh->GetY()) / 2.0, 0.0f);
                path = FindWall(pos.x, pos.y);
                if (path != NULL) {
                    path->SetWall(false);
                }
            }
        }
    

    }
    bool Maze::IsWallBetween(Node* a, Node* b) {
        if (!a || !b) return true;
        for (int i = 0; i < a->GetNumEdges(); ++i) {
            if (a->GetEdge(i).n2 == b) {
                return false; // 有边连接 => 不是墙
            }
        }
        return true; // 没有边 => 是墙
    }


    Node* Maze::GetNextNode() {
        std::cout << path_node_.size() << std::endl;
        if (path_node_.size() >= 1) {
            return path_node_[1];//just find the next node we are going to go
        }
        return NULL;
    }
    void Maze::Render(glm::mat4 view_matrix, double current_time) {

        // First, render all the nodes in the graph so that they appear on
        // top of the edges
        //
        // Go through each node and render it using the provided game object
        for (int i = 0;i < wall_.size();i++) {
            Node* current_node = wall_[i];
            glm::vec3 pos(current_node->GetX(), current_node->GetY(), 0.0f);
            wall_obj_->SetPosition(pos);
            path_obj_->SetPosition(pos);
            //node_obj_->SetScale(1.0);
            // Set the color of the node via the color modifier uniform
            // The default color is green
            if (current_node->IsWall()) {
                wall_obj_->Render(view_matrix, current_time); // Red
            }
            else {
                path_obj_->Render(view_matrix, current_time); // Red
            }

        }

        // Now, render all the edges in the graph
       
    }


    // Structure used for ranking nodes in the priority queue
    // Declared here as it is only used in the FindPath() method below
    struct QNode {
        Node* node; // Pointer to node
        float cost; // Cost of node
    };


    // Class used for comparing two QNode structures
    class CompareNode
    {
    public:
        int operator() (const QNode& n1, const QNode& n2)
        {
            return n1.cost > n2.cost;
        }
    };



    void Maze::FindPath(void) {

        // Initialize the priority queue used in path finding
        // It is created using the QNode struct with a min compare class called CompareNode
        std::priority_queue<QNode, std::vector<QNode>, CompareNode> pq;
        // Clear current path
        path_node_.clear();
        // Set the costs of all nodes to infinity
        // Reset all nodes to be off-path
        for (int i = 0; i < node_.size(); i++) {
            node_[i]->SetCost(INT_MAX);
            node_[i]->SetOnPath(false);
            node_[i]->SetSearch(false);
        }

        // The start node is added to the priority queue with cost 0
        float estmiate = 0;
        //we know the lowest edge cost is 10, because it is build by 10+rand()
        float lowestEdgeCost = 10;
        //calculate with Manhattan distance
        estmiate = (glm::abs(start_node_->GetX() - end_node_->GetX()) + glm::abs(start_node_->GetY() - end_node_->GetY())) * lowestEdgeCost;
        QNode temp = { start_node_, 0 + estmiate };//+ heuristic(source, target)
        pq.push(temp);

        // Set the cost of the starting node
        start_node_->SetCost(0.0);

        // Now that the pq is initialized, we can start the algorithm
        while (!pq.empty()) {
            // Get the current lowest-cost node in the pq
            QNode lowest = pq.top();

            // Remove the lowest node from the queue after retrieving it
            pq.pop();

            // If the current node is the end node, we are done
            if (lowest.node == end_node_) {
                break;
            }

            // Otherwise, check the neighbors of the lowest node
            for (int i = 0; i < lowest.node->GetNumEdges(); i++) {

                // Compute cost to get to neighbouring node
                // cost = the cost to get the current node + cost to traverse the edge
                const Edge edge = lowest.node->GetEdge(i);
                Node* n = edge.n2;
                //if it is searching, then set it's search to true
                n->SetSearch(true);
                float node_cost = lowest.cost + edge.cost;

                //calculate the new estimate
                estmiate = (glm::abs(n->GetX() - end_node_->GetX()) + glm::abs(n->GetY() - end_node_->GetY())) * lowestEdgeCost;
                std::cout << "estmiate: " << estmiate << std::endl;
                // If the new cost is smaller than the current node cost,
                // update the node cost, and add an updated QNode to the pq
                if (node_cost < n->GetCost()) {
                    // Update node cost
                    n->SetCost(node_cost);


                    // Update the shortest path to the node
                    n->SetPrev(lowest.node);

                    // Add zombie node to update value of node in the queue
                    QNode updated_node = { n, node_cost + estmiate };//change here also
                    pq.push(updated_node);
                }
            }

        }

        // Queue is done, go in reverse from END to START to determine path
        Node* current_node = end_node_->GetPrev();

        // While the current node isn't null, or the end, mark the current
        // node as on the path
        path_node_.push_back(end_node_);
        while (current_node != NULL && current_node != start_node_) {
            path_node_.push_back(current_node);
            current_node->SetOnPath(true);
            current_node = current_node->GetPrev();
        }
        path_node_.push_back(current_node);
        // Reverse path to get the order from start to end
        std::reverse(path_node_.begin(), path_node_.end());

        // Also set the start and end nodes to be on the path for display
        // purposes
        start_node_->SetOnPath(true);
        end_node_->SetOnPath(true);

        // Uncomment to see the ids in order on the path 
        ///for (Node *ele : path_node_) {
        //    std::cout << "id:" << ele->GetId() << std::endl;
        //}/
    }

    void Maze::ResetAll() {
        path_node_.clear();
        // Set the costs of all nodes to infinity
        // Reset all nodes to be off-path
        for (int i = 0; i < node_.size(); i++) {
            node_[i]->SetCost(INT_MAX);
            node_[i]->SetOnPath(false);
            node_[i]->SetSearch(false);
        }
    }

    void Maze::BuildMaze(Maze& output) {

        // Copy all the nodes to the output graph
        for (int i = 0; i < node_.size(); i++) {
            output.AddNode(node_[i]->GetId(), node_[i]->GetX(), node_[i]->GetY());
        }

        // Perform a depth search to add edges to the output graph, creating
        // a maze structure in this process

        // Initialize all nodes as not visited
        for (int i = 0; i < node_.size(); i++) {
            node_[i]->SetVisited(false);
        }

        // Initialize a stack
        std::stack<Node*> st;
        std::vector<Node*> ve;//initialize a vector

        // Add the first node to the stack
        //st.push(node_[0]);

        //add the node to the vector
        ve.push_back(node_[0]);

        node_[0]->SetVisited(true);

        // Depth-first search
        while (ve.size() > 0) {
            // Retrieve top element from the stack
            //Node *n=st.top();
            //random choose an index
            int rand_ele = (rand() % ve.size());
            //get the random node
            Node* n = ve[rand_ele];

            // Create a randomized list of neighbors	
            std::vector<int> index;
            for (int i = 0; i < n->GetNumEdges(); i++) {
                index.push_back(i);
            }
            std::random_shuffle(index.begin(), index.end());

            // Find an unvisited neighbor
            bool found_unvisited = false;
            for (int i = 0; i < index.size(); i++) {
                const Edge edge = n->GetEdge(index[i]);
                Node* neigh = edge.n2;

                if (!neigh->GetVisited()) {
                    // Add connection to unvisited neighbor in the output graph
                    // Assumes the id of a node corresponds to its index
                    Node* n1 = output.GetNode(n->GetId());
                    Node* n2 = output.GetNode(neigh->GetId());
                    n1->AddNeighbor(n2, edge.cost);

                    // Mark node as visited
                    neigh->SetVisited(true);

                    // Add neighbor to the stack
                    //st.push(neigh);

                    //add neighbor to the vector
                    ve.push_back(neigh);

                    // Set flag that a node was found
                    found_unvisited = true;
                    break;
                }
            }

            // If no unvisited neighbor was found, remove the node from the
            // stack
            if (!found_unvisited) {
                //st.pop();
                //if we want to pop the node out in vector, swap it with the final node, and pop it.
                std::swap(ve.back(), ve[rand_ele]);
                ve.pop_back();
            }
        }

        // Set default start and end nodes for output graph
        output.SetStartNode(output.GetNode(0));
        output.SetEndNode(output.GetNode(output.GetNumNodes() - 1));

        // Find shortest path between nodes in the output graph
      //  output.FindPath();
    }

} // namespace game
