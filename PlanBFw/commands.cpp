#include "commands.hpp"
#include "sbus.hpp"
#include "json_parser.hpp"

CommandClass** Commands;
int MaxCommands;

void ProcessCommands()
{
  if (failsafe) //do not process in case of failsafe
    return;

    for(int i = 0; i < MaxCommands; i++)
      {
        auto res = Commands[i]->Validate(communication_data, true);
//        Serial.print(i);Serial.print(">");Serial.print(cast[0]);Serial.print(">");Serial.print(cast[1]);Serial.print(">");
//        Serial.println(res);

        if (res)
          {
            if (Commands[i]->FileName != "")
              LoadServoAnimationFromFile(Commands[i]->FileName+".jsn");

            Commands[i]->IsActive = true;
            Serial.println("Command activated");
          }

      }

}