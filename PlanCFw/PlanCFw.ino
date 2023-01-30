#include "configuration.hpp"
#include "json_parser.hpp"
#include "DFPlayer_Mini_Mp3.hpp"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"
#include "servos.hpp"

#ifdef SBUS_PIN
  #include "sbus.hpp"
#endif

#ifdef SERVO_COUNT
  #include "servos.hpp"
#endif

#ifdef NEOPIXEL
  #include "neopixel.hpp"
#endif

// connect motor controller pins to Arduino digital pins
// motor one
int in1 = 14;
int in2 = 15;
int en1 = 13;
int en2 = 12;

int LED_PIN  = 25;

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


// Core 1 resource setup - will output SBUS and input Neopixel data for fastest/seamless result
void setup1()
{
  /*while(!Serial)
    {};
  */
  #ifdef SBUS_PIN
  Serial.println("Core 1 SBUS init");
  InitSbus();
  #endif


  #ifdef NEOPIXEL
  Serial.println("Core 1 Neopixel init");
  InitNeoPixelsProcessing();
  #endif
  
}

// Core 0 resource setup,  calculations etc... happens here due timing and simplicity
void setup() {

  seed_random_from_rosc();

  Serial.begin(115200);
  /*
  while(!Serial)
    {};
  */
  mp3_set_serial (SERIAL_PIO, 0, DFPLAYER_TX, 9600);
  pinMode(DFPLAYER_BUSY, INPUT);
  delay(100);
  mp3_reset();
  delay(100);
  mp3_set_volume(80);
  delay(100);
  mp3_set_device(2);

  analogWriteFreq(20000);
  analogWriteRange(256);
  
    // set all the motor control pins to outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);

  analogWrite(in1, 0);
  analogWrite(in2, 0);

  digitalWrite(en1, LOW);
  digitalWrite(en2, LOW);
  
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  
  #ifdef NEOPIXEL
  Serial.println("Neopixel init");
  InitNeoPixels();

  Serial.println("Neopixel init done");

  for (int i=0; i<NEOPIXEL_LED_COUNT; i++) {
    Serial.println(i);
    int pix = (rand() % 255);
    SetPixelStrip(0, i, pix, pix, pix);
  }

  #endif
    
  Serial.println("setup done");

  mp3_play_physical_num(START_SOUND);
  
}

void SetSpeed(int spd)
{
  if (abs(spd)<5)
    spd = 0;
  if (spd == 0)
  {
    digitalWrite(en1, LOW);
    digitalWrite(en2, LOW);
  }
  if (spd > 0)
{
    digitalWrite(en1, HIGH);
    digitalWrite(en2, HIGH);
    analogWrite(in1, spd);
    analogWrite(in2, 0);
}
    else
    if (spd < 0)
      {
    digitalWrite(en1, HIGH);
    digitalWrite(en2, HIGH);
     analogWrite(in2, abs(spd));
    analogWrite(in1, 0);
      }
}

bool led = false;

void play_in_range(uint8_t start, uint8_t end)
{
  int song = std::rand() % (end + 1 - start) + start;
  Serial.print("song play :");Serial.println(song);
  if (digitalRead(DFPLAYER_BUSY))
      {
        mp3_play_physical_num(song);
        delay(500);
      }
      else
      Serial.println("Play skip, already playing");

}

int last_sbus[8];

void loop() {
  Serial.println("Core 0");

  ProcessSbusRead();

  if (sbus_failsafe)
    Serial.println("FAILSAFE!!!");
  
  if (!sbus_failsafe)
  {
    Serial.print(sbus_data[0]);Serial.print(" ");Serial.print(sbus_data[4]);Serial.print(" ");Serial.println(sbus_data[5]);

    int thr = map(sbus_data[0],172,1811,-255,255);
    SetSpeed(thr);

    if (last_sbus[4]!=sbus_data[4])
    {
      last_sbus[4] = sbus_data[4];
    }
    else
    if (last_sbus[5]!=sbus_data[5])
    {
      last_sbus[5] = sbus_data[5];
    }
    else
    if (last_sbus[6]!=sbus_data[6])
    {
      last_sbus[6] = sbus_data[6];
    }
    else
    if (sbus_data[4] > 1500)
        play_in_range(GENERAL_SOUND_START, GENERAL_SOUND_END);
        else
        if (sbus_data[4] < 500)
          play_in_range(HAPPY_SOUND_START,HAPPY_SOUND_END);
          else
    if (sbus_data[5] > 1500)
        play_in_range(SAD_SOUND_START, SAD_SOUND_END);
        else
        if (sbus_data[5] < 500)
          play_in_range(MUSIC_SOUND_START,MUSIC_SOUND_END);
          else
    if (sbus_data[6] > 1500)
        play_in_range(LEIA_SOUND_START, LEIA_SOUND_END);
        
  }

  if (led)
      digitalWrite(LED_PIN,HIGH);
      else
      digitalWrite(LED_PIN,LOW);
  led = !led;

  
  for (int i=0; i<NEOPIXEL_LED_COUNT; i++) {
    int pix = (rand() % 255);
    int white_or_blue = (rand() % 100);

    if (white_or_blue < 50)
      SetPixelStrip(0, i, pix, pix, pix);
    else
      SetPixelStrip(0, i, 0, 0, pix);
  }


  delay(100);


}

void loop1()
{
  //Serial.println("Core 1");

  #ifdef SBUS_PIN
    ProcessSbusWrite();
  #endif

  ProcessNeoPixels(10); //neopixel processing with integrated delay, not need to call another delay in loop



}
