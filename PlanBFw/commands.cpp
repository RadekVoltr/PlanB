#include "commands.hpp"
#include "sbus.hpp"
#include "json_parser.hpp"

CommandClass** Commands;
int MaxCommands;

template <typename ValueT, std::size_t size>
using CArray = ValueT[size];  

template <typename ValueT, std::size_t size>  
CArray<ValueT, size> & c_array_cast(std::array<ValueT, size> & arg)
{                                      
    return *reinterpret_cast<CArray<ValueT,size>*>(arg.data());  
}  

void ProcessCommands()
{
  if (sbus_failsafe) //do not process in case of failsafe
    return;

    auto cast = c_array_cast(sbus_data);
    for(int i = 0; i < MaxCommands; i++)
      {
        auto res = Commands[i]->Validate(cast, true);
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