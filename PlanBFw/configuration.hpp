#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_


#define NEOPIXEL_COUNT 0

//#define NEOPIXEL_PIN 20
//#define NEOPIXEL_LED_COUNT 8

//#define NEOPIXEL1_PIN 21
//#define NEOPIXEL1_LED_COUNT 7

//#define NEOPIXEL2_PIN 19
//#define NEOPIXEL2_LED_COUNT 7



//devices configuration, differes on ESP32 and Pico
#ifdef ARDUINO_ARCH_ESP32


  //XBOX S/X is supported on ESP32 only
  #define XBOX_SX
  #define XBOX_PAIRING_PIN  13
  #define CHANNELS 22
  #define XBOX_AUTO_PAIR_TIMEOUT 20000

#else
  //SBUS is supported on Pico only
  #define CHANNELS 18
  #define SBUS_SERIAL Serial1
  #define SBUS_PIN 1

  #define NEOPIXEL_PIO pio0

  #define I2C_SDA  16
  #define I2C_SCL  17

  #define DFPLAYER_TX 4
  #define DFPLAYER_BUSY 3

  #define UART_TX 2
  #define UART_RX 5

#endif

#define DOME_A 6
#define DOME_B 7
#define DOME_PWM 8

#define U_SENSE 26
#define I_SENSE 27
#define ADC_SENSE 28

//16 servos is maximum
#define SERVO_COUNT 4
#define SERVO_1 13
#define SERVO_2 12
#define SERVO_3 14
#define SERVO_4 27
/*#define SERVO_5 26
#define SERVO_6 25
#define SERVO_7 4
#define SERVO_8 2
#define SERVO_9 -1
#define SERVO_10 -1
#define SERVO_11 -1
#define SERVO_12 -1
#define SERVO_13 -1
#define SERVO_14 -1
#define SERVO_15 -1
#define SERVO_16 -1
*/

//6 motors max
#define MOTOR_PWM_FREQUENCY 25000

#define MOTOR1_PWM_A 4
#define MOTOR1_PWM_B 2 //optional for bi-directional drivers
#define MOTOR1_PWM_E 15 //optional for break/enabled drivers
#define MOTOR1_PWM_BREAK false //optional for break capable drivers (EN LOW => free wheeling)

#define MOTOR2_PWM_A 17
#define MOTOR2_PWM_B 5 //optional for bi-directional drivers
#define MOTOR2_PWM_E 16 //optional for break/enabled drivers
#define MOTOR2_PWM_BREAK false //optional for break capable drivers (EN LOW => free wheeling)


#endif
