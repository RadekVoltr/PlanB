#include "tweens.hpp"
#include "servos.hpp"



Tween::Timeline ServoTimeline;

void ProcessServoTimeline()
{
    //calculate next frame
    ServoTimeline.update();
    //update servo pwm
    ProcessServos();

}