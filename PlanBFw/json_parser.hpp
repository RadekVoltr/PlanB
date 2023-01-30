#ifndef SRC_JSON_H_
#define SRC_JSON_H_

#include "configuration.hpp"

#include <ArduinoJson.h>
#include <array>
#include <Arduino.h>
#include <Tween.h>
#include "commands.hpp"
#include "tweens.hpp"
#include "LittleFS.h"



void InitCommandsFromFile(String s);
void InitServosFromFile(String s);
void LoadServoAnimationFromFile(String s);
void InitTankMixesFromFile(String s);
void InitMotorsFromFile(String s);




#endif
