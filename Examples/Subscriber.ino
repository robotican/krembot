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
