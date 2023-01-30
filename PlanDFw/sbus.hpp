#ifndef SRC_SBUS_HPP_
#define SRC_SBUS_HPP_

#include "configuration.hpp"
#include "sbus.h"
#include "hardware/sync.h"


//#define SBUS_DEBUG_WRITE
#define SBUS_DEBUG_READ

#define SBUS_QUEUE_START 0xFFFFFFFF

#define SBUS_NUM_CH 18

extern int16_t sbus_data[SBUS_NUM_CH];

extern volatile bool sbus_failsafe;
extern int sbus_channel_count;
extern volatile bool sbus_lostframe;

extern spin_lock_t *sbus_spinlock;

void InitSbus();
bool ProcessSbusWrite();




#endif
