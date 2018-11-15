/*******************************************************************************
* Copyright (c) 2018 RoboTICan
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the disclaimer
* below) provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*
*     * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*
*     * Neither the name of the copyright holder nor the names of its
*     contributors may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
* THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/* Author: Yair Shlomi */


#include "Krembot/krembot.h"
#include "Krembot/SandTimer/SandTimer.h"


//Only one instance of krembot object should be declared
Krembot krembot;
SandTimer driveTimer, ledsTimer, saveLedsTimer, saftySaveTimer;
bool savedToEeprom = false;
int8_t rightOffset = 0, leftOffset = 0;
void setup()
{
    krembot.setup();
    Particle.subscribe("wheels", WheelsCalibFunc, MY_DEVICES);
    driveTimer.setPeriod(5000);
    ledsTimer.setPeriod(3000);
    saveLedsTimer.setPeriod(4000);
    saftySaveTimer.setPeriod(30000);
    Particle.publish("feedback", "I'm ready to go! lets calibrate some wheels!", PRIVATE);

}


void loop()
{
  krembot.loop();
  //your code here
  if(driveTimer.isRunning() && driveTimer.finished())
  {
    krembot.Base.stop();
    krembot.Led.write(0, 0, 0);
  }

  if(ledsTimer.isRunning() && ledsTimer.finished())
  {
    krembot.Led.write(0, 0, 0);
  }

  if(saveLedsTimer.isRunning() && saveLedsTimer.finished())
  {
    krembot.Led.write(0, 0, 0);
  }

}

void WheelsCalibFunc(String topic, String data)
{

  String command = String(data);
  data.toLowerCase();
  if(command.compareTo("go") == 0)
  {
    krembot.Led.write(255, 0, 0);
    krembot.Base.drive(30, 0);
    driveTimer.reset();
    driveTimer.start();

    EEPROM.get(BASE_RIGHT_OFFSET_ADDR, rightOffset);
    EEPROM.get(BASE_LEFT_OFFSET_ADDR, leftOffset);

    String rightOffsetStr = String(rightOffset);
    String leftOffsetStr = String(leftOffset);

    Particle.publish("feedback", "drive forward command received", PRIVATE);
    Particle.publish("offset", "right offset: " + rightOffsetStr, PRIVATE);
    Particle.publish("offset", "left offset: " + leftOffsetStr, PRIVATE);

  }
  else if(command.compareTo("back") == 0)
  {
    krembot.Led.write(255, 0, 0);
    krembot.Base.drive(-30, 0);
    driveTimer.reset();
    driveTimer.start();
    Particle.publish("feedback", "drive backwords command received", PRIVATE);
  }

  else if(command.startsWith("left"))
  {
    int index = command.lastIndexOf("_");
    String offsetStr = command.substring(index + 1);
    int offset = offsetStr.toInt();
    if(offset != 0 && offset < 50)
    {
        leftOffset = offset;
        krembot.Led.write(0, 0, 255);
        ledsTimer.reset();
        ledsTimer.start();

        Particle.publish("feedback", "received left offset: " + offsetStr, PRIVATE);
    }
    else
    {
      leftOffset = 0;
      Particle.publish("feedback", "error - non number value: " + offsetStr, PRIVATE);
    }
  }

  else if(command.startsWith("right"))
  {
    int index = command.lastIndexOf("_");
    String offsetStr = command.substring(index + 1);
    int offset = offsetStr.toInt();
    if(offset != 0 && offset < 50)
    {
        rightOffset = offset;
        krembot.Led.write(0, 0, 255);
        ledsTimer.reset();
        ledsTimer.start();

        Particle.publish("feedback", "received right offset: " + offsetStr, PRIVATE);
    }
    else Particle.publish("feedback", "error - non number value: " + offsetStr, PRIVATE);
  }

  else if(command.compareTo("save") == 0)
  {
      if(!savedToEeprom)
      {
        krembot.Led.write(0, 255, 0);
        //EEPROM.write(BASE_RIGHT_OFFSET_ADDR, rightOffset);
        //EEPROM.write(BASE_LEFT_OFFSET_ADDR, leftOffset);
        EEPROM.put(BASE_RIGHT_OFFSET_ADDR, rightOffset);
        EEPROM.put(BASE_LEFT_OFFSET_ADDR, leftOffset);


        savedToEeprom = true;
        saveLedsTimer.reset();
        saveLedsTimer.start();
        saftySaveTimer.reset();
        saftySaveTimer.start();
        String rightOffsetstr = String(rightOffset);
        String leftOffsetstr = String(leftOffset);
        Particle.publish("feedback", "values saved to EEPROM successfully. right: " + rightOffsetstr + ", left: " + leftOffsetstr, PRIVATE);
      }
      else
      {
        Particle.publish("feedback", "save error - please open the lock with the code 123", PRIVATE);
      }

  }

  else if(command.compareTo("123") == 0)
  {
    if(saftySaveTimer.finished())
    {
      savedToEeprom = false;
      krembot.Led.write(255, 255, 255);
      saveLedsTimer.reset();
      saveLedsTimer.start();
      Particle.publish("feedback", "save safty lock removed", PRIVATE);

    }
    else
    {
      Particle.publish("feedback", "safty time not finished yet", PRIVATE);

    }

  }

}
