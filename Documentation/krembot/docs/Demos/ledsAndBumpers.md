#Leds and Bumpers

this demo enables to turn on the leds on red, green and blue by pressing a bumper.

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
  
  // read thr bumpers and turn the leds on based on the pressed bumper
  BumpersRes results = krembot.Bumpers.read();
  if(results.left)
  {
    // turn the red leds on
    krembot.Led.write(255,0,0);
  }
  else if(rear)
  {
    // turn the green leds on
    krembot.Led.write(0,255,0);
  }
  else if(results.right)
  {
    // turn the blue leds on
    krembot.Led.write(0,0,255);
  }

  else if(results.front)
  {
    // turn all the leds off
    krembot.Led.write(0,0,0);
  }
}
```