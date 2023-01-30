#include "json_parser.hpp"
#include "commands.hpp"
#include "tank.hpp"
#include "servos.hpp"

 DynamicJsonDocument doc(2048);


void InitCommandsFromFile(String s)
{
    Serial.println("check exists "+s);

    if (!LittleFS.exists("/"+s))
    {
      Serial.println("file not exists, skip");
      return;
    }
      Serial.println("open file");


    File f = LittleFS.open("/"+s);
    if (!f) 
      Serial.println("file open failed");
    Serial.println(f.size());
    
    f.setTimeout(1000);  // 10 seconds
    
    DeserializationError err = deserializeJson(doc, f);
    f.close();
    if (err) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.f_str());
    }
  
  Serial.println("init loaded");
  
  JsonArray array = doc.as<JsonArray>();
  MaxCommands = array.size();

  Serial.println(MaxCommands);

  Commands = (CommandClass **) malloc(sizeof(CommandClass*) * MaxCommands);

  int i = 0;
  for (JsonObject commandItem : array) {
    Serial.print("commandItem ");Serial.print(i);Serial.print(" ");
    int ch1 = commandItem["ch1"];
    Serial.print("Channel 1:");Serial.print(ch1);

    int ch2 = commandItem["ch2"];
    Serial.print(" | Channel 2:");Serial.print(ch2);
    
    int s1 = commandItem["start"];
    Serial.print(" | Start 1:");Serial.print(s1);
    
    int e1 = commandItem["end"] | 99999;
    Serial.print(" | End 1:");Serial.print(e1);

    int s2 = commandItem["start2"];
    Serial.print(" | Start 2:");Serial.print(s2);
    
    int e2 = commandItem["end2"] | 99999;
    Serial.print(" | End 2:");Serial.print(e2);
    
    String f = commandItem["file"];
    Serial.print(" | FName :");Serial.print(f);

    String snd = commandItem["snd"];
    Serial.print(" | SoundName :");Serial.print(snd);

    Serial.println();
    

    Commands[i] = new CommandClass();
    Commands[i]->Channel1 = ch1;
    Commands[i]->Channel2 = ch2;
    Commands[i]->StartRange1 = s1;
    Commands[i]->StartRange2 = s2;
    Commands[i]->EndRange1 = e1;
    Commands[i]->EndRange2 = e2;
    Commands[i]->FileName = f;
    Commands[i]->Sound = snd;

    i++;
  }

    f.close();

}

void InitServosFromFile(String s)
{
    if (!LittleFS.exists("/"+s))
    {
      Serial.println("file not exists, skip");
      return;
    }

    Serial.println("open file "+s);
    File f = LittleFS.open("/"+s);
    Serial.println("open file done "+s);
    if (!f) 
      Serial.println("file open failed");
    Serial.println(f.size());
    
  f.setTimeout(1000);  // 10 seconds
    
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
  }
  
  Serial.println("init loaded");
  Serial.println(doc.memoryUsage());

  JsonArray array = doc.as<JsonArray>();
  Serial.println(array.size());

 for (JsonObject servo : array) {
    Serial.print("servo ");
    int servo_id = servo["srv"];
    Serial.print("id:");Serial.println(servo_id);
    
    int init = servo["init"] | 0;
    Serial.print("init value:");Serial.println(init);

    SetServoMicros(servo_id-1, init, false);
  }

  f.close();

}

void InitTankMixesFromFile(String s)
{
    if (!LittleFS.exists("/"+s))
    {
      Serial.println("file not exists, skip");
      return;
    }

    Serial.println("open file "+s);
    File f = LittleFS.open("/"+s);
    Serial.println("open file done "+s);
    if (!f) 
      Serial.println("file open failed");
    Serial.println(f.size());
    
  f.setTimeout(1000);  // 10 seconds
    
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
  }
  
  Serial.println("init loaded");
  Serial.println(doc.memoryUsage());

  JsonArray array = doc.as<JsonArray>();
  Serial.println(array.size());

  MaxTanks = array.size();
  Tanks = (TankClass**) malloc(sizeof(TankClass*) * MaxTanks);
  int i = 0;
 for (JsonObject tank : array) {
    Serial.print("tank object ");
    int16_t channelA = tank["channelA"];
    Serial.print("channelA:");Serial.println(channelA);
    int16_t channelB = tank["channelB"];
    Serial.print("channelB:");Serial.println(channelB);
    
    int16_t revertA = tank["revertA"];
    Serial.print("revertA:");Serial.println(revertA);
    int16_t revertB = tank["revertB"];
    Serial.print("revertB:");Serial.println(revertB);

    int16_t servoA = tank["servoA"];
    Serial.print("servoA:");Serial.println(servoA);
    
    int16_t servoB = tank["servoB"];
    Serial.print("servoB:");Serial.println(servoB);
    
    int16_t pwmA = tank["pwmA"];
    Serial.print("pwmA:");Serial.println(pwmA);
    int16_t pwmB = tank["pwmB"];
    Serial.print("pwmB:");Serial.println(pwmB);
    
    int16_t topLimitA = tank["topLimitA"];
    Serial.print("topLimitA:");Serial.println(topLimitA);
    
    int16_t downLimitA = tank["downLimitA"];
    Serial.print("downLimitA:");Serial.println(downLimitA);
    
    int16_t trimA = tank["trimA"];
    Serial.print("trimA:");Serial.println(trimA);

    int16_t topLimitB = tank["topLimitB"];
    Serial.print("topLimitB:");Serial.println(topLimitB);
    
    int16_t downLimitB = tank["downLimitB"];
    Serial.print("downLimitB:");Serial.println(downLimitB);
    
    int16_t trimB = tank["trimB"];
    Serial.print("trimB:");Serial.println(trimB);

    
    String type = tank["type"];
    Serial.print("type value:");Serial.println(type);

    if (type == "pwm")
      {
        Tanks[i] = new TankClass(channelA, channelB, pwmA-1, pwmB-1, topLimitA,downLimitA,trimA,topLimitB,downLimitB,trimB, revertA, revertB, pwm_driver);
      }
      else
      {
        Tanks[i] = new TankClass(channelA, channelB, servoA-1, servoB-1, topLimitA,downLimitA,trimA,topLimitB,downLimitB,trimB, revertA, revertB, servo_driver);
      }

    i++;
    
  }

  f.close();

}

void InitMotorsFromFile(String s)
{
    if (!LittleFS.exists("/"+s))
    {
      Serial.println("file not exists, skip");
      return;
    }

    Serial.println("open file "+s);
    File f = LittleFS.open("/"+s);
    Serial.println("open file done "+s);
    if (!f) 
      Serial.println("file open failed");
    Serial.println(f.size());
    
  f.setTimeout(1000);  // 10 seconds
    
  DeserializationError err = deserializeJson(doc, f);
  f.close();
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
  }
  
  Serial.println("init loaded");
  Serial.println(doc.memoryUsage());

  JsonArray array = doc.as<JsonArray>();
  Serial.println(array.size());

  MaxMotors = array.size();
  Motors = (MotorClass**) malloc(sizeof(MotorClass*) * MaxMotors);
  int i = 0;
 for (JsonObject Motor : array) {
    Serial.print("Motor object ");
    int channel = Motor["channel"];
    Serial.print("channel:");Serial.println(channel);

    int revert = Motor["revert"];
    Serial.print("revert:");Serial.println(revert);

    int servoIdx = Motor["servo"];
    Serial.print("servo:");Serial.println(servoIdx);
    
    int pwm = Motor["pwm"];
    Serial.print("pwm:");Serial.println(pwm);
    
    int ena = Motor["ena"];
    Serial.print("ena:");Serial.println(ena);
    
    int topLimit = Motor["topLimit"];
    Serial.print("topLimit:");Serial.println(topLimit);
    
    int downLimit = Motor["downLimit"];
    Serial.print("downLimit:");Serial.println(downLimit);
    
    int trim = Motor["trim"];
    Serial.print("trim:");Serial.println(trim);
    
    String type = Motor["type"];
    Serial.print("type value:");Serial.println(type);

    if (type == "pwm")
      {

      }
      else
      {
        Motors[i] = new MotorClass(channel, servoIdx-1, 0, topLimit, downLimit, trim, revert, servo_driver);
      }

    i++;
    
  }
  f.close();

}

void LoadServoAnimationFromFile(String s)
{
      Serial.println("open file "+s);
  File f = LittleFS.open("/"+s, "r");
  if (!f) 
    Serial.println("file open failed");

  deserializeJson(doc, f);
  f.close();

  Serial.println("loaded");
  Serial.println(doc.memoryUsage());

  int repeat = doc["rpt"] | 0;
  Serial.print("repeat :");Serial.println(repeat);

  ServoTimeline.mode((Tween::Mode)repeat);

  Serial.print("number of servo operations ");

  JsonArray array = doc["srvs"];
  Serial.println(array.size());

  for (JsonObject servo : array) 
  {
    Serial.print("servo ");
    int servo_id = servo["srv"];
    Serial.print("id:");Serial.println(servo_id);
    
    int init = servo["init"] | 0;
    Serial.print("init value:");Serial.println(init);

    int timeout = servo["tout"] | 0;
    Serial.print("time delay:");Serial.println(timeout);

    bool AutoErase = (repeat == 0);
    Serial.print("AutoErase :");Serial.println(AutoErase);

    //clear timeline
    ServoTimeline.stop();
    ServoTimeline.clear();

/*
    if (init != 0)
      ServoTimeline.add(Servos[servo_id].position, AutoErase).init(init).offset(timeout);
      else
      ServoTimeline.add(Servos[servo_id].position, AutoErase).init(Servos[servo_id].position).offset(timeout);
      

      Serial.print("number of moves ");
      JsonArray moves = servo["mov"];
      Serial.println(moves.size());
      
      
      for (JsonObject mo : moves) {     
        int target = mo["trg"] | 0;
        Serial.print("target:");Serial.println(target);
        
        int period = mo["time"] | 0;
        Serial.print("period:");Serial.println(period);
        
        int temp = mo["eas"] | 0;
        EasingType easing = (EasingType)temp;
        Serial.print("easing:");Serial.println(easing);

        switch (easing)
        {
          Hold:
            ServoTimeline[Servos[servo_id].position].hold(period);
          QuartIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::QuartIn>(target, period);
          QuartOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::QuartOut>(target, period);
          QuartInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::QuartInOut>(target, period);
          QuintIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::QuintIn>(target, period);
          QuintOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::QuintOut>(target, period);
          QuintInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::QuintInOut>(target, period);
          SineIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::SineIn>(target, period);
          SineOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::SineOut>(target, period);
          SineInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::SineInOut>(target, period);
          BackIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::BackIn>(target, period);
          BackOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::BackOut>(target, period);
          BackInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::BackInOut>(target, period);
          BounceIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::BounceIn>(target, period);
          BounceOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::BounceOut>(target, period);
          BounceInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::BounceInOut>(target, period);
          CircIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::CircIn>(target, period);
          CircOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::CircOut>(target, period);
          CircInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::CircInOut>(target, period);
          CubicIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::CubicIn>(target, period);
          CubicOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::CubicOut>(target, period);
          CubicInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::CubicInOut>(target, period);
          ElasticIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::ElasticIn>(target, period);
          ElasticOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::ElasticOut>(target, period);
          ElasticInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::ElasticInOut>(target, period);
          ExpoIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::ExpoIn>(target, period);
          ExpoOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::ExpoOut>(target, period);
          ExpoInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::ExpoInOut>(target, period);
          QuadIn:
            ServoTimeline[Servos[servo_id].position].then<Ease::QuadIn>(target, period);
          QuadOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::QuadOut>(target, period);
          QuadInOut:
            ServoTimeline[Servos[servo_id].position].then<Ease::QuadInOut>(target, period);
          default:
            ServoTimeline[Servos[servo_id].position].then<Ease::Linear>(target, period);
        }
      } */
  }
  ServoTimeline.update_duration();
  ServoTimeline.start();
}
