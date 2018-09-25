#chameleon
In this demo we use the RGBA sensors to sense other colors and turn on the leds in the color it sensed.

```c++

#include "Krembot/krembot.h"

Krembot krembot;


void setup()
{
    krembot.setup();
}


void loop()
{
  krembot.loop();
  RGBAResult rgbRes;
  HSVResult hsvRes;
  
  // read the rgba sensor. 
  rgbRes = krembot.RgbaRear.read();

  // convert the reult from rgb to hsv
  hsvRes = RGBASensor::rgbToHSV(rgbRes);

  // detect the color based on the rgb and hsv results
  Colors color = RGBASensor::WhichColor(rgbRes, hsvRes);

  // turn on the leds with the calculated color 
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
```