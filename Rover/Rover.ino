#include "configuration.hpp"

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
// motor two
int enB = 6;
int in3 = 8;
int in4 = 7;
int LED_PIN  = 25;

int MotDir[2];
int MotSpeed[2];

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

  Serial.begin(115200);
  /*
  while(!Serial)
    {};
  */
  Serial.println("setup done");

  analogWriteFreq(20000);
  analogWriteRange(256);
  
    // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  MotDir[0] = 0;
  MotDir[1] = 0;

  MotSpeed[0] = 0;
  MotSpeed[1] = 0;
}

volatile bool Setup1Done = false;

void StopAll()
{
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  
  
  MotDir[0] = 0;
  MotDir[1] = 0;

  MotSpeed[0] = 0;
  MotSpeed[1] = 0;  
}

void SetDirection(int motor, int dir)
{
  if (MotDir[motor] == dir)
    return;
  
  MotDir[motor] = dir  ;
  
  if (motor == 0)
  {
    MotSpeed[motor] = 0;
    analogWrite(enA, 0);
    if (dir == 1)
      {
         digitalWrite(in1, HIGH);
         digitalWrite(in2, LOW);
    Serial.print("Motor 0 dir set 1");
      }
      else
      {
         digitalWrite(in1, LOW);
         digitalWrite(in2, HIGH);
    Serial.print("Motor 0 dir set 2");
      }
  }
  
  if (motor == 1)
  {
    MotSpeed[motor] = 0;    
    analogWrite(enB, 0);
    if (dir == 1)
      {
         digitalWrite(in3, HIGH);
         digitalWrite(in4, LOW);
      Serial.print("Motor 1 dir set 1");
      }
      else
      {
         digitalWrite(in3, LOW);
         digitalWrite(in4, HIGH);
      Serial.print("Motor 1 dir set 2");
      }
  }  
  

  
}

void SetSpeed(int motor, int spd)
{
  if (abs(spd)<5)
    spd = 0;
  
  if (spd > 0)
    SetDirection(motor, 1);
    else
    if (spd < 0)
      SetDirection(motor, -1);

  if (motor == 0)
  {
    MotSpeed[motor] = abs(spd);
    analogWrite(enA, MotSpeed[motor]);
    Serial.print("Motor 0 speed set :");Serial.println(MotSpeed[motor]);
  }
  
  if (motor == 1)
  {
    MotSpeed[motor] = abs(spd);        
    analogWrite(enB, MotSpeed[motor]);
    Serial.print("Motor 1 speed set :");Serial.println(MotSpeed[motor]);
  }
}

bool led = false;

void loop() {
  Serial.println("Core 0");

  ProcessSbusRead();
  Serial.println("Core 0 - Sbus Read");

  if (sbus_failsafe)
    Serial.println("FAILSAFE!!!");
  
  if (!sbus_failsafe)
  {
    Serial.print(sbus_data[0]);Serial.print(" ");Serial.print(sbus_data[1]);Serial.print(" ");Serial.print(sbus_data[2]);Serial.print(" ");Serial.println(sbus_data[3]);

    int dir = map(sbus_data[1],172,1811,-255,255);
    int thr = map(sbus_data[2],172,1811,-255,255);

    int m1 = thr - dir;
    int m2 = thr + dir;

    m1 = min(255,m1);
    m1 = max(-255,m1);

    m2 = min(255,m2);
    m2 = max(-255,m2);

    Serial.print(m1);Serial.print(" ");Serial.println(m2);
    SetSpeed( 0,m1);
    SetSpeed( 1,m2);
  }

  if (led)
      digitalWrite(LED_PIN,HIGH);
      else
      digitalWrite(LED_PIN,LOW);
  led = !led;


  delay(100);


}

void loop1()
{
  Serial.println("Core 1");

  #ifdef SBUS_PIN
    ProcessSbusWrite();
  #endif

  Setup1Done = true;

  delay(10);

}
