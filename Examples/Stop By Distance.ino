/*
* In this demo we constantly read the distance from the rgba sensor.
* the krembot drives forward with the green leds on.
* when the distance read is below the minimum distance
* the krembot stops and the red leds are turned on.
*/


#include "Krembot/krembot.h"

//Only one instance of krembot object should be declared
Krembot krembot;

const int MinDistance = 9;
float distance = 0, lastDistance = 0, lastLastDistance = 0;
int deadband = 2;

RGBAResult rgbRes;

void setup()
{
    krembot.setup();

    //save the first distance result of the rgba sensor
    rgbRes = krembot.RgbaLeft.readRGBA();
    lastDistance = rgbRes.Distance;
}


void loop()
{
  krembot.loop();
  // read the distance from the front sensor
  rgbRes = krembot.RgbaLeft.readRGBA();
  distance = rgbRes.Distance;

  // if the distance is larger then the minimum and the last 2 samples has
  // reasonable distance, drive forward and turn the leds green

  if(distance > MinDistance && abs((lastDistance - distance)) < deadband)
  {
    krembot.Base.drive(30, 0);
    krembot.Led.write(0, 255, 0);
  }

  // if the distance is not larger then the minimum and the last 2 samples has
  // reasonable distance, stop driving and turn the leds red
  else if (distance <= MinDistance && abs((lastDistance - distance)) < deadband)
  {
    krembot.Base.stop();
    krembot.Led.write(255, 0, 0);
  }
  lastDistance = distance;

}
