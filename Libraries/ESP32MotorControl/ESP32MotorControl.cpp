
/*****************************************
 * Library   : ESP32MotorControl - Library for dual motor driver carrier for Arduino.
 * Programmer: Joao Lopes
 * Comments  : This library is to use with dual motors ICs, as DRV8833, DRV8825 and L298.
 * 			   And uses the MCPWM for control motors
 * Versions  :
 * ------ 	---------- 		-------------------------
 * 0.1.0  	2018-12-26		First beta
 * 0.2.0  	2021-11-21		Revision by Karol Pieniacy
 *****************************************/

/*
 * Source for ESP32MotorControl
 *
 * Copyright (C) 2021  Karol Pieniacy https://github.com/pieniacy/ESP32MotorControl
 *
 * Copyright (C) 2018  Joao Lopes https://github.com/JoaoLopesF/ESP32MotorControl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * This file contains the code for ESP32MotorControl library.
 *
 */

#include "ESP32MotorControl.h"

#include <cmath>

#include "Arduino.h"
#include "esp_attr.h"
#include "esp_system.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

namespace
{
	mcpwm_timer_t MCPWM_TIMER(uint8_t motor)
	{
		mcpwm_timer_t t = motor == 0 ? MCPWM_TIMER_0 : motor == 1 ? MCPWM_TIMER_1
																  : MCPWM_TIMER_2;
		Serial.printf("timer : %d ",t);
		return t;
	}
} // namespace

void ESP32MotorControl::setMotorNoPWM(mcpwm_unit_t unit, uint8_t motor, int8_t dir)
{
	if (dir > 0)
	{
		// Full forward
		digitalWrite(mMotorEnable[motor], HIGH);
		mcpwm_set_signal_low(unit, MCPWM_TIMER(motor), MCPWM_OPR_B);
		mcpwm_set_signal_high(unit, MCPWM_TIMER(motor), MCPWM_OPR_A);
	}
	else if (dir < 0)
	{
		// Full reverse
		digitalWrite(mMotorEnable[motor], HIGH);
		mcpwm_set_signal_low(unit, MCPWM_TIMER(motor), MCPWM_OPR_A);
		mcpwm_set_signal_high(unit, MCPWM_TIMER(motor), MCPWM_OPR_B);
	}
	else
	{
		// Stop
		mcpwm_set_signal_low(unit, MCPWM_TIMER(motor), MCPWM_OPR_A);
		mcpwm_set_signal_low(unit, MCPWM_TIMER(motor), MCPWM_OPR_B);
		digitalWrite(mMotorEnable[motor], mMotorBreak[motor]);
	}
}

void ESP32MotorControl::setMotorPWM(mcpwm_unit_t unit, uint8_t motor, int8_t speed)
{
	if (speed == 0 || speed == -100 || speed == 100)
	{
		setMotorNoPWM(unit, motor, speed);
		return;
	}
	Serial.printf("set pwm %d \r\n", motor);

	digitalWrite(mMotorEnable[motor], HIGH);

	if (speed > 0)
	{
		mcpwm_set_signal_low(unit, MCPWM_TIMER(motor), MCPWM_OPR_B);
		mcpwm_set_duty(unit, MCPWM_TIMER(motor), MCPWM_OPR_A, speed);
		// call the following each time, in case we used mcpwm_set_signal_low/high
		mcpwm_set_duty_type(unit, MCPWM_TIMER(motor), MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
	}
	else
	{
		mcpwm_set_signal_low(unit, MCPWM_TIMER(motor), MCPWM_OPR_A);
		mcpwm_set_duty(unit, MCPWM_TIMER(motor), MCPWM_OPR_B, -speed);
		// call the following each time, in case we used mcpwm_set_signal_low/high
		mcpwm_set_duty_type(unit, MCPWM_TIMER(motor), MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
	}
	Serial.printf("set pwm %d done \r\n", motor);
}

void ESP32MotorControl::attachMotors(uint8_t gpioIn0A, uint8_t gpioIn0B, uint8_t gpioEn0, uint8_t gpioIn1A, uint8_t gpioIn1B, uint8_t gpioEn1, uint8_t gpioIn2A, uint8_t gpioIn2B, uint8_t gpioEn2,
									 uint32_t frequencyHz)
{

	esp_err_t err;
	if (!(gpioIn0A == 0 && gpioIn0B == 0))
	{

		if (gpioEn0 > 0)
		{
			mMotorEnable[0] = gpioEn0;
			pinMode(gpioEn0, OUTPUT);
			digitalWrite(gpioEn0, mMotorBreak[0]);
		}

		// Attach motor 0 input pins.
		err = mcpwm_gpio_init(unit, MCPWM0A, gpioIn0A);
		Serial.println(err);
		err = mcpwm_gpio_init(unit, MCPWM0B, gpioIn0B);
		Serial.println(err);

		Serial.println("timer 0 init");
		// Initial MCPWM configuration
		mcpwm_config_t cfg;
		cfg.frequency = frequencyHz;
		cfg.cmpr_a = 0;
		cfg.cmpr_b = 0;
		cfg.counter_mode = MCPWM_UP_COUNTER;
		cfg.duty_mode = MCPWM_DUTY_MODE_0;

		// Configure PWM0A & PWM0B with above settings
		mcpwm_init(unit, MCPWM_TIMER_0, &cfg);

		// Indicate the motor 0 is attached.
		this->mMotorAttached_[0] = true;
	}

	if (!(gpioIn1A == 0 && gpioIn1B == 0))
	{

		Serial.printf("Pins : %d %d %d \r\n", gpioIn1A, gpioIn1B, gpioEn1);

		if (gpioEn1 > 0)
		{
			mMotorEnable[1] = gpioEn1;
			pinMode(gpioEn1, OUTPUT);
			digitalWrite(gpioEn1, mMotorBreak[1]);
		}
		// Attach motor 1 input pins.
		err = mcpwm_gpio_init(unit, MCPWM1A, gpioIn1A);
		Serial.println(err);

		err = mcpwm_gpio_init(unit, MCPWM1B, gpioIn1B);
		Serial.println(err);

		Serial.println("timer 1 init");
		mcpwm_config_t cfg1;
		cfg1.frequency = frequencyHz;
		cfg1.cmpr_a = 0;
		cfg1.cmpr_b = 0;
		cfg1.counter_mode = MCPWM_UP_COUNTER;
		cfg1.duty_mode = MCPWM_DUTY_MODE_0;
		// Configure PWM1A & PWM1B with above settings
		mcpwm_init(unit, MCPWM_TIMER_1, &cfg1);

		// Indicate the motor 1 is attached.
		this->mMotorAttached_[1] = true;
	}

	if (!(gpioIn2A == 0 && gpioIn2B == 0))
	{
		Serial.println("motor 2 init");
		if (gpioEn2 > 0)
		{
			mMotorEnable[2] = gpioEn2;
			pinMode(gpioEn2, OUTPUT);
			digitalWrite(gpioEn2, mMotorBreak[2]);
		}

		// Attach motor 1 input pins.
		err = mcpwm_gpio_init(unit, MCPWM2A, gpioIn2A);
		Serial.println(err);

		err = mcpwm_gpio_init(unit, MCPWM2B, gpioIn2B);
		Serial.println(err);

		Serial.println("timer 2 init");
		mcpwm_config_t cfg2;
		cfg2.frequency = frequencyHz;
		cfg2.cmpr_a = 0;
		cfg2.cmpr_b = 0;
		cfg2.counter_mode = MCPWM_UP_COUNTER;
		cfg2.duty_mode = MCPWM_DUTY_MODE_0;
		// Configure PWM1A & PWM1B with above settings
		mcpwm_init(unit, MCPWM_TIMER_2, &cfg2);

		// Indicate the motor 2 is attached.
		this->mMotorAttached_[2] = true;
	}
}

void ESP32MotorControl::motorForward(uint8_t motor, uint8_t speed) { setMotor_(motor, speed); }

void ESP32MotorControl::motorReverse(uint8_t motor, uint8_t speed) { setMotor_(motor, -speed); }

void ESP32MotorControl::motorFullForward(uint8_t motor) { setMotor_(motor, 100); }

void ESP32MotorControl::motorFullReverse(uint8_t motor) { setMotor_(motor, -100); }

void ESP32MotorControl::motorStop(uint8_t motor) { setMotor_(motor, 0); }

void ESP32MotorControl::motorsStop()
{
	setMotor_(0, 0);
	setMotor_(1, 0);
	setMotor_(2, 0);
}

void ESP32MotorControl::motorsSet(int8_t speed0, int8_t speed1, int8_t speed2)
{
	setMotor_(0, speed0);
	setMotor_(1, speed1);
	setMotor_(2, speed2);
}

uint8_t ESP32MotorControl::getMotorSpeed(uint8_t motor)
{
	if (!isMotorValid_(motor))
		return false;

	return mMotorSpeed[motor];
}

boolean ESP32MotorControl::isMotorForward(uint8_t motor)
{
	if (!isMotorValid_(motor) || isMotorStopped(motor))
		return false;

	return mMotorForward[motor];
}

boolean ESP32MotorControl::isMotorStopped(uint8_t motor)
{
	if (!isMotorValid_(motor))
		return true;

	return (mMotorSpeed[motor] == 0);
}

void ESP32MotorControl::setMotor_(uint8_t motor, int8_t speed)
{
	Serial.printf("motor %d > %d", motor,speed);
	if (!isMotorValid_(motor) || speed > 100 || speed < -100)
		{
			Serial.println(" not valid");
			return;
		}
	setMotorPWM(unit, motor, speed);

	Serial.println(" speed set");

	mMotorSpeed[motor] = std::abs(speed);
	mMotorForward[motor] = speed > 0;
}

boolean ESP32MotorControl::isMotorValid_(uint8_t motor)
{
	if (motor > 2)
		return false;

	return mMotorAttached_[motor];
}

void ESP32MotorControl::motorsBreakSet(bool Break0, bool Break1, bool Break2)
{
	mMotorBreak[0] = Break0 ? HIGH : LOW;
	mMotorBreak[1] = Break1 ? HIGH : LOW;
	mMotorBreak[2] = Break2 ? HIGH : LOW;
}
