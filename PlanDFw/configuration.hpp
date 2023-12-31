#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#define NEOPIXEL_COUNT 1

#define NEOPIXEL_PIO pio0

#define NEOPIXEL_PIN 16
#define NEOPIXEL_LED_COUNT 80


/*#define NEOPIXEL1_PIN 22
#define NEOPIXEL1_LED_COUNT 30

/*#define NEOPIXEL2 19
#define NEOPIXEL2_LED_COUNT 7

#define I2C_SDA  16
#define I2C_SCL  17
*/

#define SBUS_SERIAL Serial1
#define SBUS_PIN 1

#define SERVOS_PIO  pio1
#define SERVO_SET_1_PIN 8
#define SERVO_SET_1_COUNT 8
//#define SERVO_SET_2_PIN 15
//#define SERVO_SET_2_COUNT 6


////////////////////////// ADVANCED SECTION, DON'T CHANGE UNLESS ARE YOU EXACTLY NOW WHAT YOU ARE DOING :-D

//queue lenght => 16 prop channels + 1 for failsafe info
//RC format => (WORD)CHANELID_(WORD)VALUE - from Core 1 to Core 0
//Neo Pixel format => (BYTE)LEDID_(RGB 3x BYTE)LEDVALUE - max 255 neopixels driven by system

#define MAX_QUEUE_LENGTH 17


//hw detail for neopixels
#define NEOPIXEL_IS_RGBW false
#define NEOPIXEL_CLOCK 800000

#define NEOPIXEL1_IS_RGBW NEOPIXEL_IS_RGBW
#define NEOPIXEL1_CLOCK NEOPIXEL_CLOCK

/*#define NEOPIXEL2_IS_RGBW NEOPIXEL_IS_RGBW
#define NEOPIXEL2_CLOCK NEOPIXEL_CLOCK

#define NEOPIXEL3_IS_RGBW NEOPIXEL_IS_RGBW
#define NEOPIXEL3_CLOCK NEOPIXEL_CLOCK
*/


#endif
