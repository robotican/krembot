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

//This sketch is for testing the krembots.
//It tests all of the krembot's components - leds, bumpers, motors, imu and rgb sensors.
//Its designf as state machine - you can cont to the next phase only after the
//previous phase ended successfully. In every phase, instructions will be printed.

//This sketch implements krembots API for the tests.

//If the krembot does not enter to flashing mode, try to power off the
//krembot and wait until the led on the photon is breathing cyan, or if
//this sketch was the last uploaded firmware, wait until the red leds
//will turn on,




#include "Krembot/krembot.h"
#include "Krembot/SandTimer/SandTimer.h"



enum PHASE
{
  IMU,
  LEDS,
  BUMPERS,
  DRIVING,
  SENSORS,
  BATTERY,
  NONE,
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

struct BumpersPhase
{

  BumperState front = BumperState::UNPRESSED,
              right = BumperState::UNPRESSED,
              rear = BumperState::UNPRESSED,
              left = BumperState::UNPRESSED,
              front_right = BumperState::UNPRESSED,
              rear_right = BumperState::UNPRESSED,
              rear_left = BumperState::UNPRESSED,
              front_left = BumperState::UNPRESSED;

  bool front_printed = false,
       front_right_printed = false,
       right_printed = false,
       rear_right_printed = false,
       rear_printed = false,
       rear_left_printed = false,
       left_printed = false,
       front_left_printed = false;


};

struct Driving
{
  bool calibrating_left = false,
       calibrating_right = false;

  bool header_printed = false,
       case_printed = false,
       menu_printed = false,
       calibration_printed = false;
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

struct Imu
{
  bool imu_printed = false,
       init_error_printed = false;
};

bool battery_header_printed = false;

Krembot krembot;
SandTimer drive_timer;
SandTimer print_timer;
SandTimer delay_timer;

BumpersRes results;
PHASE current_phase;
Leds leds;
Battery battery;
BumpersPhase bumpers;
Driving driving;
Imu imu;
Sensors sensors;

char input;
int8_t offset_added;
bool done_printed = false;
bool main_menu_printed = false;
int selection = 0;



void setup()
{
    wait(5000);
    krembot.setup();
    //while(!Serial.available()){}
    ///Serial.read();
    Serial.println("Welcome To Krembot Self Tests");
    print_timer.setPeriod(5000);
    drive_timer.setPeriod(3000);
    delay_timer.setPeriod(1000);

    current_phase = PHASE::NONE;
    Serial.println("\n************************************\n");

    if(krembot.getVersion() == Version::V1)
    {
      bumpers.front_right = BumperState::UNDEFINED,
      bumpers.rear_right = BumperState::UNDEFINED,
      bumpers.rear_left = BumperState::UNDEFINED,
      bumpers.front_left = BumperState::UNDEFINED;
    }
}

void loop()
{
  krembot.loop();
  if(current_phase == PHASE::NONE)
  {
    if(print_main_menu(selection))
    {
      switch (selection)
      {
        case 1:
        {
          current_phase = PHASE::IMU;
          selection = 0;
          break;
        }

        case 2:
        {
          current_phase = PHASE::BUMPERS;
          selection = 0;
          break;
        }

        case 3:
        {
          current_phase = PHASE::DRIVING;
          selection = 0;
          break;
        }

        case 4:
        {
          current_phase = PHASE::LEDS;
          selection = 0;
          break;
        }

        case 5:
        {
          current_phase = PHASE::SENSORS;
          selection = 0;
          break;
        }

        case 6:
        {
          current_phase = PHASE::BATTERY;
          selection = 0;
          break;
        }

        case 9:
        {
          current_phase = PHASE::DONE;
          selection = 0;
          break;
        }

        default:
        {
          current_phase = PHASE::NONE;
          selection = 0;
        }
      }
   }
 }

  if(current_phase != PHASE::NONE)
  {
    if(current_phase == PHASE::IMU)
    {
      if(check_imu())
      {
        current_phase = PHASE::NONE;
        Serial.println("\n************************************\n");
      }
    }
    else if(current_phase == PHASE::BUMPERS)
    {

      if(check_bumpers())
      {
        current_phase = PHASE::NONE;
        Serial.println("\n************************************\n");
      }
    }
    else if(current_phase == PHASE::DRIVING)
    {
      if(check_driving())
      {
        current_phase = PHASE::NONE;
        Serial.println("\n************************************\n");
      }
    }
    else if(current_phase == PHASE::LEDS)
    {
      if(check_leds())
      {
        current_phase = PHASE::NONE;
        Serial.println("\n************************************\n");
      }
    }
    else if(current_phase == PHASE::SENSORS)
    {
      if(check_sensors())
      {
        current_phase = PHASE::NONE;
        Serial.println("\n************************************\n");
      }
    }
    else if(current_phase == PHASE::BATTERY)
    {
      if(check_battery())
      {
        current_phase = PHASE::NONE;
        Serial.println("\n************************************\n");
      }
    }
    else if(current_phase == PHASE::DONE)
    {
      if(!done_printed)
      {
        Serial.println("******************self test done***********************");
        done_printed = true;
      }
    }
  }
}

bool print_main_menu(int &selection)
{
  if(!main_menu_printed)
  {
    Serial.println("Main menu:");
    Serial.println("1. check imu");
    Serial.println("2. check bumpers");
    Serial.println("3. check driving");
    Serial.println("4. check leds");
    Serial.println("5. check rgba sensors");
    Serial.println("6. check battery");
    Serial.println("9. exit ");
    main_menu_printed = true;
  }
  if (!Serial.available())
    return false;

  selection = Serial.parseInt();
  if(selection > 0 && selection < 10)
  {
    main_menu_printed = false;
    return true;
  }
  return false;

}

bool check_leds ()
{
  if (!leds.red_printed)
  {
    Serial.println("Let's start with the leds");
    wait(1000);
    krembot.Led.write(255,0,0);
    Serial.println("Look at the leds located on the krembot's head. Are all of them red? (y/n)");
    leds.red_printed = true;
  }

  if(!leds.red)
  {
    if (!Serial.available())
      return false;
    input = (char)Serial.read();
    if (input != 'y' && input != 'Y')
    {
     Serial.println("error");
     return false;
    }
    else
    {
      Serial.println("great. lets continue");
      leds.red = true;
      Serial.println("------------------------------------\n");
    }
  }

  if(!leds.green_printed)
  {
    Serial.println("Look at the leds again. Are all of them green? (y/n)");
    wait(1000);
    krembot.Led.write(0,255,0);
    leds.green_printed = true;
  }

  if(!leds.green)
  {
    if (!Serial.available())
      return false;
    input = (char)Serial.read();
    if (input != 'y' && input != 'Y')
    {
     Serial.println("error");
     return false;
    }
    else
    {
      leds.green = true;
      Serial.println("great. lets continue");
      Serial.println("------------------------------------\n");
    }
  }

  if(!leds.blue_printed)
  {
    wait(1000);
    krembot.Led.write(0,0,255);
    Serial.println("Now look at the leds for the last time. Are all of them blue? (y/n)");
    leds.blue_printed = true;
  }
  if(!leds.blue)
  {
    if (!Serial.available())
      return false;
    input = (char)Serial.read();
    if (input != 'y' && input != 'Y')
    {
     Serial.println("error");
     return false;
    }
    else
    {
      leds.blue = true;
      Serial.println("------------------------------------\n");
      krembot.Led.write(0,0,0);
      Serial.println("great. Leds phase Done");
      return true;
    }
  }

  return false;

}

bool check_bumpers()
{
  if(!bumpers.front_printed && bumpers.front != BumperState::UNDEFINED)
  {
    Serial.println("Let's continue with the bumpers");
    wait(1000);
    Serial.println("Please press only the front bumper.");
    bumpers.front_printed = true;
  }
  if(bumpers.front == BumperState::UNPRESSED)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(results.front == BumperState::PRESSED && results.front_right != BumperState::PRESSED && results.right != BumperState::PRESSED &&
      results.rear_right != BumperState::PRESSED && results.rear != BumperState::PRESSED && results.rear_left != BumperState::PRESSED &&
      results.left  != BumperState::PRESSED&& results.front_left != BumperState::PRESSED)
    {
      bumpers.front = BumperState::PRESSED;
      Serial.println("great. I see the front bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.front_right_printed && bumpers.front_right != BumperState::UNDEFINED)
  {
    Serial.println("Please press only the front right bumper.");
    bumpers.front_right_printed = true;
  }
  if(bumpers.front_right == BumperState::UNPRESSED)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(results.front != BumperState::PRESSED && results.front_right == BumperState::PRESSED && results.right != BumperState::PRESSED &&
      results.rear_right != BumperState::PRESSED && results.rear != BumperState::PRESSED && results.rear_left != BumperState::PRESSED &&
      results.left  != BumperState::PRESSED&& results.front_left != BumperState::PRESSED)
    {
      bumpers.front_right = BumperState::PRESSED;
      Serial.println("great. I see the front right bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.right_printed && bumpers.right != BumperState::UNDEFINED)
  {
    Serial.println("Please press only the right bumper.");
    bumpers.right_printed = true;
  }
  if(bumpers.right == BumperState::UNPRESSED)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(results.front != BumperState::PRESSED && results.front_right != BumperState::PRESSED && results.right == BumperState::PRESSED &&
      results.rear_right != BumperState::PRESSED && results.rear != BumperState::PRESSED && results.rear_left != BumperState::PRESSED &&
      results.left  != BumperState::PRESSED&& results.front_left != BumperState::PRESSED)
    {
      bumpers.right = BumperState::PRESSED;
      Serial.println("great. I see the right bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.rear_right_printed && bumpers.rear_right != BumperState::UNDEFINED)
  {
    Serial.println("Please press only the rear right bumper.");
    bumpers.rear_right_printed = true;
  }
  if(bumpers.rear_right == BumperState::UNPRESSED)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(results.front != BumperState::PRESSED && results.front_right != BumperState::PRESSED && results.right != BumperState::PRESSED &&
      results.rear_right == BumperState::PRESSED && results.rear != BumperState::PRESSED && results.rear_left != BumperState::PRESSED &&
      results.left  != BumperState::PRESSED&& results.front_left != BumperState::PRESSED)
    {
      bumpers.rear_right = BumperState::PRESSED;
      Serial.println("great. I see the rear right bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.rear_printed && bumpers.rear != BumperState::UNDEFINED)
  {
    Serial.println("Please press only the rear bumper.");
    bumpers.rear_printed = true;
  }
  if(bumpers.rear == BumperState::UNPRESSED)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(results.front != BumperState::PRESSED && results.front_right != BumperState::PRESSED && results.right != BumperState::PRESSED &&
      results.rear_right != BumperState::PRESSED && results.rear == BumperState::PRESSED && results.rear_left != BumperState::PRESSED &&
      results.left  != BumperState::PRESSED&& results.front_left != BumperState::PRESSED)
    {
      bumpers.rear = BumperState::PRESSED;
      Serial.println("great. I see the rear bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.rear_left_printed && bumpers.rear_left != BumperState::UNDEFINED)
  {
    Serial.println("Please press only the rear left bumper.");
    bumpers.rear_left_printed = true;
  }
  if(bumpers.rear_left == BumperState::UNPRESSED)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(results.front != BumperState::PRESSED && results.front_right != BumperState::PRESSED && results.right != BumperState::PRESSED &&
      results.rear_right != BumperState::PRESSED && results.rear != BumperState::PRESSED && results.rear_left == BumperState::PRESSED &&
      results.left  != BumperState::PRESSED&& results.front_left != BumperState::PRESSED)
    {
      bumpers.rear_left = BumperState::PRESSED;
      Serial.println("great. I see the rear left bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.left_printed && bumpers.left != BumperState::UNDEFINED)
  {
    Serial.println("Please press only the left bumper.");
    bumpers.left_printed = true;
  }
  if(bumpers.left == BumperState::UNPRESSED)
  {
    clear_bumpers_results();

    results = krembot.Bumpers.read();
    if(results.front != BumperState::PRESSED && results.front_right != BumperState::PRESSED && results.right != BumperState::PRESSED &&
      results.rear_right != BumperState::PRESSED && results.rear != BumperState::PRESSED && results.rear_left != BumperState::PRESSED &&
      results.left  == BumperState::PRESSED&& results.front_left != BumperState::PRESSED)
    {
      bumpers.left = BumperState::PRESSED;
      Serial.println("great. I see the left bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
      if(krembot.getVersion() == Version::V1)
      {
        return true;
      }
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.front_left_printed && bumpers.front_left != BumperState::UNDEFINED)
  {
    Serial.println("Please press only the front left bumper.");
    bumpers.front_left_printed = true;
  }
  if(bumpers.front_left == BumperState::UNPRESSED)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(results.front != BumperState::PRESSED && results.front_right != BumperState::PRESSED && results.right != BumperState::PRESSED &&
      results.rear_right != BumperState::PRESSED && results.rear != BumperState::PRESSED && results.rear_left != BumperState::PRESSED &&
      results.left  != BumperState::PRESSED&& results.front_left == BumperState::PRESSED)
    {
      bumpers.front_left = BumperState::PRESSED;
      Serial.println("great. I see the front left bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
      return true;
    }
    else
    {
      return false;
    }
  }

  return false;

}



bool check_driving()
{
  if(!driving.header_printed)
  {
    Serial.println("Let's check the krembot's movement");
    wait(1000);
    driving.header_printed = true;
  }
  int result = 8;
  do
  {
    if(!driving.menu_printed)
    {
      print_driving_menu();
      driving.menu_printed = true;
    }

    if(!Serial.available() && !driving.calibrating_left && !driving.calibrating_right)
    {
      return false;
    }

    result = Serial.parseInt();
    if(driving.calibrating_right)
    {
      result = 5;
    }
    else if(driving.calibrating_left)
    {
      result = 6;
    }

    switch (result)
    {
      case 1:
      {
        Serial.println("In 3 Seconds the robot will move forward for 3 seconds.");
        Serial.println("Please make sure that the robot is on flat surface and have enough space.");
        Serial.println("Please look if the robot is moving in a straight line.");
        wait(3000);
        krembot.Base.drive(30, 0);
        drive_timer.start();
        while(!drive_timer.finished()){}
        krembot.Base.stop();
        Serial.println("If the robot didn't move in straight line, the motors may need calibration.");
        Serial.println("In case the robot turned right, the right motor needs calibration.");
        Serial.println("In case the robot turned left, the left motor needs calibration.");
        Serial.println("You can choose the calibration mode you need the next time that the menu will be printed");
        Serial.println("------------------------------------\n");
        wait(3000);
        driving.menu_printed = false;
        break;
      }

      case 2:
      {
        Serial.println("In 3 Seconds the robot will move backword for 3 seconds.");
        Serial.println("Please make sure that the robot is on flat surface and have enough space.");
        Serial.println("Please look if the robot is moving in a straight line.");
        wait(3000);
        krembot.Base.drive(-30, 0);
        drive_timer.start();
        while(!drive_timer.finished()){}
        krembot.Base.stop();
        Serial.println("If the robot didn't move in straight line, the motors may need calibration.");
        Serial.println("In case the robot turned right, the right motor needs calibration.");
        Serial.println("In case the robot turned left, the left motor needs calibration.");
        Serial.println("You can choose the calibration mode you need the next time that the menu will be printed");
        driving.menu_printed = false;
        Serial.println("------------------------------------\n");
        wait(3000);
        break;
      }

      case 3:
      {
        Serial.println("In 3 Seconds the robot will rotate left for 3 seconds.");
        Serial.println("Please make sure that the robot is on flat surface and have enough space.");
        Serial.println("Please look if the robot is rotating left ");
        wait(3000);
        krembot.Base.drive(0, 30);
        drive_timer.start();
        while(!drive_timer.finished()){}
        krembot.Base.stop();
        Serial.println("If the robot didn't rotate left, the motors may need calibration.");
        Serial.println("You can choose the calibration mode you need the next time that the menu will be printed");
        driving.menu_printed = false;
        Serial.println("------------------------------------\n");
        wait(3000);
        break;
      }

      case 4:
      {
        Serial.println("In 3 Seconds the robot will rotate right for 3 seconds.");
        Serial.println("Please make sure that the robot is on flat surface and have enough space.");
        Serial.println("Please look if the robot is rotating right");
        wait(3000);
        krembot.Base.drive(0, -30);
        drive_timer.start();
        while(!drive_timer.finished()){}
        krembot.Base.stop();
        Serial.println("If the robot didn't rotate right, the motors may need calibration.");
        Serial.println("You can choose the calibration mode you need the next time that the menu will be printed");
        driving.menu_printed = false;
        Serial.println("------------------------------------\n");
        wait(3000);
        break;
      }

      case 5:
      {
        driving.calibrating_right = true;
        if(calibration_mode(true)) //calibrating right
        {
          driving.menu_printed = false;
          driving.calibration_printed = false;

          wait(3000);
        }
        break;
      }

      case 6:
      {
        driving.calibrating_left = true;
        if(calibration_mode(false)) //calibrating left
        {
          driving.menu_printed = false;
          driving.calibration_printed = false;
          wait(3000);
        }
        break;
      }

      case 9:
      {
        return true;
      }

      default:
      {
        return false;
      }
    }
  } while(result != 9);

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
  Serial.println("9. exit ");
}

bool calibration_mode(bool isRightMotor)
{
  int8_t offset;
  if(isRightMotor)
    driving.calibrating_right = true;
  else driving.calibrating_left = true;

  if(isRightMotor)
  {
    offset = EEPROM.read(BASE_RIGHT_OFFSET_ADDR);
  }
  else
  {
    offset = EEPROM.read(BASE_LEFT_OFFSET_ADDR);
  }
  char input = '0';
  if(!driving.calibration_printed)
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
    driving.calibration_printed = true;

  }
  while(!Serial.available()) {}
  //Serial.println("reading input");
  input = Serial.read();
  //Serial.println(input);
  if(input == '+')
  {
    if((offset + offset_added) < 50)
    {
      offset_added++;
      Serial.print("Currnet offset: ");
      Serial.println(offset + offset_added);
    }
  }
  else if (input == '-')
  {
    if((offset + offset_added) > 0)
    {
      offset_added--;
      Serial.print("Currnet offset: ");
      Serial.println(offset + offset_added);
    }
  }
  else if(input == 's')
  {
    Serial.print("Saved offset: ");
    if(isRightMotor)
    {
      EEPROM.write(BASE_RIGHT_OFFSET_ADDR, offset + offset_added);
    }
    else
    {
      EEPROM.write(BASE_LEFT_OFFSET_ADDR, offset + offset_added);
    }
    Serial.println(offset + offset_added);
    Serial.println("------------------------------------\n");
    wait(3000);
    driving.calibrating_right = false;
    driving.calibrating_left = false;
    return true;
  }

  return false;
}

bool check_imu()
{
  if(krembot.Imu.InitFlags.address && krembot.Imu.InitFlags.online)
  {
    if(!imu.imu_printed)
    {
      Serial.println("Now we will check the IMU");
      Serial.println("The roll, pitch and yaw of the IMU will be printed on the screen.");
      Serial.println("It will be printed until you press 's'");
      Serial.println("Please make sure that the values changes when you tilt the robot");
      Serial.println("------------------------------------\n");
      imu.imu_printed = true;
      print_timer.start();
    }
    if(!print_timer.finished())
    {
      return false;
    }
    delay_timer.start();
    if(!delay_timer.finished())
    {
      return false;
    }
    krembot.Imu.printRaw();

    Serial.println("");
    if(Serial.available())
    {
      input = Serial.read();
      if(input == 's')
      {
        Serial.println("O.K. moving to the next phase.");
        return true;
      }
    }
  }
  else if(!imu.init_error_printed)
  {
    Serial.println("IMU initialization error");
    imu.init_error_printed = true;
  }
    return false;
}

bool check_sensors()
{
  if(!sensors.header_printed)
  {
    Serial.println("now we will check the RGB sensors");
    Serial.println("The values of the sensors will be printed on the screen.");
    Serial.println("The values of each sensor will be printed until you press 's'");
    Serial.println("Please make sure that the values changes when the color of the led changes");
    Serial.println("------------------------------------\n");
    wait(5000);
    sensors.header_printed = true;
  }

  if(!sensors.front_printed)
  {
    Serial.print("front sensor: ");
  }

  if(!sensors.front)
  {
    sensors.front = check_sensor(krembot.RgbaFront);
    if(sensors.front)
    {
      sensors.front_printed = true;
    }
    return false;

  }

  if(!sensors.front_right_printed)
  {
    Serial.print("front right sensor: ");
  }

  if(!sensors.front_right)
  {
    sensors.front_right = check_sensor(krembot.RgbaFrontRight);
    if(sensors.front_right)
    {
      sensors.front_right_printed = true;
    }
    return false;
  }


  if(!sensors.right_printed)
  {
    Serial.print("right sensor: ");
  }

  if(!sensors.right)
  {
    sensors.right = check_sensor(krembot.RgbaRight);
    if(sensors.right)
    {
      sensors.right_printed = true;
    }
    return false;
  }


  if(!sensors.rear_right_printed)
  {
    Serial.print("rear right sensor: ");
  }

  if(!sensors.rear_right)
  {
    sensors.rear_right = check_sensor(krembot.RgbaRearRight);
    if(sensors.rear_right)
    {
      sensors.rear_right_printed = true;
    }
    return false;
  }


  if(!sensors.rear_printed)
  {
    Serial.print("rear sensor: ");
  }

  if(!sensors.rear)
  {
    sensors.rear = check_sensor(krembot.RgbaRear);
    if(sensors.rear)
    {
      sensors.rear_printed = true;
    }
    return false;
  }


  if(!sensors.rear_left_printed)
  {
    Serial.print("rear left sensor: ");
  }

  if(!sensors.rear_left)
  {
    sensors.rear_left = check_sensor(krembot.RgbaRearLeft);
    if(sensors.rear_left)
    {
      sensors.rear_left_printed = true;
    }
    return false;
  }



  if(!sensors.left_printed)
  {
    Serial.print("left sensor: ");
  }

  if(!sensors.left)
  {
    sensors.left = check_sensor(krembot.RgbaLeft);
    if(sensors.left)
    {
      sensors.left_printed = true;
    }
    return false;
  }

  if(!sensors.front_left_printed)
  {
    Serial.print("front left sensor: ");
  }

  if(!sensors.front_left)
  {
    sensors.front_left = check_sensor(krembot.RgbaFrontLeft);
    if(sensors.front_left)
    {
      sensors.front_left_printed = true;
    }
    return false;
  }


  if(sensors.front && sensors.front_right && sensors.right && sensors.rear_right
     && sensors.rear && sensors.rear_left && sensors.left && sensors.front_left)
  {
    return true;
  }


  return false;

}

bool check_sensor (RGBASensor &sensor)
{
  sensor.print();
    wait(2000);
    if(Serial.available())
    {
      input = Serial.read();
      if(input == 's')
      {
        Serial.println("O.K. moving to the next sensor\n");
        return true;
      }
    }
  return false;
}


bool check_battery()
{
  if(!battery_header_printed)
  {
    Serial.println("Now we will check the battery");
    Serial.println("The Raw Battery read, Battery level, Charge Level, charging status (Yes/No) and battery full status(Yes/No) will be printed");
    Serial.println("It will be printed until you press 's'");
    Serial.println("Please make sure that the values are correct. charge and stop charging the robot and check if the status changes");
    Serial.println("------------------------------------\n");
    wait(5000);
    battery_header_printed = true;
  }
    battery.print();
    wait(2000);
    Serial.println("");
    if(Serial.available())
    {
      input = Serial.read();
      if(input == 's')
      {
        Serial.println("O.K. moving to the next phase.");
        return true;
      }
    }
    return false;
}

void wait(int period)
{
  SandTimer timer;
  timer.setPeriod(period);
  timer.start();
  while (!timer.finished());
}

void clear_bumpers_results()
{
  results.front = BumperState::UNPRESSED;
  results.right = BumperState::UNPRESSED;
  results.rear = BumperState::UNPRESSED;
  results.left = BumperState::UNPRESSED;

  if(krembot.getVersion() == Version::V2)
  {
    results.front_right = BumperState::UNPRESSED;
    results.rear_left = BumperState::UNPRESSED;
    results.rear_right = BumperState::UNPRESSED;
    results.front_left = BumperState::UNPRESSED;
  }


}
