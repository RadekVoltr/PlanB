#include "tank.hpp"
#include "xbox.hpp"

TankClass** Tanks;
int MaxTanks;

int current_tank = 0;

void TankClass::Process(int16_t channels[])
{
    int m1 = 0;
    int m2 = 0;

  if (!failsafe)
  {

    int thr = channels[ChannelA];
    int dir = channels[ChannelB];

 //   Serial.print("Tank output :");
 //   Serial.print("native:");Serial.print(thr);Serial.print(" ");Serial.print(dir);Serial.print("   ");

    if (RevertA)
      thr = map(thr,ANALOG_MIN,ANALOG_MAX,512,-512);
      else
      thr = map(thr,ANALOG_MIN,ANALOG_MAX,-512,512);


    if (RevertB)
      dir = map(dir,ANALOG_MIN,ANALOG_MAX,512,-512);
      else
      dir = map(dir,ANALOG_MIN,ANALOG_MAX,-512,512);
      
   // Serial.print("revert:");Serial.print(thr);Serial.print(" ");Serial.print(dir);Serial.print("   ");


    m1 = thr - dir;
    m2 = thr + dir;

    //Serial.print("calculated:");Serial.print(m1);Serial.print(" ");Serial.print(m2);Serial.print("   ");

    m1 = min(512,m1);
    m1 = max(-512,m1);

    m2 = min(512,m2);
    m2 = max(-512,m2);

  //  Serial.print("normalized:");Serial.print(m1);Serial.print(" ");Serial.print(m2);Serial.print("   ");

    if (Type == servo_driver)
    {
      m1 = map(m1,-512,512,max(MIN_MICROS, DownLimitA), min(MAX_MICROS, TopLimitA)) + TrimA;
      m2 = map(m2,-512,512,max(MIN_MICROS, DownLimitB), min(MAX_MICROS, TopLimitB)) + TrimB;

      //Servos[PwmA].position = m1;
      //Servos[PwmB].position = m2;
      TargetA = m1;
      TargetB = m2;

//      Serial.print(m1);Serial.print(" ");Serial.print(m2);Serial.print("   ");Serial.print(PwmA);Serial.print(" ");Serial.print(PwmB);Serial.print("   ");Serial.println(current_tank);

    }
    else
    if (Type == pwm_driver)
    {
      m1 = map(m1,-512,512,max((int16_t)-100, DownLimitA), min((int16_t)100, TopLimitA)) + TrimA;
      m2 = map(m2,-512,512,max((int16_t)-100, DownLimitB), min((int16_t)100, TopLimitB)) + TrimB;
      TargetA = m1;
      TargetB = m2;

//      Serial.print("final output:");Serial.print(m1);Serial.print(" ");Serial.print(m2);Serial.print("   ");Serial.print(DownLimitA);Serial.print(" ");Serial.print(TopLimitA);Serial.print("   ");Serial.println(current_tank);
    }
}

}

#define SPEED_STEP 20

void TankClass::ProcessFast()
{
  if (failsafe)
  {
    //Serial.println("FAST FAILSAFE !!!!");
    if (Type == servo_driver)
    {
        SetServoMicros(PwmA, DEFAULT_MICROS + TrimA, true);
        SetServoMicros(PwmB, DEFAULT_MICROS + TrimB, true);
    }
    else
    if (Type == pwm_driver)
    {
      SetPWMMotor(PwmA, TrimA);
      SetPWMMotor(PwmB, TrimB);
    }
  }
  else
  {
    if (Type == servo_driver)
    {
        int diff = TargetA - CurrentA;

 //       Serial.print("servos :");Serial.print(TargetA);Serial.print(" ");Serial.print(diff);Serial.print(" ");Serial.print(CurrentA);Serial.print(" ");
   
        if (diff == 0)
          return;
        if (abs(diff) < SPEED_STEP)
          CurrentA = TargetA;
          else
          if (diff < 0)
            CurrentA = CurrentA - SPEED_STEP;
            else
            CurrentA = CurrentA + SPEED_STEP;

      SetServoMicros(PwmA, CurrentA);

        diff = TargetB - CurrentB;

       // Serial.print(TargetB);Serial.print(" ");Serial.print(diff);Serial.print(" ");Serial.print(CurrentB);Serial.print(" ");

        if (diff == 0)
          return;
        if (abs(diff) < SPEED_STEP)
          CurrentB = TargetB;
          else
          if (diff < 0)
            CurrentB = CurrentB - SPEED_STEP;
            else
            CurrentB = CurrentB + SPEED_STEP;

      SetServoMicros(PwmB, CurrentB);

      //Serial.print(CurrentA);Serial.print(" ");Serial.print(CurrentB);Serial.println(" ");
    }
    else
    if (Type == pwm_driver)
    {
        Serial.print("pwm motors :");
        CurrentB = TargetB;
        CurrentA = TargetA;
        SetPWMMotor(PwmA, TargetA);
        SetPWMMotor(PwmB, TargetB);

        Serial.print(PwmA);Serial.print(" ");Serial.print(TargetA);Serial.print(" ");Serial.print(PwmB);Serial.print(" ");Serial.println(TargetB);

    }


  }
}



void ProcessTanks()
{
    for(int i = 0; i < MaxTanks; i++)
        Tanks[i]->Process(communication_data);

}

void ProcessTanksUpdate()
{
    for(int i = 0; i < MaxTanks; i++)
        Tanks[i]->ProcessFast();

}