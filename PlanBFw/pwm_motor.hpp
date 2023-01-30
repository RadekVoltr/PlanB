#ifndef SRC_PWMMOTOR_HPP_
#define SRC_PWMMOTOR_HPP_

#include "configuration.hpp"
#include "configuration_ex.hpp"
#include <Arduino.h>

void InitPWMMotors();
void SetPWMMotor(uint8_t Motor, int8_t Speed);



#endif
