#ifndef SRC_SBUS_HPP_
#define SRC_SBUS_HPP_

#include "configuration.hpp"

#include "sbus.h"
#include "core_queue.hpp"

//#define SBUS_DEBUG_WRITE
#define SBUS_DEBUG_READ

#define SBUS_QUEUE_START 0xFFFFFFFF

extern std::array<int16_t, bfs::SbusRx::NUM_CH() + 2> sbus_data;

extern volatile bool sbus_failsafe;
extern int sbus_channel_count;
extern volatile bool sbus_lostframe;

void InitSbus();
bool ProcessSbusWrite();
void ProcessSbusRead();




#endif

