#ifndef SRC_SBUS_HPP_
#define SRC_SBUS_HPP_

#include "configuration.hpp"
#include "communication.hpp"

#ifdef SBUS_PIN
#include "sbus.h"
#include "core_queue.hpp"

//#define SBUS_DEBUG_WRITE
//#define SBUS_DEBUG_READ

#define ANALOG_MIN 172 
#define ANALOG_MAX 1811 


#define SBUS_QUEUE_START 0xFFFFFFFF

extern volatile bool sbus_lostframe;

void InitSbus();
bool ProcessSbusWrite();
void ProcessSbusRead();




#endif
#endif
