#include "sbus.hpp"

bfs::SbusRx sbus_rx(&SBUS_SERIAL);
/* Array for storing SBUS data */
int16_t sbus_data[SBUS_NUM_CH];
volatile bool sbus_failsafe = true;
volatile bool sbus_lostframe = true;
spin_lock_t *sbus_spinlock;

//#define SBUS_DEBUG_WRITE

void InitSbus()
{
    auto spinlock_num = spin_lock_claim_unused(true) ;
    sbus_spinlock = spin_lock_init(spinlock_num) ;

    
    sbus_rx.Begin();
    gpio_set_inover(SBUS_PIN, GPIO_OVERRIDE_INVERT);
}

bool ProcessSbusWrite()
{
  auto res = sbus_rx.Read();

  if (res)
    {
    bfs::SbusData data = sbus_rx.data();
    sbus_failsafe = data.failsafe;
    sbus_lostframe = data.lost_frame;
    

    if (sbus_failsafe || sbus_lostframe)
      {
        Serial.println("SBUS packet skip");
        return false;
      }

    for (int8_t i = 0; i < data.NUM_CH; i++)
    if (data.ch[i]<172 || data.ch[i]>1811)
            {
              Serial.println("SBUS packet invalid");
              return false;
            }

    spin_lock_unsafe_blocking(sbus_spinlock) ;
    for (int8_t i = 0; i < data.NUM_CH; i++)
    { 
      sbus_data[i] = data.ch[i];
    }
    sbus_data[data.NUM_CH + 1] = data.ch17;
    sbus_data[data.NUM_CH + 2] = data.ch18;

    spin_unlock_unsafe(sbus_spinlock) ;


    #ifdef SBUS_DEBUG_WRITE
    Serial.println();
    /* Display the received data */
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      Serial.print(data.ch[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    Serial.print(data.lost_frame);
    Serial.print("\t");
    Serial.println(data.failsafe);
    #endif
    }
    else
      sbus_failsafe = true;

  return res;
}

uint32_t last_millis = 0;
uint32_t q_read = 0;
