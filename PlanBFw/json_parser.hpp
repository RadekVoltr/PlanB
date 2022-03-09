#ifndef SRC_JSON_H_
#define SRC_JSON_H_

#include "configuration.hpp"

#include <ArduinoJson.h>
#include <array>
#include <Arduino.h>
#include <Tween.h>
#include "LittleFS.h" // LittleFS is declared
#include "commands.hpp"
#include "tweens.hpp"



void InitCommandsFromFile(String s);
void InitServosFromFile(String s);
void LoadServoAnimationFromFile(String s);


#endif
