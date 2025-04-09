#include "ract.h"
#include "iostream"


Ract::Ract(void) {
	//Initialize all attributes
	width_ = 0;
	height_ = 0;


}
Ract::Ract(float w, float h) {
	width_ = w;
	height_ = h;



}

Ract::~Ract() {

}
//we need update Ract position and angle with the object
glm::vec2 Ract::get_point(int index) {
    float w = width_/2;
    float h = height_/2;

    glm::vec2 localPoints[6] = {
        {-w, -h}, {w, -h},
        {w, h}, {-w, h},
        {-w,0},{w,0}
    };

    // Set up the parent transformation matrix

    glm::vec4 tran_point = parent_transformation_matrix_ * glm::vec4(localPoints[index], 0.0f, 1.0f);
    glm::vec2 point = glm::vec2(tran_point.x, tran_point.y);
    return point;
	
}
glm::vec2 Ract:: get_My_Center() {

    glm::vec4 world_pos = parent_transformation_matrix_ * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return glm::vec2(world_pos.x, world_pos.y);
}
void Ract::Print(){
    //std::cout << "P1£º " << get_point(0).x << " " << get_point(0).y << std::endl;
    //std::cout << "P2£º " << get_point(1).x << " " << get_point(1).y << std::endl;
    //std::cout << "P3£º " << get_point(2).x << " " << get_point(2).y << std::endl;
    //std::cout << "P4£º " << get_point(3).x << " " << get_point(3).y << std::endl;
    //std::cout << "P5£º " << get_point(4).x << " " << get_point(4).y << std::endl;
    //std::cout << "P6£º " << get_point(5).x << " " << get_point(5).y << std::endl;
    }