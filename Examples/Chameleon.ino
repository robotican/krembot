/*
 * In this demo we use the RGBA sensors to sense other colors and turn on the leds in the color it sensed.
 */ 

#include "Krembot/krembot.h"

Krembot krembot;


void setup()
{
    krembot.setup();
}


void loop()
{
  krembot.loop();
  
  //read color using the front rgba sensor
  Colors color = krembot.RgbaFront.readColor();

  // turn on the leds with the color the krembot sees
  switch (color)
  {
    case Colors::Red :
    {
      // red
      krembot.Led.write(255,0,0);
      break;
    }

    case Colors::Green :
    {
      // green
      krembot.Led.write(0,255,0);
      break;
    }


    case Colors::Blue :
    {
      // blue
      krembot.Led.write(0,0,255);
      break;
    }


    default :
    {
      // if none detected - turn off all leds
      krembot.Led.write(0,0,0);
      break;
    }

  }

}
