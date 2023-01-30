#include "servos.hpp"
#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP32
#include "ESP32Servo.h"

typedef struct
{
  uint8_t servoPin;
  bool Loaded;
  Servo *servo;
  uint16_t position;
} Servo_t;

Servo_t Servos[SERVO_COUNT + 1] =
    {
#ifdef SERVO_1
        {SERVO_1, false, nullptr, 0},
#endif
#ifdef SERVO_2
        {SERVO_2, false, nullptr, 0},
#endif
#ifdef SERVO_3
        {SERVO_3, false, new Servo(), 0},
#endif
#ifdef SERVO_4
        {SERVO_4, false, new Servo(), 0},
#endif
#ifdef SERVO_5
        {SERVO_5, false, new Servo(), 0},
#endif
#ifdef SERVO_6
        {SERVO_6, false, new Servo(), 0},
#endif
#ifdef SERVO_7
        {SERVO_7, false, new Servo(), 0},
#endif
#ifdef SERVO_8
        {SERVO_8, false, new Servo(), 0},
#endif
#ifdef SERVO_9
        {SERVO_9, false, new Servo(), 0},
#endif
#ifdef SERVO_10
        {SERVO_10, false, new Servo(), 0},
#endif
#ifdef SERVO_11
        {SERVO_11, false, new Servo(), 0},
#endif
#ifdef SERVO_12
        {SERVO_12, false, new Servo(), 0},
#endif
#ifdef SERVO_13
        {SERVO_13, false, new Servo(), 0},
#endif
#ifdef SERVO_14
        {SERVO_14, false, new Servo(), 0},
#endif
#ifdef SERVO_15
        {SERVO_15, false, new Servo(), 0},
#endif
#ifdef SERVO_16
        {SERVO_16, false, new Servo(), 0},
#endif
        {0, false, nullptr}

};

#else
typedef struct
{
  int servoIndex;
  uint8_t servoPin;
  uint16_t position;
} ISR_servo_t;

ISR_servo_t Servos[SERVO_COUNT + 1] =
    {
#ifdef SERVO_1
        {-1, SERVO_1, 0},
#endif
#ifdef SERVO_2
        {-1, SERVO_2, 0},
#endif
#ifdef SERVO_3
        {-1, SERVO_3, 0},
#endif
#ifdef SERVO_4
        {-1, SERVO_4, 0},
#endif
#ifdef SERVO_5
        {-1, SERVO_5, 0},
#endif
#ifdef SERVO_6
        {-1, SERVO_6, 0},
#endif
#ifdef SERVO_7
        {-1, SERVO_7, 0},
#endif
#ifdef SERVO_8
        {-1, SERVO_8, 0}
#endif
#ifdef SERVO_9
        {-1, SERVO_9, 0},
#endif
#ifdef SERVO_10
        {-1, SERVO_10, 0},
#endif
#ifdef SERVO_11
        {-1, SERVO_11, 0},
#endif
#ifdef SERVO_12
        {-1, SERVO_12, 0},
#endif
#ifdef SERVO_13
        {-1, SERVO_13, 0},
#endif
#ifdef SERVO_14
        {-1, SERVO_14, 0},
#endif
#ifdef SERVO_15
        {-1, SERVO_15, 0},
#endif
#ifdef SERVO_16
        {-1, SERVO_16, 0},
#endif
        {0, 0, 0}

};

#define SERVOS RP2040_ISR_Servos
#endif

void OffServos()
{
  for (int index = 0; index < SERVO_COUNT; index++)
  {
    pinMode(Servos[index].servoPin, OUTPUT);
    digitalWrite(Servos[index].servoPin, LOW);
  }
}

void InitServos()
{

#ifdef ARDUINO_ARCH_ESP32
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  Serial.println("Servo timers allocated");
  for (size_t i = 0; i < SERVO_COUNT; i++)
  {
    Serial.print("Init servo :");Serial.println(i);
    Servos[i].servo = new Servo();
    Servos[i].servo->setPeriodHertz(50);
    Serial.println("Servo ok");
  }
#else
  for (int index = 0; index < SERVO_COUNT; index++)
  {
    Serial.print("Servo :");
    Serial.print(index);
    Serial.print(" Pin:");
    Serial.println(Servos[index].servoPin);

    Servos[index].servoIndex = SERVOS.setupServo(Servos[index].servoPin, MIN_MICROS, MAX_MICROS);

    Serial.println("setupt servo ok");
  }
  else
  {
    Serial.println("Setup Failed Servo index = ");
    Serial.println(Servos[index].servoIndex);
  }
}
#endif
}

void ProcessServos()
{
  for (int index = 0; index < SERVO_COUNT; index++)
  {
    uint16_t pos = Servos[index].position;
    if (pos == 0)
      continue;

#ifdef ARDUINO_ARCH_ESP32

    if (!Servos[index].Loaded)
    {
      Servos[index].servo->attach(Servos[index].servoPin, MIN_MICROS, MAX_MICROS);
      Servos[index].Loaded = true;
    }

    Servos[index].servo->writeMicroseconds(Servos[index].position);
#else

    // if (SERVOS.getPulseWidth(Servos[index].servoIndex) != pos)
    {
      Serial.print(index);
      Serial.print(" ");
      Serial.print(pos);
      Serial.println();
      // SERVOS.setPulseWidth(Servos[index].servoIndex, pos );
    }

#endif
  }
  // Serial.println();
}

void ServoTest()
{

  for (int index = 0; index < SERVO_COUNT; index++)
    for (uint16_t pos = MIN_MICROS; pos < MAX_MICROS; pos = pos + 50)
    {
      Serial.print(SERVO_COUNT);
      Serial.print(" ");
      Serial.print(index);
      Serial.print(" ");
      Serial.print(" ");
      Serial.print(pos);
      Serial.println();

      // SERVOS.setPulseWidth(Servos[index].servoIndex, pos );
      delay(10);
    }
}

void SetServoMicros(uint8_t index, uint16_t micros, bool Force)
{
  if (index >= SERVO_COUNT)
    return;

#ifdef ARDUINO_ARCH_ESP32
  Servos[index].position = micros;
  if (Force)
    Servos[index].servo->writeMicroseconds(micros);

#else

#endif
}
