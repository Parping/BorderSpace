#include "circle.h"
#include "iostream"


Circle::Circle(void) {
	//Initialize all attributes
	x = 0;
	y = 0;
	r = 0;
}

Circle::Circle(float xp, float yp, float rp) {
	this->x = xp;
	y = yp;
	r = rp;
}
Circle::~Circle() {

}
float Circle:: Area(void) {
	return glm::pi<float>()*  r* r;
}
void Circle::Print(void) {
	//std::cout << x << " " << y << " " << r << std::endl;
}

void Circle::SetRadius(float rp) {
	r = rp;
	////std::cout << "Update radius" << r << std::endl;
}
float Circle::get_x() {
	return x;
}
float Circle::get_y() {
	return y;
}
float Circle::get_r() {
	return r;
}
