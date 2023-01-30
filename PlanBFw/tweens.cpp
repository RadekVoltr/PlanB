#include "tweens.hpp"
#include "servos.hpp"
#include "motors.hpp"



Tween::Timeline ServoTimeline;

void ProcessServoTimeline()
{
    //calculate next frame
    ServoTimeline.update();
}