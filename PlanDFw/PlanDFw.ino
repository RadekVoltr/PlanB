
#include "sbus.h"

#include "configuration.hpp"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"

#ifdef SBUS_PIN
  #include "sbus.hpp"
#endif

#ifdef SERVOS_PIO
  #include <pio_servo.hpp>

  PioServo pioServo(SERVOS_PIO);


  
#endif

#ifdef NEOPIXEL_LED_COUNT
  #include "neopixel.hpp"
#endif

// connect motor controller pins to Arduino digital pins
// motor one
int in1 = 14;
int in2 = 15;
int en1 = 13;

int LED_PIN  = 25;
bool init_done = false;

void seed_random_from_rosc()
{
    uint32_t random = 0x811c9dc5;
    uint8_t next_byte = 0;
    volatile uint32_t* rnd_reg = (uint32_t*)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
    

    for (int i = 0; i < 16; i++) {
        for (int k = 0; k < 8; k++) {
            next_byte = (next_byte << 1) | (*rnd_reg & 1);
        }

        random ^= next_byte;
        random *= 0x01000193;
    }
    srand(random);
}


// Core 0 resource setup,  calculations etc... happens here due timing and simplicity
void setup() {

  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN, LOW);

  seed_random_from_rosc();

  Serial.begin(115200);
  //delay(15000);
  
  digitalWrite(LED_PIN, HIGH);

    #ifdef SBUS_PIN
  Serial.println("Core 1 SBUS init");
  InitSbus();
  #endif
  Serial.println("Core 1 SBUS init done");
  

  #ifdef NEOPIXEL_COUNT
  Serial.println("Core 1 Neopixel init");
  InitNeoPixelsProcessing();
  

  Serial.println("Neopixel init done");

  for (int i=0; i<NEOPIXEL_LED_COUNT; i++) {
    Serial.println(i);
    int pix = (rand() % 255);
    SetPixelStrip(0, i, pix, pix, pix);
  }

  #ifdef NEOPIXEL1_LED_COUNT
  for (int i=0; i<NEOPIXEL1_LED_COUNT; i++) {
    Serial.println(i);
    int pix = (rand() % 255);
    SetPixelStrip(1, i, pix, pix, pix);
  }
  #endif

  #endif

  #ifdef SERVOS_PIO

  #ifdef SERVO_SET_1_PIN

  auto blck = pioServo.addServoBlock(SERVO_SET_1_PIN, SERVO_SET_1_COUNT, NULL);
  Serial.printf("addServoblock :%d \r\n", blck);
  Serial.flush();

  if (blck < 0)
    Serial.printf("addServoblock error :%d \r\n", blck);
  else
    pioServo.runServoBlock(blck);

  #endif

  Serial.println("Servo 1 run");
  Serial.flush();

  #ifdef SERVO_SET_2_PIN
  blck = pioServo.addServoBlock(SERVO_SET_2_PIN, SERVO_SET_2_COUNT, NULL);
  Serial.printf("addServoblock :%d \r\n", blck);
  Serial.flush();

  if (blck < 0)
    Serial.printf("addServoblock error :%d \r\n", blck);
  else
    pioServo.runServoBlock(blck);
  #endif

  #endif
  
  Serial.println("setup done");
  Serial.flush();

  init_done = true;
 
}

void setup1()
{

}

bool led = false;

int last_sbus[8];

void ProcessRCServo(uint8_t Channel, uint8_t Servo)
{
  if (sbus_failsafe)
    return;

  spin_lock_unsafe_blocking(sbus_spinlock) ;
  auto value = sbus_data[Channel];
  auto ch15 = sbus_data[14];
  spin_unlock_unsafe(sbus_spinlock) ;
  

  if (value<172 || value>1811)
    return;

  if (ch15 > 1000)
  {
    //Serial.println("servo disabled");
    pioServo.setServoMicros(0,Servo,0); //disable servo
  }
    else
    {
      int uM = map(value,172,1811,1000,2000);
      pioServo.setServoMicros(0,Servo,uM);
    }
}

void loop() {
  //Serial.println("Core 0");

  if (sbus_failsafe)
    Serial.println("FAILSAFE!!!");
  
  if (!sbus_failsafe)
  {
    //Serial.print("SBUS:");Serial.print(sbus_data[15]);Serial.print(" - ");Serial.print(sbus_data[14]);Serial.print(" - ");Serial.print(sbus_data[8]);Serial.print(" - ");Serial.print(sbus_data[1]);Serial.print(" - ");Serial.print(sbus_data[2]);Serial.print(" - ");Serial.print(sbus_data[3]);Serial.print(" - ");Serial.print(sbus_data[0]);Serial.print(" ");Serial.print(sbus_data[4]);Serial.print(" ");Serial.println(sbus_data[5]);

    //utility arms from RC originally
    //ProcessRCServo(1, 9);
    //ProcessRCServo(7, 10);

    ProcessRCServo(8, 0);
    ProcessRCServo(10, 1);
    ProcessRCServo(11, 2);
    ProcessRCServo(9, 3);
    ProcessRCServo(12, 4);
    ProcessRCServo(13, 5);
    ProcessRCServo(15, 6);
    //ProcessRCServo(15, 14);
    //ProcessRCServo(16, 8);

  }

  if (led)
      digitalWrite(LED_PIN,HIGH);
      else
      digitalWrite(LED_PIN,LOW);
  led = !led;

  
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

  delay(100);
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
      Serial.println("SB write");
    ProcessSbusWrite();
      Serial.println("SB write done");
  #endif

  #ifdef NEOPIXEL_COUNT
  Serial.println("Neo write");
  ProcessNeoPixels(); 
  Serial.println("Neo write done");
  #endif

  delay(10);
}
