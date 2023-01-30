#ifndef SRC_COMMANDS_H_
#define SRC_COMMANDS_H_

#include "configuration.hpp"
#include "communication.hpp"

#include <Arduino.h>

#define CMD_DEBUG


class CommandClass {
  public:
  bool IsActive;
  byte Channel1;
  byte Channel2;
  uint16_t StartRange1;
  uint16_t EndRange1;
  uint16_t StartRange2;
  uint16_t EndRange2;
  String FileName;
  String Sound;
  
    CommandClass() {
      Channel1 = 0;
      Channel2 = 0;
      StartRange1 = 0;
      EndRange1 = 2000;
      StartRange2 = 0;
      EndRange2 = 2000;
      IsActive = false;
      FileName = "";
      Sound = "";
    };
    bool Validate(int16_t channels[], bool OneTime) {
      
      bool valid = false;
        if (Channel1 > 0)
          {
            int16_t Ch1 = channels[Channel1-1];
            valid = (Ch1 >= StartRange1 && Ch1 <= EndRange1);
          }
      if ((valid || Channel1==0) && Channel2 > 0)
          {
            int16_t Ch2 = channels[Channel2-1];
            valid = (Ch2 >= StartRange2 && Ch2 <= EndRange2);
          }

      if (IsActive && !valid)
        IsActive = false;
      
      if (OneTime && IsActive)
        return false;

      return valid;
    };
};

extern CommandClass** Commands;
extern int MaxCommands;


void ProcessCommands();

#endif
