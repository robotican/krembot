#Publisher

This demo shows how to publish data to the particle cloud.  
please refer to the [cloud console](https://console.particle.io/devices)
and choose one of the online krembots to see the data that was published.


```c++
#include "Krembot/krembot.h"
#include "Krembot/SandTimer/SandTimer.h"

Krembot krembot;
SandTimer publishTimer;

const long INTERVAL = 60000;

void setup()
{
    krembot.setup();
    // start timer of 1 minute
    publishTimer.start(INTERVAL);
}


void loop()
{
  krembot.loop();
  if (publishTimer.finished())
  {
    // if the timer finishedm read the battery level
    String bat =  String(krembot.Bat.getBatLvl());
    // publish the battery level to the cloud.
    Particle.publish("battery", bat, PRIVATE);
    publishTimer.startOver();
  }

}
```