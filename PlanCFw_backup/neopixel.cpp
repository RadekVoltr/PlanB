#include <Arduino.h>
#include "neopixel.hpp"

_CoreQueue NeoPixelQueue[NEOPIXEL_COUNT];

//memory cache for storing data. It is needed as all strip data needs to be sent at once and we are not using library with same cache inside
uint32_t NeoPixelStrip[NEOPIXEL_COUNT][NEOPIXEL_LED_COUNT]; 
int NeoPixelSm[NEOPIXEL_COUNT];


//255 LEDs takes about 10ms to be send so two strips takes about 20ms which is less than delay between SBUS frames (readed on same Core)
void InitNeoPixels()
{
  for (int i =0; i < NEOPIXEL_COUNT; i++)
    NeoPixelQueue[i].begin(16);

}

void InitNeoPixelsProcessing()
{

  uint offset = pio_add_program(NEOPIXEL_PIO, &ws2812_program);
  for (int i =0; i < NEOPIXEL_COUNT; i++)
    NeoPixelSm[i] = pio_claim_unused_sm(NEOPIXEL_PIO, true);

  if (NEOPIXEL_COUNT>0)
    ws2812_program_init(NEOPIXEL_PIO, NeoPixelSm[0], offset, NEOPIXEL, NEOPIXEL_CLOCK, NEOPIXEL_IS_RGBW);
  #ifdef NEOPIXEL1
    ws2812_program_init(NEOPIXEL_PIO, NeoPixelSm[1], offset, NEOPIXEL1, NEOPIXEL1_CLOCK, NEOPIXEL1_IS_RGBW);
  #endif
  #ifdef NEOPIXEL2
    ws2812_program_init(NEOPIXEL_PIO, NeoPixelSm[2], offset, NEOPIXEL2, NEOPIXEL2_CLOCK, NEOPIXEL2_IS_RGBW);
    
  #endif
  #ifdef NEOPIXEL3
    ws2812_program_init(NEOPIXEL_PIO, NeoPixelSm[3], offset, NEOPIXEL3, NEOPIXEL3_CLOCK, NEOPIXEL3_IS_RGBW);
    
  #endif
}


static inline void put_pixel(PIO pio, int sm, uint32_t pixel_ggb) {
    pio_sm_put_blocking(pio, sm, pixel_ggb << 8u);
}

static inline uint32_t urgb_u32(uint8_t index,uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            ((uint32_t) (index) << 24) |
            (uint32_t) (b);
}


inline bool GetPixelFromQueue(uint8_t strip)
{
          uint32_t read;
        if (!NeoPixelQueue[strip].pop_nb(&read))
          return false;

        uint8_t index = read >> 24;

        read &= ~0xFF000000;
        if (index < NEOPIXEL_LED_COUNT)
          NeoPixelStrip[strip][index] = read;

        return true;
}


void SetPixelStrip(uint8_t strip, uint8_t index,uint8_t r, uint8_t g, uint8_t b)
{
  NeoPixelQueue[strip].push(urgb_u32(index,r,g,b));
}


void ProcessNeoPixels(int Delay)
{
  auto last_millis = millis();

  for (int s =0; s < NEOPIXEL_COUNT; s++)
  for (int i =0; i < NEOPIXEL_LED_COUNT; i++)
  {
      if (NeoPixelQueue[s].available()) GetPixelFromQueue(s); //this should be fast enough to keep timing ok -> means that we need to fit under 50us or neopixel resets and start from begining

      put_pixel(NEOPIXEL_PIO, NeoPixelSm[s], NeoPixelStrip[s][i]);
  }

  do {
  for (int s =0; s < NEOPIXEL_COUNT; s++)
    while (NeoPixelQueue[s].available())
      if (!GetPixelFromQueue(s))
        break;
    delay(1);
  }
  while (last_millis + Delay > millis());


}
