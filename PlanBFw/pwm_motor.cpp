#include "pwm_motor.hpp"
#include "configuration.hpp"


#ifdef ARDUINO_ARCH_ESP32
#include "ESP32MotorControl.h"

#ifdef MOTOR1_PWM_A
ESP32MotorControl motors1_3;

#ifndef MOTOR1_PWM_E
  #define MOTOR1_PWM_E 0
#endif
#ifndef MOTOR1_PWM_BREAK
  #define MOTOR1_PWM_BREAK false
#endif
#ifndef MOTOR2_PWM_A
  #define MOTOR2_PWM_A 0
#endif
#ifndef MOTOR2_PWM_B
  #define MOTOR2_PWM_B 0
#endif
#ifndef MOTOR2_PWM_E
  #define MOTOR2_PWM_E 0
#endif
#ifndef MOTOR2_PWM_BREAK
  #define MOTOR2_PWM_BREAK false
#endif

#ifndef MOTOR3_PWM_A
  #define MOTOR3_PWM_A 0
#endif
#ifndef MOTOR3_PWM_B
  #define MOTOR3_PWM_B 0
#endif
#ifndef MOTOR3_PWM_E
  #define MOTOR3_PWM_E 0
#endif
#ifndef MOTOR3_PWM_BREAK
  #define MOTOR3_PWM_BREAK false
#endif

#endif

#ifdef MOTOR4_PWM_B
ESP32MotorControl motors4_6(true);

#ifndef MOTOR4_PWM_E
  #define MOTOR4_PWM_E 0
#endif
#ifndef MOTOR4_PWM_BREAK
  #define MOTOR4_PWM_BREAK false
#endif
#ifndef MOTOR5_PWM_A
  #define MOTOR5_PWM_A 0
#endif
#ifndef MOTOR5_PWM_B
  #define MOTOR5_PWM_B 0
#endif
#ifndef MOTOR5_PWM_E
  #define MOTOR5_PWM_E 0
#endif
#ifndef MOTOR5_PWM_BREAK
  #define MOTOR5_PWM_BREAK false
#endif

#ifndef MOTOR5_PWM_A
  #define MOTOR5_PWM_A 0
#endif
#ifndef MOTOR5_PWM_B
  #define MOTOR5_PWM_B 0
#endif
#ifndef MOTOR5_PWM_E
  #define MOTOR5_PWM_E 0
#endif
#ifndef MOTOR5_PWM_BREAK
  #define MOTOR5_PWM_BREAK false
#endif

#endif


#endif



void InitPWMMotors()
{
  #ifdef ARDUINO_ARCH_ESP32

  #ifdef MOTOR1_PWM_A
    motors1_3.attachMotors(MOTOR1_PWM_A,MOTOR1_PWM_B,MOTOR1_PWM_E,MOTOR2_PWM_A,MOTOR2_PWM_B,MOTOR2_PWM_E,MOTOR3_PWM_A,MOTOR3_PWM_B,MOTOR3_PWM_E,MOTOR_PWM_FREQUENCY);
    motors1_3.motorsBreakSet(MOTOR1_PWM_BREAK,MOTOR2_PWM_BREAK,MOTOR3_PWM_BREAK);
  #endif

  #ifdef MOTOR4_PWM_B
    motors4_6.attachMotors(MOTOR4_PWM_A,MOTOR4_PWM_B,MOTOR4_PWM_E,MOTOR5_PWM_A,MOTOR5_PWM_B,MOTOR5_PWM_E,MOTOR6_PWM_A,MOTOR6_PWM_B,MOTOR6_PWM_E,MOTOR_PWM_FREQUENCY);
    motors4_6.motorsBreakSet(MOTOR4_PWM_BREAK,MOTOR5_PWM_BREAK,MOTOR6_PWM_BREAK);
  #endif

  #endif
}


void SetPWMMotor(uint8_t Motor, int8_t Speed)
{
  #ifdef ARDUINO_ARCH_ESP32

  #ifdef MOTOR1_PWM_A
  if (Motor>=0 && Motor<3)
    motors1_3.setMotor_(Motor, Speed);
  #endif

  #ifdef MOTOR4_PWM_B
  if (Motor>2 && Motor<6)
    motors4_6.setMotor_(Motor, Speed);
  #endif

  #endif

}
