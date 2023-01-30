#ifndef SRC_TWEENS_H_
#define SRC_TWEENS_H_

#include "configuration.hpp"

#include <Tween.h>
#include "servos.hpp"


extern Tween::Timeline ServoTimeline;


enum EasingType {Hold, Linear, QuartIn, QuartOut, QuartInOut,QuintIn, QuintOut, QuintInOut,SineIn, SineOut, SineInOut,
                 BackIn, BackOut, BackInOut,BounceIn, BounceOut, BounceInOut,CircIn, CircOut, CircInOut,CubicIn,
                 CubicOut, CubicInOut,ElasticIn, ElasticOut, ElasticInOut,ExpoIn, ExpoOut, ExpoInOut,QuadIn, QuadOut,
                 QuadInOut};


void ProcessServoTimeline();



#endif
