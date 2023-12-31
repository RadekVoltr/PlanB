
#include "sbus.h"

#include "configuration.hpp"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"

#ifdef SBUS_PIN
  #include "sbus.hpp"
#endif
unsigned long last_packet;


int LED_PIN  = 25;
bool init_done = false;

// Core 0 resource setup,  calculations etc... happens here due timing and simplicity
void setup() {
  last_packet = 0;

  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN, LOW);


  Serial.begin(115200);
  delay(5000);
  
  digitalWrite(LED_PIN, HIGH);

  #ifdef SBUS_PIN
  Serial.println("Core 1 SBUS init");
  InitSbus();
  #endif
  Serial.println("Core 1 SBUS init done");
  Serial.println("setup done");
  Serial.flush();

  init_done = true;
 
}

void setup1()
{

}

bool led = false;

void loop() {
  //Serial.println("Core 0");


  if (sbus_failsafe)
    {
      if (last_packet + 100 < millis())
        {
          //failsafe reset of data
          Serial.println("FAILSAFE reset !!!");
          spin_lock_unsafe_blocking(sbus_spinlock) ;
          for (int8_t i = 0; i < SBUS_NUM_CH; i++)
            sbus_data[i] = 1500;//reset to default RC value center
          spin_unlock_unsafe(sbus_spinlock) ;
        }
    }
    else
    last_packet = millis();
  
  if (!sbus_failsafe)
  {
    for (int i=0;i < SBUS_NUM_CH;i++)
      {
        Serial.print(i);Serial.print(":");Serial.print(sbus_data[i]);Serial.print(" ");
      }
    Serial.println();
  }

  if (led)
      digitalWrite(LED_PIN,HIGH);
      else
      digitalWrite(LED_PIN,LOW);
  led = !led;

  delay(10);
}

void loop1()
{
  if (!init_done)
    {
      Serial.println("init not done!");
      delay(100);
      return;
    }
  
    #ifdef SBUS_PIN
      //Serial.println("SB write");
    ProcessSbusWrite();
      //Serial.println("SB write done");
  #endif
  
  delay(10);
}
