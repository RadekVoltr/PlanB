#ifndef SRC_TANK_H_
#define SRC_TANK_H_

#include "configuration.hpp"
#include "communication.hpp"
#include "pwm_motor.hpp"
#include "motors.hpp"
#include "servos.hpp"

#include <Arduino.h>

class TankClass {
  public:
  byte ChannelA;
  byte ChannelB;
  byte PwmA;
  byte PwmB;
  bool RevertA;
  bool RevertB;
  MotorType Type;

  int16_t TopLimitA;
  int16_t DownLimitA;
  int16_t TrimA;

  int16_t TopLimitB;
  int16_t DownLimitB;
  int16_t TrimB;

  int16_t TargetA;
  int16_t CurrentA;

  int16_t TargetB;
  int16_t CurrentB;


  
    TankClass(uint8_t channelA, uint8_t channelB, uint8_t pwmA, uint8_t pwmB, int16_t topLimitA, int16_t downLimitA, int16_t trimA, int16_t topLimitB, int16_t downLimitB, int16_t trimB,bool revertA, bool revertB, MotorType type)
    {
      ChannelA = channelA;
      ChannelB = channelB;
      PwmA = pwmA;
      PwmB = pwmB;
      RevertA = revertA;
      RevertB = revertB;
      Type = type;

      TrimA = trimA;
      TrimB = trimB;

      if (type == servo_driver)
      {

      if (topLimitA <= 0)
        TopLimitA = MAX_MICROS;
      else
        TopLimitA = topLimitA;

      if (downLimitA <= 0 )
        DownLimitA = MIN_MICROS;
      else
        DownLimitA = downLimitA;

      if (topLimitB <= 0)
        TopLimitB = MAX_MICROS;
      else
        TopLimitB = topLimitB;

      if (downLimitB <= 0 )
        DownLimitB = MIN_MICROS;
      else
        DownLimitB = downLimitB;

      TargetA = DEFAULT_MICROS;
      CurrentA = DEFAULT_MICROS;

      TargetB = DEFAULT_MICROS;
      CurrentB = DEFAULT_MICROS;
      }
      else
      {
      if (topLimitA == 0)
      {
        TopLimitA = 100;
        Serial.print("Toplimit to default ");Serial.println(TopLimitA);
      }
      else
      {
        Serial.print("Toplimit to value ");Serial.println(TopLimitA);
        TopLimitA = topLimitA;
      }

      if (downLimitA == 0 )
        DownLimitA = -100;
      else
        DownLimitA = downLimitA;

      if (topLimitB == 0)
        TopLimitB = 100;
      else
        TopLimitB = topLimitB;

      if (downLimitB == 0 )
        DownLimitB = -100;
      else
        DownLimitB = downLimitB;

      Serial.printf("Limits %d %d %d %d \r\n", TopLimitA, DownLimitB, TopLimitB, DownLimitB);

      TargetA = 0;
      CurrentA = 0;

      TargetB = 0;
      CurrentB = 0;

      }


    };
    void Process(int16_t channels[]);
    void ProcessFast();
};

extern TankClass** Tanks;
extern int MaxTanks;


void ProcessTanks();
void ProcessTanksUpdate();

#endif
