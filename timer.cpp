#include <GLFW/glfw3.h>
#include <iostream>

#include "timer.h"

namespace game {

Timer::Timer(void){
    //Initialize to be 0, 0 stands for the timer is not countint time or finished.
    end = 0;
}


Timer::~Timer(void)
{
}


void Timer::Start(float end_time)
{   
    end = 0;//set the end to 0, so the timer can be reused.
    double current_time = glfwGetTime();//get the current time
    end = end_time + current_time;//set the end time
    

}



bool Timer::Finished(void) const
{   
    double current_time = glfwGetTime();//get the current time
   /// std::cout<<"current: "<<current_time<<" end: "<<end << std::endl;;
    if (end <= current_time) {     //if it is greater or equal to the end time, then it is finished. 
        return true;// or it is not counting time.
    }
    return false;
}

} // namespace game
