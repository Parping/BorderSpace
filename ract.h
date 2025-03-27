


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>


class Ract {
	/*
	Circle is an object which the spaceship object have.
	It standfore the collided area
	If the gameobject has a circle, then it can be collided
	*/
private:
	float width_,height_;//x y is 2D position value, r is the circle radius
	glm::mat4 parent_transformation_matrix_;
	

public:
	// Constructor and destructor
	Ract(void);
	Ract(float w, float h);
	~Ract();
	void SetParent_Transformation_matrix(glm::mat4 t) { parent_transformation_matrix_ = t; };


	void SetWH(glm::vec2 v) { width_ = v.x;height_ = v.y; };
	//geters
	float get_W() { return width_; };
	float get_H() { return height_; };

	glm::vec2 get_My_Center() ;
	
	glm::vec2 get_point(int index);
	//Printer
	void Print(void);
};