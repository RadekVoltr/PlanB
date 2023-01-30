#include "motors.hpp"
#include "tweens.hpp"
#include "xbox.hpp"

MotorClass** Motors;
int MaxMotors;

int i = 0;

void MotorClass::Process(int16_t channels[])
{
    int thr = 0;
  if (!failsafe)
  {

    thr = channels[Channel];

    //Serial.print("Motor output :");
    //Serial.print(thr);Serial.print(" ");

    if (Revert)
      thr = map(thr,ANALOG_MIN,ANALOG_MAX,512,-512);
      else
      thr = map(thr,ANALOG_MIN,ANALOG_MAX,-512,512);

    //Serial.print(thr);Serial.print(" ");

  }

    if (Type == servo_driver)
      thr = map(thr,-512,512,max(MIN_MICROS, DownLimit), min(MAX_MICROS, TopLimit)) + Trim;

      Target = thr;
      //Current = Servos[Pwm].position;
      //Serial.print(thr);Serial.print(" ");Serial.print(Current);Serial.print(" ");Serial.print(Pwm);Serial.println(" ");


}

#define SPEED_STEP 10

void MotorClass::ProcessFast()
{
  if (failsafe)
  {
    //Serial.println("FAST FAILSAFE !!!!");
    if (Type == servo_driver)
        SetServoMicros(Pwm, DEFAULT_MICROS, true);
        
  }
  else
  {
    if (Type == servo_driver)
    {
        int diff = Target - Current;
     //   Serial.print(Target);Serial.print(" ");Serial.print(diff);Serial.print(" ");Serial.print(Current);Serial.print(" ");
        if (diff == 0)
          return;
        if (abs(diff) < SPEED_STEP)
          Current = Target;
          else
          if (diff < 0)
            Current = Current - SPEED_STEP;
            else
            Current = Current + SPEED_STEP;

      SetServoMicros(Pwm, Current);


      //Serial.print(Servos[Pwm].position);Serial.print(" ");Serial.print(Pwm);Serial.println(" ");
    }
  }


}


void ProcessMotors()
{
    for(int i = 0; i < MaxMotors; i++)
        Motors[i]->Process(communication_data);

}

void ProcessMotorsUpdate()
{
    for(int i = 0; i < MaxMotors; i++)
        Motors[i]->ProcessFast();

}