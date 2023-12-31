#include "sbus.hpp"

bfs::SbusRx sbus_rx(&SBUS_SERIAL);
/* Array for storing SBUS data */
std::array<int16_t, bfs::SbusRx::NUM_CH() + 2> sbus_data;
volatile bool sbus_failsafe = true;
volatile bool sbus_lostframe = true;
int sbus_channel_count = 16;

_CoreQueue SBUSQueue;

void InitSbus()
{
    SBUSQueue.begin(23);
    sbus_rx.Begin();
    gpio_set_inover(SBUS_PIN, GPIO_OVERRIDE_INVERT);
    sbus_channel_count =  bfs::SbusRx::NUM_CH()+2;
}

bool ProcessSbusWrite()
{
  auto res = sbus_rx.Read();
  sbus_failsafe = sbus_rx.failsafe();

  if (res)
    {
    SBUSQueue.clear();
    SBUSQueue.push(SBUS_QUEUE_START);
    SBUSQueue.push(millis());
    SBUSQueue.push((0xFFFF << 16) + (uint16_t)sbus_rx.failsafe());
    SBUSQueue.push((0xFFFE << 16) + (uint16_t)sbus_rx.lost_frame());

    auto sbus_data = sbus_rx.ch();

    for (int8_t i = 0; i < bfs::SbusRx::NUM_CH(); i++) 
      SBUSQueue.push((i << 16) + (uint16_t)sbus_data[i]);

    SBUSQueue.push((17 << 16) + (uint16_t)sbus_rx.ch17());
    SBUSQueue.push((18 << 16) + (uint16_t)sbus_rx.ch18());

    #ifdef SBUS_DEBUG_WRITE
    /* Display the received data */
    for (int8_t i = 0; i < bfs::SbusRx::NUM_CH(); i++) {
      Serial.print(sbus_data[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    Serial.print(sbus_rx.lost_frame());
    Serial.print("\t");
    Serial.println(sbus_rx.failsafe());
    #endif
    }

  return res;
}

uint32_t last_millis = 0;
uint32_t q_read = 0;

void ProcessSbusRead()
{
   if (SBUSQueue.pop_nb(&q_read))
    {
      if (q_read == SBUS_QUEUE_START)
         if (SBUSQueue.pop_nb(&q_read))
            last_millis = q_read; //last millis from queue

    uint16_t id, value;

    while (SBUSQueue.pop_nb(&q_read))
    {
      Serial.print(q_read, HEX);
      id = (q_read >> 16) & 0xFFFF;
      value = q_read & 0xFFFF;
      
      switch (id)
      {
        case 0xFFFF : 
          sbus_failsafe = (bool)value;
          break;
        case 0xFFFE : 
          sbus_lostframe = (bool)value;
          break;
        default : 
          if (id>=0 && id<=sbus_channel_count) sbus_data[id] = value;
          #ifdef SBUS_DEBUG_READ
            Serial.printf(" = %d => %d | ", id, value);
          #endif
      }
    }
          #ifdef SBUS_DEBUG_READ
            Serial.println();
          #endif

    if (last_millis + 500 < millis())
    {
      sbus_failsafe = true;
      #ifdef SBUS_DEBUG_READ
        Serial.println("time failsafe");
      #endif
    }
  }
}

