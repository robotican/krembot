/*******************************************************************************
* Copyright (c) 2018, RoboTICan, LTD.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* * Neither the name of RoboTICan nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/* Author: Yair Shlomi */

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
