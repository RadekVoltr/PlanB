#ifndef SRC_SERVOS_HPP_
#define SRC_SERVOS_HPP_


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

#include <Arduino.h>
#include "configuration.hpp"
#include "communication.hpp"


#ifdef ARDUINO_ARCH_ESP32 


#else

    #if ( defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || \
          defined(ARDUINO_GENERIC_RP2040) ) && !defined(ARDUINO_ARCH_MBED)
      #if !defined(RP2040_ISR_SERVO_USING_MBED)    
        #define RP2040_ISR_SERVO_USING_MBED     false
      #endif  
      
    #elif ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || \
          defined(ARDUINO_GENERIC_RP2040) ) && defined(ARDUINO_ARCH_MBED)
          
      #if !defined(RP2040_ISR_SERVO_USING_MBED)    
        #define RP2040_ISR_SERVO_USING_MBED     true
      #endif  
      
    #else      
      #error This code is intended to run on the mbed / non-mbed RP2040 platform! Please check your Tools->Board setting.
    #endif

    #define TIMER_INTERRUPT_DEBUG       2
    #define ISR_SERVO_DEBUG             2

    #include "RP2040_ISR_Servo.h"
#endif

// Published values for SG90 servos; adjust if needed
#define MIN_MICROS        (int16_t)800
#define MAX_MICROS        (int16_t)2200
#define DEFAULT_MICROS    (int16_t)1500

void OffServos();
void InitServos();
void ProcessServos();
void SetServoMicros(uint8_t index, uint16_t micros , bool Force = false);

void ServoTest();









#endif