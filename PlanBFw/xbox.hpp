#ifndef SRC_XBOX_H_
#define SRC_XBOX_H_

#if defined(ARDUINO_ARCH_ESP32) && defined(XBOX_SX)

#define XBOX_SERIES_X_CONTROLLER_RESET_WHEN_FAILING_CONNECTION
//#define XBOX_SERIES_X_CONTROLLER_DEBUG_SERIAL Serial

#define ANALOG_MIN 0 
#define ANALOG_MAX 1024 

#endif

#endif