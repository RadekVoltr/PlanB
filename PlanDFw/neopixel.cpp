#include <Arduino.h>
#include "neopixel.hpp"

#ifdef NEOPIXEL_COUNT

//_CoreQueue NeoPixelQueue[NEOPIXEL_COUNT];

#ifdef ARDUINO_ARCH_ESP32
  #ifdef NEOPIXEL_PIN
  Adafruit_NeoPixel pixels(NEOPIXEL_LED_COUNT, NEOPIXEL_PIN, NEOPIXEL_CONFIG);
  #endif
  #ifdef NEOPIXEL1_PIN
  Adafruit_NeoPixel pixels1(NEOPIXEL1_LED_COUNT, NEOPIXEL1_PIN, NEOPIXEL1_CONFIG);
  #endif
  #ifdef NEOPIXEL2_PIN
  Adafruit_NeoPixel pixels2(NEOPIXEL2_LED_COUNT, NEOPIXEL2_PIN, NEOPIXEL2_CONFIG);
  #endif
  #ifdef NEOPIXEL3_PIN
  Adafruit_NeoPixel pixels3(NEOPIXEL3_LED_COUNT, NEOPIXEL3_PIN, NEOPIXEL3_CONFIG);
  #endif


#else
//memory cache for storing data. It is needed as all strip data needs to be sent at once and we are not using library with same cache inside
  #ifdef NEOPIXEL_PIN
  uint32_t NeoPixelStrip[NEOPIXEL_LED_COUNT]; 
  #endif
  #ifdef NEOPIXEL1_PIN
  uint32_t NeoPixelStrip1[NEOPIXEL1_LED_COUNT]; 
  #endif
  #ifdef NEOPIXEL2_PIN
  uint32_t NeoPixelStrip2[NEOPIXEL2_LED_COUNT]; 
  #endif
  #ifdef NEOPIXEL3_PIN
  uint32_t NeoPixelStrip3[NEOPIXEL3_LED_COUNT]; 
  #endif
  int NeoPixelSm[NEOPIXEL_COUNT];
#endif

  int NeoPixelCounts[NEOPIXEL_COUNT];


//255 LEDs takes about 10ms to be send so two strips takes about 20ms which is less than delay between SBUS frames (readed on same Core)
void InitNeoPixels()
{
 /* for (int i =0; i < NEOPIXEL_COUNT; i++)
    NeoPixelQueue[i].begin(16);
*/
}

void InitNeoPixelsProcessing()
{

  Serial.println("InitNeoPixelsProcessing");

#ifdef ARDUINO_ARCH_ESP32
  #ifdef NEOPIXEL_PIN
    pixels.begin();
    NeoPixelCounts[0] = NEOPIXEL_LED_COUNT;
  #endif
  #ifdef NEOPIXEL1_PIN
    pixels1.begin();
    NeoPixelCounts[1] = NEOPIXEL1_LED_COUNT;
  #endif
  #ifdef NEOPIXEL2_PIN
    pixels2.begin();
    NeoPixelCounts[2] = NEOPIXEL2_LED_COUNT;
  #endif
  #ifdef NEOPIXEL3_PIN
    pixels3.begin();
    NeoPixelCounts[3] = NEOPIXEL3_LED_COUNT;
  #endif


#else
  Serial.println("Pio Add Program");

  uint offset = pio_add_program(NEOPIXEL_PIO, &ws2812_program);

  Serial.print("Init offset:");Serial.println(offset);



  for (int i =0; i < NEOPIXEL_COUNT; i++)
  {
    NeoPixelSm[i] = pio_claim_unused_sm(NEOPIXEL_PIO, true);

    Serial.print("unused pio ");Serial.println(NeoPixelSm[i]);
  }

    #ifdef NEOPIXEL_PIN
    ws2812_program_init(NEOPIXEL_PIO, NeoPixelSm[0], offset, NEOPIXEL_PIN, NEOPIXEL_CLOCK, NEOPIXEL_IS_RGBW);
    NeoPixelCounts[0] = NEOPIXEL_LED_COUNT;
    
    Serial.println("neo0 init");

    #endif
  #ifdef NEOPIXEL1_PIN
    ws2812_program_init(NEOPIXEL_PIO, NeoPixelSm[1], offset, NEOPIXEL1_PIN, NEOPIXEL1_CLOCK, NEOPIXEL1_IS_RGBW);
    NeoPixelCounts[1] = NEOPIXEL1_LED_COUNT;
    Serial.println("neo1 init");
  #endif
  #ifdef NEOPIXEL2_PIN
    ws2812_program_init(NEOPIXEL_PIO, NeoPixelSm[2], offset, NEOPIXEL2_PIN, NEOPIXEL2_CLOCK, NEOPIXEL2_IS_RGBW);
        NeoPixelCounts[2] = NEOPIXEL2_LED_COUNT;

  #endif
  #ifdef NEOPIXEL3_PIN
    ws2812_program_init(NEOPIXEL_PIO, NeoPixelSm[3], offset, NEOPIXEL3_PIN, NEOPIXEL3_CLOCK, NEOPIXEL3_IS_RGBW);
        NeoPixelCounts[3] = NEOPIXEL3_LED_COUNT;

  #endif
#endif
}

#ifndef ARDUINO_ARCH_ESP32

static inline void put_pixel(PIO pio, int sm, uint32_t pixel_ggb) {
    pio_sm_put_blocking(pio, sm, pixel_ggb << 8u);
}

#endif

static inline uint32_t urgb_u32(uint8_t index,uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            ((uint32_t) (index) << 24) |
            (uint32_t) (b);
}
/*
inline bool GetPixelFromQueue(uint8_t strip)
{
          uint32_t read;
        if (!NeoPixelQueue[strip].pop_nb(&read))
          return false;

        uint8_t index = read >> 24;

        read &= ~0xFF000000;

        


        if (index < NeoPixelCounts[strip])
        #ifdef ARDUINO_ARCH_ESP32
          switch (strip)
          {
            case 0:
              #ifdef NEOPIXEL_PIN
              pixels.setPixelColor(index, read);
              #endif
              break;
            case 1:
              #ifdef NEOPIXEL1_PIN
              pixels1.setPixelColor(index, read);
              #endif
              break;
            case 2:
              #ifdef NEOPIXEL2_PIN
              pixels2.setPixelColor(index, read);
              #endif
              break;
            case 3:
              #ifdef NEOPIXEL3_PIN
              pixels3.setPixelColor(index, read);
              #endif
              break;
            default:
              break;
          }
        #else
          switch (strip)
          {
            case 0:
              #ifdef NEOPIXEL_PIN
                NeoPixelStrip[index] = read;
              #endif
              break;
            case 1:
              #ifdef NEOPIXEL1_PIN
                NeoPixelStrip1[index] = read;
              #endif
              break;
            case 2:
              #ifdef NEOPIXEL2_PIN
                NeoPixelStrip2[index] = read;
              #endif
              break;
            case 3:
              #ifdef NEOPIXEL3_PIN
                NeoPixelStrip3[index] = read;
              #endif
              break;
            default:
              break;
          }
        #endif

        return true;
}

*/
void SetPixelStrip(uint8_t strip, uint8_t index,uint8_t r, uint8_t g, uint8_t b)
{
            switch (strip)
          {
            case 0:
              #ifdef NEOPIXEL_PIN
                NeoPixelStrip[index] = urgb_u32(index,r,g,b);
              #endif
              break;
            case 1:
              #ifdef NEOPIXEL1_PIN
                NeoPixelStrip1[index] = urgb_u32(index,r,g,b);
              #endif
              break;
            case 2:
              #ifdef NEOPIXEL2_PIN
                NeoPixelStrip2[index] = urgb_u32(index,r,g,b);
              #endif
              break;
            case 3:
              #ifdef NEOPIXEL3_PIN
                NeoPixelStrip3[index] = urgb_u32(index,r,g,b);
              #endif
              break;
            default:
              break;
          }
}


void ProcessNeoPixels()
{
 
  #ifdef ARDUINO_ARCH_ESP32
  #ifdef NEOPIXEL_PIN
    pixels.show();
  #endif
  #ifdef NEOPIXEL1_PIN
    pixels1.show();
  #endif
  #ifdef NEOPIXEL2_PIN
    pixels2.show();
  #endif
  #ifdef NEOPIXEL3_PIN
    pixels3.show();
  #endif


  #else
  #ifdef NEOPIXEL_PIN

  for (int i =0; i < NEOPIXEL_LED_COUNT; i++)
  {
      //if (NeoPixelQueue[0].available()) GetPixelFromQueue(0); //this should be fast enough to keep timing ok -> means that we need to fit under 50us or neopixel resets and start from begining
      put_pixel(NEOPIXEL_PIO, NeoPixelSm[0], NeoPixelStrip[i]);

  }
  #endif

  #ifdef NEOPIXEL1_PIN
  for (int i =0; i < NEOPIXEL1_LED_COUNT; i++)
  {
     // if (NeoPixelQueue[1].available()) GetPixelFromQueue(1); //this should be fast enough to keep timing ok -> means that we need to fit under 50us or neopixel resets and start from begining
      put_pixel(NEOPIXEL_PIO, NeoPixelSm[1], NeoPixelStrip1[i]);
  }
  #endif

  #ifdef NEOPIXEL2_PIN
  for (int i =0; i < NEOPIXEL2_LED_COUNT; i++)
  {
      //if (NeoPixelQueue[2].available()) GetPixelFromQueue(2); //this should be fast enough to keep timing ok -> means that we need to fit under 50us or neopixel resets and start from begining
      put_pixel(NEOPIXEL_PIO, NeoPixelSm[2], NeoPixelStrip2[i]);
  }
  #endif

  #ifdef NEOPIXEL3_PIN
  for (int i =0; i < NEOPIXEL3_LED_COUNT; i++)
  {
      //if (NeoPixelQueue[3].available()) GetPixelFromQueue(s); //this should be fast enough to keep timing ok -> means that we need to fit under 50us or neopixel resets and start from begining
      put_pixel(NEOPIXEL_PIO, NeoPixelSm[3], NeoPixelStrip3[i]);
  }
  #endif
  #endif

}


#define FIX_LED_CODE


void ProcessFixedLed()
{

    for (int i=0; i<NEOPIXEL_LED_COUNT; i++) {
    int pix = (rand() % 255);
    int white_or_blue = (rand() % 100);

    if (white_or_blue < 30)
      SetPixelStrip(0, i, pix, pix, pix);
    else
    if (white_or_blue < 60)
      SetPixelStrip(0, i, pix, 0, 0);
    else
      SetPixelStrip(0, i, 0, 0, pix);
    }

}


#endif