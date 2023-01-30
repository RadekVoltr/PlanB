#include <FS.h>
#include "LittleFS.h"

#include "configuration.hpp"
#include "json_parser.hpp"
#include "tweens.hpp"
#include "commands.hpp"
#include "tank.hpp"
#include "motors.hpp"

#ifdef SBUS_PIN
  #include "sbus.hpp"
#endif

#ifdef SERVO_COUNT
  #include "servos.hpp"
#endif

#ifdef NEOPIXEL_COUNT
  #include "neopixel.hpp"
#endif


// Core 0 resource setup,  calculations etc... happens here due timing and simplicity
void setup() {

  failsafe = true;

  //OffServos();

  pinMode(LED_BUILTIN , OUTPUT);
  digitalWrite(LED_BUILTIN , LOW);


  Serial.begin(115200);
  
  /*
  while(!Serial)
   {};
  */

  Serial.println("init FS");
  

  LittleFS.begin(true);

  // put your setup code here, to run once:
  #ifdef SERVO_COUNT
  Serial.println("init servos");
  InitServos();

  Serial.println("init servos from file");
  InitServosFromFile("init.jsn");
  #endif

  Serial.println("init commands");
  InitCommandsFromFile("cmds.jsn");

  Serial.println("init motors");
  InitMotorsFromFile("motors.jsn");

  Serial.println("init tank");
  InitTankMixesFromFile("tank.jsn");

  #ifdef NEOPIXEL_COUNT
  Serial.println("Neopixel init");
  InitNeoPixels();
  #endif

  #ifdef MOTOR1_PWM_A
  InitPWMMotors();
  #endif

  MultiCoreInit();

  Serial.println("setup done");
}

// Core 1 resource setup - will output SBUS and input Neopixel data for fastest/seamless result
void setup1()
{
  failsafe = true;

  #ifdef SBUS_PIN
  Serial.println("SBUS init");
  InitSbus();
  #endif

  #ifdef XBOX_SX
    Serial.println("XBOX Series X init");
    InitXBoxSeriesSX();
  #endif


  #ifdef NEOPIXEL
  Serial.println("Neopixel init");
  InitNeoPixelsProcessing();
  #endif
  
}


int cnt = 0;

void loop() {

  //ServoTest();
  //return;

  cnt++;

  //Serial.print("Core 0"); Serial.println(cnt);
  // put your main code here, to run repeatedly:
  ProcessServoTimeline();

  ProcessMotorsUpdate();
  ProcessTanksUpdate();


  //update servo pwm
  ProcessServos();


  #ifdef SBUS_PIN
  ProcessSbusRead();
  #endif

  /*if (failsafe)
    Serial.println("FAILSAFE!!!");
    */
  
  if (!failsafe)
    {
    //  ProcessCommands();
    }

  if (cnt >= 10)
  {
    //code with internal failsafe processing  
    ProcessTanks();
    ProcessMotors();
    cnt = 0;
  }

  delay(10);

}

void loop1()
{
  //Serial.println("Core 1");

  ProcessRemoteReceive();

  //Serial.printf("XBOX : %d %d %d   ", failsafe, communication_data[0], communication_data[1]);


  ProcessNeoPixels(10); //neopixel processing with integrated delay, not need to call another delay in loop

}
