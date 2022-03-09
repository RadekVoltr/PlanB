#include "configuration.hpp"
#include "json_parser.hpp"
#include "tweens.hpp"
#include "commands.hpp"

#ifdef SBUS_PIN
  #include "sbus.hpp"
#endif

#ifdef SERVO_COUNT
  #include "servos.hpp"
#endif

#ifdef NEOPIXEL
  #include "neopixel.hpp"
#endif

// Core 0 resource setup,  calculations etc... happens here due timing and simplicity
void setup() {

  Serial.begin(115200);
  
  while(!Serial)
    {};

  Serial.println("init FS");
  

  LittleFS.begin();

  // put your setup code here, to run once:
  Serial.println("init commands");
  InitCommandsFromFile("cmds.jsn");

  #ifdef SERVO_COUNT
  Serial.println("init servos");
  InitServos();

  Serial.println("init servos from file");
  InitServosFromFile("init.jsn");
  #endif

  #ifdef NEOPIXEL
  Serial.println("Neopixel init");
  InitNeoPixels();
  #endif

  SetPixelStrip(0, 1, 0, 0, 255);
  SetPixelStrip(0, 5, 255, 0, 0);

  SetPixelStrip(1, 1, 255, 0, 255);
  SetPixelStrip(1, 3, 0, 255, 255);

  SetPixelStrip(2, 2, 255, 0, 0);
  SetPixelStrip(2, 4, 0, 255, 0);

  Serial.println("setup done");
}

// Core 1 resource setup - will output SBUS and input Neopixel data for fastest/seamless result
void setup1()
{
  #ifdef SBUS_PIN
  Serial.println("SBUS init");
  InitSbus();
  #endif


  #ifdef NEOPIXEL
  Serial.println("Neopixel init");
  InitNeoPixelsProcessing();
  #endif
  
}

void loop() {
  Serial.println("Core 0");
  // put your main code here, to run repeatedly:
  ProcessServoTimeline();

  ProcessSbusRead();

  if (sbus_failsafe)
    Serial.println("FAILSAFE!!!");
  
  if (!sbus_failsafe)
    ProcessCommands();

  delay(100);

}

void loop1()
{
  Serial.println("Core 1");

  #ifdef SBUS_PIN
    ProcessSbusWrite();
  #endif

  ProcessNeoPixels(10); //neopixel processing with integrated delay, not need to call another delay in loop

}
