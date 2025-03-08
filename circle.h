


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>


class Circle {
	/*
	Circle is an object which the spaceship object have.
	It standfore the collided area
	If the gameobject has a circle, then it can be collided
	*/
private:
	float x, y, r;//x y is 2D position value, r is the circle radius
public:
	// Constructor and destructor
	Circle(void);
	Circle(float xp, float yp, float rp);
	~Circle();
	float Area(void); //area calculated by the radius
	void SetRadius(float rp);//set the radius with different value.
	//geters
	float get_x();
	float get_y();
	float get_r();
	//Printer
	void Print(void);
};