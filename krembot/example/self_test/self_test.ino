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

/* Author:  Yair Shlomi*/

#include "Krembot/krembot.h"

Krembot krembot;
Timer driveTimer;
void setup()
{
    krembot.setup();
    Serial.println("Welcome To Krembot Self Tests");
    delay(2000);
    driveTimer.setPeriod(3000);
    checkLeds();
    checkBumpers();
    checkDriving();
    checkSensores();
}


void loop()
{


}

void checkLeds ()
{
  Serial.println("Let's start with the leds");
  delay(1000);
  krembot.Led.write(255,0,0);
  Serial.println("Look at the leds located on the krembot's head. Are all of them red? (y/n)");
  while(Serial.available() == 0){}
  if (Serial.read() != "y")
  {
   Serial.println("error");
   while ();
  }
  else
  {
    Serial.println("great. lets continue");
  }

  delay(1000);
  krembot.Led.write(0,255,0);
  Serial.println("Look at the leds again. Are all of them green? (y/n)");
  while(Serial.available() == 0){}
  if(Serial.read() != "y")
  {
   Serial.println("error");
   while ();
  }
  else
  {
    Serial.println("great. lets continue");
  }

  delay(1000);
  krembot.Led.write(0,0,255);
  Serial.println("Now look at the leds for the last time. Are all of them blue? (y/n)");
  while(Serial.available() == 0){}
  if(Serial.read() != "y")
  {
   Serial.println("error");
   while ();
  }
  else
  {
    Serial.println("great. let's move on to the next phase");
  }

}

void checkBumpers()
{
  Serial.println("Let's continue with the bumpers");
  BumpersRes results;
  delay(1000);
  Serial.println("Please press only the front bumper.");
  results = Bumpers.read();
  while(!(results.front && !results.right && !results.rear && !results.left))
  {
    results = Bumpers.read();
  }
  Serial.println("great. I see the front bumper was pressed");
  delay(1000);

  Serial.println("now please press only the right bumper.");
  results = Bumpers.read();
  while(!(!results.front && results.right && !results.rear && !results.left))
  {
    results = Bumpers.read();
  }
  Serial.println("great. I see the right bumper was pressed");
  delay(1000);

  Serial.println("now please press only the rear bumper.");
  results = Bumpers.read();
  while(!(!results.front && !results.right && results.rear && !results.left))
  {
    results = Bumpers.read();
  }
  Serial.println("great. I see the rear rear was pressed");
  delay(1000);

  Serial.println("and for the last onw - Please press only the left bumper.");
  results = Bumpers.read();
  while(!(!results.front && !results.right && !results.rear && results.left))
  {
    results = Bumpers.read();
  }
  Serial.println("great. I see the left bumper was pressed");
  delay(1000);
}

void checkDriving()
{
  Serial.println("Let's check the krembot's movement");
  delay(1000);
  int result = "0";
  do
  {
    printDrivingMenu();
    while (Serial.available() == 0);
    result = Serial.parseInt();
    switch (result)
    {
      case 1:
      {
        Serial.println("The robot will move forward for 3 seconds.");
        Serial.println("Please make sure that the robot is on flat surface and have enough space.");
        Serial.println("Please look if the robot is moving in a straight line.");
        Base.drive(30, 0);
        driveTimer.start();
        while(!driveTimer.finished()){}
        Base.stop();
        Serial.println("If the robot didn't move in straight line, the motors may need calibration.");
        Serial.println("In case the robot turned right, the right motor needs calibration.");
        Serial.println("In case the robot turned left, the left motor needs calibration.");
        Serial.println("You can choose the calibration mode you need the next time that the menu will be printed");
        break;
      }

      case 2:
      {
        Serial.println("The robot will move backword for 3 seconds.");
        Serial.println("Please make sure that the robot is on flat surface and have enough space.");
        Serial.println("Please look if the robot is moving in a straight line.");
        Base.drive(30, 0);
        driveTimer.start();
        while(!driveTimer.finished()){}
        Base.stop();
        Serial.println("If the robot didn't move in straight line, the motors may need calibration.");
        Serial.println("In case the robot turned right, the right motor needs calibration.");
        Serial.println("In case the robot turned left, the left motor needs calibration.");
        Serial.println("You can choose the calibration mode you need the next time that the menu will be printed");
        break;
      }

      case 3:
      {
        Serial.println("The robot will rotate left for 3 seconds.");
        Serial.println("Please make sure that the robot is on flat surface and have enough space.");
        Serial.println("Please look if the robot is rotating left ");
        Base.drive(0, 30);
        driveTimer.start();
        while(!driveTimer.finished()){}
        Base.stop();
        Serial.println("If the robot didn't rotate left, the motors may need calibration.");
        Serial.println("You can choose the calibration mode you need the next time that the menu will be printed");
        break;
      }

      case 4:
      {
        Serial.println("The robot will rotate right for 3 seconds.");
        Serial.println("Please make sure that the robot is on flat surface and have enough space.");
        Serial.println("Please look if the robot is rotating right");
        Base.drive(0, -30);
        driveTimer.start();
        while(!driveTimer.finished()){}
        Base.stop();
        Serial.println("If the robot didn't rotate right, the motors may need calibration.");
        Serial.println("You can choose the calibration mode you need the next time that the menu will be printed");
        break;
      }

      case 5:
      {
        calibrationMode(true);

        break;
      }

      case 6:
      {
        calibrationMode(false);
        break;
      }

      case 0:
      {
        result = 0;
        break;
      }

      default:
      {
        result = 0;
        break;
      }
    }
  } while(result != 0);
}

void printDrivingMenu()
{
  Serial.println("menu:");
  Serial.println("1. drive forward");
  Serial.println("2. drive backword");
  Serial.println("3. drive left");
  Serial.println("4. drive right");
  Serial.println("5. calibrate right motor");
  Serial.println("6. calibrate left motor");
  Serial.println("0. exit ");
}

void calibrationMode(bool isRightMotor)
{
  int8_t offset;
  if(isRightMotor)
  {
    offset = EEPROM.read(BASE_RIGHT_OFFSET_ADDR);
  }
  else
  {
    offset = EEPROM.read(BASE_LEFT_OFFSET_ADDR);
  }
  char input = '0';
  Serial.print("Motors Calibration Mode: ");
  if(isRightMotor)
  {
    Serial.print("right ");
  }
  else
  {
    Serial.print("left");
  }
  Serial.println("Motor.");

  Serial.print("Currnet offset: ");
  Serial.println(offset);
  Serial.println("to increase the offset of the motor by 1, press '+'.");
  Serial.println("to decrease the offset of the motor by 1, press '-'.");
  Serial.println("to save the offset of the motor and exit, press 's'.");
  while (input != 's' )
  {
    while(Serial.available() == 0){}
    input = Serial.read();
    if(input == '+')
    {
      if(offset < 50)
      {
        offset++;
      }
    }
    else if (input == '-')
    {
      if(offset > 0)
      {
        offset--;
      }
    }
    if(input == 's')
    {
      Serial.print("Saved offset: ");
      if(isRightMotor)
      {
        EEPROM.write(BASE_RIGHT_OFFSET_ADDR, offset);
      }
      else
      {
        EEPROM.write(BASE_LEFT_OFFSET_ADDR, offset);
      }
      Serial.println(offset);
    }
    else
    {
      Serial.print("Currnet offset: ");
      Serial.println(offset);
    }
  }

}

void checkSensores()
{
  Serial.println("Last but not least - the RGB sensors");
  Serial.println("The values of the sensors will be printed on the screen.");
  Serial.println("The values of each sensor will be printed until you press any key");
  Serial.println("Please make sure that the values changes when the color of the led changes");

  Serial.println("front sensor:");
  CheckSensor(RgbaFront);

  Serial.println("front right sensor:");
  CheckSensor(RgbaFrontRight);

  Serial.println("right sensor:");
  CheckSensor(RgbaRight);

  Serial.println("rear right sensor:");
  CheckSensor(RgbaRearRight);

  Serial.println("rear sensor:");
  CheckSensor(RgbaRear);

  Serial.println("rear left sensor:");
  CheckSensor(RgbaRearLeft);

  Serial.println("left sensor:");
  CheckSensor(RgbaLeft);

  Serial.println("front left sensor:");
  CheckSensor(RgbaFrontLeft);

}

void CheckSensor (RGBASensor &sensor)
{

  while(Serial.available() == 0)
  {
    sensor.print();
  }
  Serial.println("O.K. moving to the next sensor.");
}
