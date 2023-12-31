#ifndef SRC_SERVOS_HPP_
#define SRC_SERVOS_HPP_

#include "configuration.hpp"
#include <Arduino.h>
/****************************************************************************************************************************
  RP2040_MultipleRandomServos.ino
  For :
  - MBED RP2040-based boards such as Nano_RP2040_Connect, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040.
  - RP2040-based boards such as RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040 using arduino_pico core
  
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/RP2040_ISR_Servo
  Licensed under MIT license

  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy
*****************************************************************************************************************************/

/****************************************************************************************************************************
   From ESP32 Servo Example Using Arduino ESP32 Servo Library
   John K. Bennett
   March, 2017

   Different servos require different pulse widths to vary servo angle, but the range is
   an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
   sweep 180 degrees, so the lowest number in the published range for a particular servo
   represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
   of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
   1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
   degrees.

   - Circuit:
   Servo motors have three wires: power, ground, and signal. The power wire is typically red,
   the ground wire is typically black or brown, and the signal wire is typically yellow,
   orange or white. Since the RP2040 can supply limited current at only 3.3V, and servos draw
   considerable power, we will connect servo power to the VBat pin of the RP2040 (located
   near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS.

   We could also connect servo power to a separate external power source (as long as we connect all of 
   the grounds (RP2040, servo, and external power).
   In this example, we just connect RP2040 ground to servo ground. The servo signal pins
   connect to any available GPIO pins on the RP2040 (in this example, we use pins (D1-D6).

   In this example, we assume four Tower Pro SG90 small servos.
   The published min and max for this servo are 500 and 2400, respectively.
   These values actually drive the servos a little past 0 and 180, so
   if you are particular, adjust the min and max values to match your needs.
   Experimentally, 800 and 2450 are pretty close to 0 and 180.
*****************************************************************************************************************************/


/****************************************************************************************************************************
    PlanB Droid Control
    Radek Voltr
    January, 2022

    Some portion of code is took from RP2040_MultipleRandomServos example

*****************************************************************************************************************************/



// Published values for SG90 servos; adjust if needed
#define MIN_MICROS        800
#define MAX_MICROS        2450
#define DEFAULT_MICROS    1500

typedef struct
{
  int     servoIndex;
  uint8_t servoPin;
  uint16_t position;
} ISR_servo_t;

extern ISR_servo_t Servos[SERVO_COUNT];

void InitServos(RP2040_ISR_Servos* RP2040_ISR_Servos);
void ProcessServos(RP2040_ISR_Servos* RP2040_ISR_Servos);


#endif
