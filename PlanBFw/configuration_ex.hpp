#ifndef SRC_CONFIGEX_H_
#define SRC_CONFIGEX_H_

////////////////////////// ADVANCED SECTION, DON'T CHANGE UNLESS ARE YOU EXACTLY NOW WHAT YOU ARE DOING :-D

//Motor drivers hardware setup
#define MOTOR_FREQ 20000
//in bits so 8 = 0..255 range
#define MOTOR_RANGE 8

#ifdef ARDUINO_ARCH_ESP32
#define MOTOR_LEDC_CHANEL 0
#define MOTOR_MAX_PINS 33
#define MOTOR_ZERO_RANGE 5

#endif

//queue lenght => 16 prop channels + 1 for failsafe info
//RC format => (WORD)CHANELID_(WORD)VALUE - from Core 1 to Core 0
//Neo Pixel format => (BYTE)LEDID_(RGB 3x BYTE)LEDVALUE - max 255 neopixels driven by system

#define MAX_QUEUE_LENGTH 17


//hw detail for neopixels
#ifdef ARDUINO_ARCH_ESP32
  #define NEOPIXEL_CONFIG NEO_GRB + NEO_KHZ800
  #define NEOPIXEL1_CONFIG NEOPIXEL_CONFIG
  #define NEOPIXEL2_CONFIG NEOPIXEL_CONFIG
  #define NEOPIXEL3_CONFIG NEOPIXEL_CONFIG

#else
#define NEOPIXEL_IS_RGBW false
#define NEOPIXEL_CLOCK 800000

#define NEOPIXEL1_IS_RGBW NEOPIXEL_IS_RGBW
#define NEOPIXEL1_CLOCK NEOPIXEL_CLOCK

#define NEOPIXEL2_IS_RGBW NEOPIXEL_IS_RGBW
#define NEOPIXEL2_CLOCK NEOPIXEL_CLOCK

#define NEOPIXEL3_IS_RGBW NEOPIXEL_IS_RGBW
#define NEOPIXEL3_CLOCK NEOPIXEL_CLOCK
#endif



#endif
