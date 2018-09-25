/*
* This demo shows how to subscribes to a cloud event and handles it in the handler function.
* It subscribes to the event that was published in the publisher demo
* and turns the leds on in a color that matches to the publisher krembot's battery level.
*/


#include "Krembot/krembot.h"

Krembot krembot;

void setup()
{
    krembot.setup();
    // subscribe to the battery topic from the cloud and declare the handler
    Particle.subscribe("battery", ChangeLedsForBattery, MY_DEVICES);
}

void loop()
{
  krembot.loop();
}

// handler for the battery topic
void ChangeLedsForBattery(String topic, String data)
{

  // read the battery level, from the topic
  String batLvlString = String(data);
  long batLvl = batLvlString.toInt();

  // change the leds colors based on the battery level
  if(batLvl > 80)
  {
    // turn on the green leds
    krembot.Led.write(0, 255 ,0);
  }
  else if(batLvl <  80 && batLvl > 20)
  {
    // turn on the blue leds
    krembot.Led.write(0, 0, 255);
  }
  else
  {
    // turn on the red leds
    krembot.Led.write(255, 0 ,0);
  }
}
