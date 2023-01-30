#if defined(ARDUINO_ARCH_ESP32) && defined(XBOX_SX)

#include "xbox.hpp"

#define XBOX_SERIES_X_CONTROLLER_RESET_WHEN_FAILING_CONNECTION
//#define XBOX_SERIES_X_CONTROLLER_DEBUG_SERIAL Serial



#include <XboxSeriesXControllerESP32_asukiaaa.hpp>

// XboxSeriesXControllerESP32_asukiaaa::Core xboxController("44:16:22:5e:b2:d4");
XboxSeriesXControllerESP32_asukiaaa::Core xboxController; // any xbox controller

unsigned long Last_Connected;
uint8_t count;

void InitXBoxSeriesSX()
{
  xboxController.begin();
  Last_Connected = millis();
}

void ProcessRemoteReceive()
{
  xboxController.onLoop();
  if (xboxController.isConnected())
  {
    Last_Connected = millis();

    if (xboxController.xboxNotif.joyLHori > 0 || xboxController.xboxNotif.joyLVert > 0)

    {
      // joystick
      communication_data[0] = map(xboxController.xboxNotif.joyLHori, 0, 65536, ANALOG_MIN, ANALOG_MAX);
      communication_data[1] = map(xboxController.xboxNotif.joyLVert, 65536, 0, ANALOG_MIN, ANALOG_MAX);
      communication_data[2] = map(xboxController.xboxNotif.joyRHori, 0, 65536, ANALOG_MIN, ANALOG_MAX);
      communication_data[3] = map(xboxController.xboxNotif.joyRVert, 65536, 0, ANALOG_MIN, ANALOG_MAX);

      // trigger analogs
      communication_data[4] = map(xboxController.xboxNotif.trigLT, 0, 1024, ANALOG_MIN, ANALOG_MAX);
      communication_data[5] = map(xboxController.xboxNotif.trigRT, 0, 1024, ANALOG_MIN, ANALOG_MAX);

      // binary channels
      communication_data[6] = xboxController.xboxNotif.btnY;
      communication_data[7] = xboxController.xboxNotif.btnX;
      communication_data[8] = xboxController.xboxNotif.btnB;
      communication_data[9] = xboxController.xboxNotif.btnA;
      communication_data[10] = xboxController.xboxNotif.btnLB;
      communication_data[11] = xboxController.xboxNotif.btnRB;
      communication_data[12] = xboxController.xboxNotif.btnSelect;
      communication_data[13] = xboxController.xboxNotif.btnStart;
      communication_data[14] = xboxController.xboxNotif.btnXbox;
      communication_data[15] = xboxController.xboxNotif.btnShare;
      communication_data[16] = xboxController.xboxNotif.btnLS;
      communication_data[17] = xboxController.xboxNotif.btnRS;
      communication_data[18] = xboxController.xboxNotif.btnDirUp;
      communication_data[19] = xboxController.xboxNotif.btnDirRight;
      communication_data[20] = xboxController.xboxNotif.btnDirDown;
      communication_data[21] = xboxController.xboxNotif.btnDirLeft;

      failsafe = false;
    }
    else
    {
      failsafe = true;
      count++;
      Serial.print("Xbox connections :");Serial.println(count);
      if (count>5)
        {
          ESP.restart();
        }


    }
  }
}

#endif
