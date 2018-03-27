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

enum Stage
{
  LEDS,
  BUMPERS,
  DRIVING,
  SENSORS,
  DONE
};

struct Leds
{
  bool red = false,
      green = false,
      blue = false;

  bool red_printed = false,
       green_printed = false,
       blue_printed = false;
};

struct Bumpers
{
  bool front = false,
       front_right = false,
       right = false,
       rear_right = false,
       rear = false,
       rear_left = false,
       left = false,
       front_left = false;

  bool front_printed = false,
       front_right_printed = false,
       right_printed = false,
       rear_right_printed = false,
       rear_printed = false,
       rear_left_printed = false,
       left_printed = false,
       front_left_printed = false;
}

struct Driving
{
  bool calibrating_left = false,
       calibrating_right = false;

  bool header_printed = false,
       case_printed = false,
       menu_printed = false,
       calibration_printed = false,
};

struct Sensors
{
  bool front = false,
       front_right = false,
       right = false,
       rear_right = false,
       rear = false,
       rear_left = false,
       left = false,
       front_left = false;

  bool front_printed = false,
       front_right_printed = false,
       right_printed = false,
       rear_right_printed = false,
       rear_printed = false,
       rear_left_printed = false,
       left_printed = false,
       front_left_printed = false;

  bool header_printed;
};

Krembot krembot;
Timer drive_timer;
BumpersRes results;
Stage current_stage;

void setup()
{
    krembot.setup();
    Serial.println("Welcome To Krembot Self Tests");
    wait(2000);
    drive_timer.setPeriod(3000);
    current_stage = STAGE.LEDS;
    11133d
}

void loop()
{
  if(current_stage == STAGE.LEDS)
  {
    if(check_leds())
    {
      current_stage == STAGE.BUMPERS;
    }
  }
  else if(current_stage == STAGE.BUMPERS)
  {
    if(check_bumpers())
    {
      current_stage == STAGE.DRIVING;
    }
  }
  else if(current_stage == STAGE.DRIVING)
  {
    if(check_driving())
    {
      current_stage == STAGE.SENSORS;
    }
  }
  else if(current_stage == STAGE.SENSORS)
  {
    if(check_sensors())
    {
      current_stage == STAGE.DONE;
    }
  }
  else if(current_stage == STAGE.DONE)
  {
    Serial.println("self test done");
    wait(5000);
  }



}
bool check_leds ()
{
  if (!Leds.red_printed)
  {
    Serial.println("Let's start with the leds");
    wait(1000);
    krembot.Led.write(255,0,0);
    Serial.println("Look at the leds located on the krembot's head. Are all of them red? (y/n)");
    Leds.red_printed = true;
  }

  if(!Leds.red)
  {
    if (!Serial.available())
      return false;
    if (Serial.read() != "y")
    {
     Serial.println("error");
     return false;
    }
    else
    {
      Serial.println("great. lets continue");
      Leds.red = true;
    }
  }

  if(!Leds.green_printed)
  {
    Serial.println("Look at the leds again. Are all of them green? (y/n)");
    wait(1000);
    krembot.Led.write(0,255,0);
    Leds.green_printed = true;
  }

  if(!Leds.green)
  {
    if (!Serial.available())
      return false;
    if (Serial.read() != "y")
    {
     Serial.println("error");
     return false;
    }
    else
    {
      Leds.green = true;
      Serial.println("great. lets continue");
    }
  }

  if(!Leds.blue_printed)
  {
    wait(1000);
    krembot.Led.write(0,0,255);
    Serial.println("Now look at the leds for the last time. Are all of them blue? (y/n)");
    Leds.blue_printed = true;
  }
  if(!Leds.blue)
  {
    if (!Serial.available())
      return false;
    if (Serial.read() != "y")
    {
     Serial.println("error");
     return false;
    }
    else
    {
      Leds.blue = true;
      Serial.println("great. let's move on to the next phase");
    }
  }

  if(Leds.red && Leds.green && Leds.blue)
  {
    return true;
  }

  return false;

}

bool check_bumpers()
{
  if(!Bumpers.front_printed)
  {
    Serial.println("Let's continue with the bumpers");
    wait(1000);
    Serial.println("Please press only the front bumper.");
    Bumpers.front_printed = true;
  }
  if(!Bumpers.front)
  {
    results = Bumpers.read();
    if(results.front && !results.front_right && !results.right && !results.rear_right
       && !results.rear && !results.rear_left && !results.left && !results.front_left)
    {
      Bumpers.front = true;
      Serial.println("great. I see the front bumper was pressed");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!Bumpers.front_right_printed)
  {
    Serial.println("Please press only the front right bumper.");
    Bumpers.front_right_printed = true;
  }
  if(!Bumpers.front_right)
  {
    results = Bumpers.read();
    if(!results.front && results.front_right && !results.right && !results.rear_right
       && !results.rear && !results.rear_left && !results.left && !results.front_left))
    {
      Bumpers.front_right = true;
      Serial.println("great. I see the front right bumper was pressed");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!Bumpers.right_printed)
  {
    Serial.println("Please press only the right bumper.");
    Bumpers.right_printed = true;
  }
  if(!Bumpers.right)
  {
    results = Bumpers.read();
    if(!results.front && !results.front_right && results.right && !results.rear_right
       && !results.rear && !results.rear_left && !results.left && !results.front_left))
    {
      Bumpers.right = true;
      Serial.println("great. I see the right bumper was pressed");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!Bumpers.rear_right_printed)
  {
    Serial.println("Please press only the rear right bumper.");
    Bumpers.rear_right_printed = true;
  }
  if(!Bumpers.rear_right)
  {
    results = Bumpers.read();
    if(!results.front && !results.front_right && !results.right && results.rear_right
       && !results.rear && !results.rear_left && !results.left && !results.front_left))
    {
      Bumpers.rear_right = true;
      Serial.println("great. I see the rear right bumper was pressed");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!Bumpers.rear_printed)
  {
    Serial.println("Please press only the rear bumper.");
    Bumpers.rear_printed = true;
  }
  if(!Bumpers.rear)
  {
    results = Bumpers.read();
    if(!results.front && !results.front_right && !results.right && !results.rear_right
       && results.rear && !results.rear_left && !results.left && !results.front_left))
    {
      Bumpers.rear = true;
      Serial.println("great. I see the rear bumper was pressed");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!Bumpers.rear_left_printed)
  {
    Serial.println("Please press only the rear left bumper.");
    Bumpers.rear_left_printed = true;
  }
  if(!Bumpers.rear_left)
  {
    results = Bumpers.read();
    if(!results.front && !results.front_right && !results.right && !results.rear_right
       && !results.rear && results.rear_left && !results.left && !results.front_left))
    {
      Bumpers.rear_left = true;
      Serial.println("great. I see the rear left bumper was pressed");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!Bumpers.left_printed)
  {
    Serial.println("Please press only the left bumper.");
    Bumpers.left_printed = true;
  }
  if(!Bumpers.left)
  {
    results = Bumpers.read();
    if(!results.front && !results.front_right && !results.right && !results.rear_right
       && !results.rear && !results.rear_left && results.left && !results.front_left))
    {
      Bumpers.left = true;
      Serial.println("great. I see the left bumper was pressed");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!Bumpers.front_left_printed)
  {
    Serial.println("Please press only the front left bumper.");
    Bumpers.front_left_printed = true;
  }
  if(!Bumpers.frontleft)
  {
    results = Bumpers.read();
    if(!results.front && !results.front_right && !results.right && !results.rear_right
       && !results.rear && !results.rear_left && !results.left && results.front_left))
    {
      Bumpers.front_left = true;
      Serial.println("great. I see the front left bumper was pressed");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(Bumpers.front && Bumpers.front_right && Bumpers.right && Bumpers.rear_right
     && Bumpers.rear &&Bumpers.rear_left &&Bumpers.left &&Bumpers.front_left)
  {
      return true;
  }

  return false;

}

bool check_driving()
{
  if(!Driving.header_printed)
  {
    Serial.println("Let's check the krembot's movement");
    wait(1000);
    header_printed = true;
  }
  int result = 9;
  do
  {
    if(!Driving.menu_printed)
    {
      print_driving_menu();
      Driving.menu_printed = true;
    }

    if(!Serial.available() && !Driving.calibrating_left && !Driving.calibrating_right)
    {
      return false;
    }

    result = Serial.parseInt();
    if(Driving.calibrating_right)
    {
      result = 5;
    }
    else if(Driving.calibrating_left)
    {
      result = 6;
    }

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
        wait(3000);
        Driving.menu_printed = false;
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
        Driving.menu_printed = false;
        wait(3000);
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
        Driving.menu_printed = false;
        wait(3000);
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
        Driving.menu_printed = false;
        wait(3000);
        break;
      }

      case 5:
      {
        Driving.calibrating_right = true;
        if(calibration_mode(true)) //calibrating right
        {
          Driving.menu_printed = false;
          Driving.calibration_printed = false;

          wait(3000);
        }
        break;
      }

      case 6:
      {
        Driving.calibrating_left = true;
        if(calibration_mode(false)) //calibrating left
        {
          Driving.menu_printed = false;
          Driving.calibration_printed = false;
          wait(3000);
        }
        break;
      }

      case 0:
      {
        return true;
      }

      default:
      {
        return false;
      }
    }
  } while(result != 0);

  return false;
}

void print_driving_menu()
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

bool calibration_mode(bool isRightMotor)
{
  int8_t offset;
  if(isRightMotor)
    Driving.calibrating_right = true;
  else Driving.calibrating_left = true;

  if(isRightMotor)
  {
    offset = EEPROM.read(BASE_RIGHT_OFFSET_ADDR);
  }
  else
  {
    offset = EEPROM.read(BASE_LEFT_OFFSET_ADDR);
  }
  char input = '0';
  if(!Driving.calibration_printed)
  {
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
    Driving.calibration_printed = true;
  }

  if(!Serial.available())
  {
    return false;
  }
  input = Serial.read();
  if(input == '+')
  {
    if(offset < 50)
    {
      offset++;
      Serial.print("Currnet offset: ");
      Serial.println(offset);
    }
  }
  else if (input == '-')
  {
    if(offset > 0)
    {
      offset--;
      Serial.print("Currnet offset: ");
      Serial.println(offset);
    }
  }
  else if(input == 's')
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
    return true;
  }

  return false;
}

bool check_sensors()
{
  if(!Sensors.header_printed)
  {
    Serial.println("Last but not least - the RGB sensors");
    Serial.println("The values of the sensors will be printed on the screen.");
    Serial.println("The values of each sensor will be printed until you press any key");
    Serial.println("Please make sure that the values changes when the color of the led changes");
    Sensors.header_printed = true;
  }

  if(!Sensors.front_printed)
    Serial.println("front sensor:");
  if(!check_sensor(RgbaFront))
    return false;

  if(!Sensors.front_right_printed)
    Serial.println("front right sensor:");
  if(!check_sensor(RgbaFrontRight))
    return false;

  if(!Sensors.right_printed)
    Serial.println("right sensor:");
  if(!check_sensor(RgbaRight))
    return false;

  if(!Sensors.rear_right_printed)
    Serial.println("rear right sensor:");
  if(!check_sensor(RgbaRearRight))
    return false;

  if(!Sensors.rear_printed)
    Serial.println("rear sensor:");
  if(!check_sensor(RgbaRear))
    return false;

  if(!Sensors.rear_left_printed)
    Serial.println("rear left sensor:");
  if(!check_sensor(RgbaRearLeft))
    return false;

  if(!Sensors.left_printed)
    Serial.println("left sensor:");
  if(!check_sensor(RgbaLeft))
    return false;

  if(!Sensors.front_left_printed)
    Serial.println("front left sensor:");
  if(!check_sensor(RgbaFrontLeft))
    return false;

  return true;

}

bool check_sensor (RGBASensor &sensor)
{

  if(!Serial.available())
  {
    return false;
  }
  else
  {
    sensor.print();
  }
  Serial.println("O.K. moving to the next sensor.");
  return true;
}

void wait(int period)
{
  Timer timer;
  timer.setPeriod(period)
  while (!timer.finished());
}
