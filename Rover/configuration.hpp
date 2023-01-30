#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

/*#define NEOPIXEL_COUNT 1

#define NEOPIXEL_PIO pio0

#define NEOPIXEL 16
#define NEOPIXEL_LED_COUNT 21

/*#define NEOPIXEL1 21
#define NEOPIXEL1_LED_COUNT 7

#define NEOPIXEL2 19
#define NEOPIXEL2_LED_COUNT 7

#define I2C_SDA  16
#define I2C_SCL  17
*/



#define DFPLAYER_TX 4
#define DFPLAYER_BUSY 3

#define UART_TX 2
#define UART_RX 5

#define SBUS_SERIAL Serial1
#define SBUS_PIN 1
#define SBUS_DEBUG_WRITE

#define DOME_A 6
#define DOME_B 7
#define DOME_PWM 8

#define U_SENSE 26
#define I_SENSE 27
#define ADC_SENSE 28

/*#define SERVO_COUNT 0
#define SERVO_1 15
#define SERVO_2 14
#define SERVO_3 13
#define SERVO_4 12
#define SERVO_5 11
#define SERVO_6 10
#define SERVO_7 9
#define SERVO_8 18

#define START_SOUND 0
#define GENERAL_SOUND_START 1
#define GENERAL_SOUND_END 37
#define HAPPY_SOUND_START 38
#define HAPPY_SOUND_END 44
#define SAD_SOUND_START 45
#define SAD_SOUND_END 48
#define LEIA_SOUND_START 55
#define LEIA_SOUND_END 57
#define MUSIC_SOUND_START 58
#define MUSIC_SOUND_END 63


*/

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
