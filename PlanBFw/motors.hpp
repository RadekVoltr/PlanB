#ifndef SRC_MOTORS_H_
#define SRC_MOTORS_H_

#include "configuration.hpp"
#include "communication.hpp"
#include "servos.hpp"
#include "tweens.hpp"

#include <Arduino.h>


enum MotorType { servo_driver, pwm_driver };

class MotorClass {
  public:
  byte Channel;
  byte Pwm;
  byte Ena;
  int16_t TopLimit;
  int16_t DownLimit;
  int16_t Trim;
  bool Revert;
  MotorType Type;
  int Target;
  u_int16_t Current;
  
    MotorClass(byte channel, byte pwm, byte ena, int16_t topLimit, int16_t downLimit, int16_t trim, bool revert, MotorType type)
    {
      Channel = channel;
      Pwm = pwm;
      Ena = ena;
      Revert = revert;
      Type = type;

      if (Type == servo_driver)
      {

      if (topLimit <= 0)
        TopLimit = MAX_MICROS;
      else
        TopLimit = topLimit;

      if (downLimit <= 0 )
        DownLimit = MIN_MICROS;
      else
        DownLimit = downLimit;
      Trim = trim;
      Target = DEFAULT_MICROS;
      Current = DEFAULT_MICROS;
      }
      else
      {

      if (topLimit <= 0)
        TopLimit = 100;
      else
        TopLimit = topLimit;

      if (downLimit <= 0 )
        DownLimit = -100;
      else
        DownLimit = downLimit;
      Trim = trim;
      Target = 0;
      Current = 0;
      }
    };
    void Process(int16_t channels[]);
    void ProcessFast();
};

extern MotorClass** Motors;
extern int MaxMotors;


void ProcessMotors();
void ProcessMotorsUpdate();
 





#endif