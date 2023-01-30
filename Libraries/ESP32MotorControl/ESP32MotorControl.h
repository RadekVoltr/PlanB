/* Header for ESP32MotorControl
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
 * This header file describes the public API for SerialDebug.
 *
 */

#ifndef ESP32MotorControl_H
#define ESP32MotorControl_H

#include "Arduino.h"
#include "driver/mcpwm.h"

#ifndef ESP32
#error "this library is only for ESP32"
#endif

class ESP32MotorControl {
public:
	// Fields:
	uint16_t mMotorSpeed[3] = {0, 0, 0};
	uint8_t mMotorEnable[3] = {0,0,0};
	boolean mMotorForward[3] = {true, true, true};
	uint8_t mMotorBreak[3] = {LOW, LOW, LOW};
	mcpwm_unit_t unit = MCPWM_UNIT_0 ;

	ESP32MotorControl() {     // Constructor
    }

	ESP32MotorControl(bool SecondUnit) {     // Constructor
		unit = MCPWM_UNIT_1;
    }

	// Methods:
	// Attach two motors
	void attachMotors(uint8_t gpioIn0A, uint8_t gpioIn0B, uint8_t gpioEn0, uint8_t gpioIn1A, uint8_t gpioIn1B, uint8_t gpioEn1, uint8_t gpioIn2A, uint8_t gpioIn2B, uint8_t gpioEn2,
	                  uint32_t frequencyHz = 1000);

	void motorsBreakSet(bool Break0, bool Break1, bool Break2);

	// Set speed -> PWM duty in the range 0-100
	void motorForward(uint8_t motor, uint8_t speed);
	void motorReverse(uint8_t motor, uint8_t speed);
	// Set full speed
	void motorFullForward(uint8_t motor);
	void motorFullReverse(uint8_t motor);
	// Stop specific motor
	void motorStop(uint8_t motor);
	// Stop all motors
	void motorsStop();
	// Set speed values of [-100 : 100] to all motors
	void motorsSet(int8_t speed0, int8_t speed1, int8_t speed2);

	uint8_t getMotorSpeed(uint8_t motor);
	boolean isMotorForward(uint8_t motor);
	boolean isMotorStopped(uint8_t motor);

	void setMotor_(uint8_t motor, int8_t speed);


private:
	// Fields:
	boolean mMotorAttached_[3] = {false, false, false};

	// Methods:
	boolean isMotorValid_(uint8_t motor);

	void setMotorNoPWM(mcpwm_unit_t unit, uint8_t motor, int8_t dir);
	void setMotorPWM(mcpwm_unit_t unit, uint8_t motor, int8_t speed);

};

#endif  // ESP32MotorControl_H
