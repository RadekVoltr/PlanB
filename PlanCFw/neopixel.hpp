#ifndef SRC_NEOPIX_H_
#define SRC_NEOPIX_H_

#include "configuration.hpp"
#include "core_queue.hpp"

#include "hardware/pio.h"
#include "ws2812.pio.h"


void InitNeoPixels();
void InitNeoPixelsProcessing();


void SetPixelStrip(uint8_t strip, uint8_t index,uint8_t r, uint8_t g, uint8_t b);


void ProcessNeoPixels(int Delay);









#endif