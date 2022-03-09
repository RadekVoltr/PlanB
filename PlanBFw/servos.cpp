#include "servos.hpp"

ISR_servo_t Servos[SERVO_COUNT] =
{
  { -1, SERVO_1, DEFAULT_MICROS }, { -1, SERVO_2 , DEFAULT_MICROS}, { -1, SERVO_3 , DEFAULT_MICROS}, { -1, SERVO_4 , DEFAULT_MICROS}
, { -1, SERVO_5 , DEFAULT_MICROS}, { -1, SERVO_6 , DEFAULT_MICROS}, { -1, SERVO_7 , DEFAULT_MICROS}, { -1, SERVO_8 , DEFAULT_MICROS}
};


void InitServos()
{

  for (int index = 0; index < SERVO_COUNT; index++)
  {
    pinMode(Servos[index].servoPin, OUTPUT);
    digitalWrite(Servos[index].servoPin, LOW);

    Servos[index].servoIndex = RP2040_ISR_Servos.setupServo(Servos[index].servoPin, MIN_MICROS, MAX_MICROS);

    if (Servos[index].servoIndex != -1)
    {
      Serial.print(F("Setup OK Servo index = ")); Serial.println(Servos[index].servoIndex);

      RP2040_ISR_Servos.setPulseWidth(Servos[index].servoIndex, Servos[index].position);
    }
    else
    {
      Serial.print(F("Setup Failed Servo index = ")); Serial.println(Servos[index].servoIndex);
    }

  }

}


void ProcessServos()
{
  for (int index = 0; index < SERVO_COUNT; index++)
  {
    //Serial.print(index);Serial.print(" ");Serial.print(Servos[index].position);Serial.print(" | ");
    RP2040_ISR_Servos.setPulseWidth(Servos[index].servoIndex, Servos[index].position );
  }
  //Serial.println();
}