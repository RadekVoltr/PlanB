#ifndef SRC_SOUNDS_H_
#define SRC_SOUNDS_H_

#include "configuration.hpp"

class SoundClass {
  public:
  bool IsActive;
  int16_t FileId;
  uint16_t PlayTime;

  
    SoundClass() {
      FileId = -1;
      PlayTime = 0;
      IsActive = false;
    };
};



#endif
