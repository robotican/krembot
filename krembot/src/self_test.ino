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
#include "Krembot/imu_sensor.h"


enum STAGE
{
  LEDS,
  BUMPERS,
  DRIVING,
  IMU,
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

Krembot krembot;
CustomTimer drive_timer;
BumpersRes results;
STAGE current_stage;
Leds leds;
Bumpers bumpers;
Driving driving;
Imu imu;
Sensors sensors;
char input;
int8_t offset_added;
bool done_printed = false;



void setup()
{
    krembot.setup();
    //while(!Serial.available()){}
    ///Serial.read();
    wait(8000);
    Serial.println("Welcome To Krembot Self Tests");
    drive_timer.setPeriod(3000);
    current_stage = STAGE::LEDS;
    Serial.println("\n************************************\n");
}

void loop()
{
  check_imu();
  if(current_stage == STAGE::LEDS)
  {
    if(check_leds())
    {
      current_stage = STAGE::BUMPERS;
      Serial.println("\n************************************\n");
    }
  }
  else if(current_stage == STAGE::BUMPERS)
  {

    if(check_bumpers())
    {
      current_stage = STAGE::DRIVING;
      Serial.println("\n************************************\n");
    }
  }
  else if(current_stage == STAGE::DRIVING)
  {
    if(check_driving())
    {
      current_stage = STAGE::SENSORS;
      Serial.println("\n************************************\n");
    }
  }
  else if(current_stage == STAGE::SENSORS)
  {
    if(check_sensors())
    {
      current_stage = STAGE::DONE;
      Serial.println("\n************************************\n");
    }
  }
  else if(current_stage == STAGE::DONE)
  {
    if(!done_printed)
    {
      Serial.println("******************self test done***********************");
      done_printed = true;
    }
  }

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
    if (input != 'y')
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
    if (input != 'y')
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
    if (input != 'y')
    {
     Serial.println("error");
     return false;
    }
    else
    {
      leds.blue = true;
      Serial.println("------------------------------------\n");
      krembot.Led.write(0,0,0);
      Serial.println("great. let's move on to the next phase");
      return true;
    }
  }

  return false;

}

bool check_bumpers()
{
  if(!bumpers.front_printed)
  {
    Serial.println("Let's continue with the bumpers");
    wait(1000);
    Serial.println("Please press only the front bumper.");
    bumpers.front_printed = true;
  }
  if(!bumpers.front)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(results.front && !results.front_right && !results.right && !results.rear_right
       && !results.rear && !results.rear_left && !results.left && !results.front_left)
    {
      bumpers.front = true;
      Serial.println("great. I see the front bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.front_right_printed)
  {
    Serial.println("Please press only the front right bumper.");
    bumpers.front_right_printed = true;
  }
  if(!bumpers.front_right)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(!results.front && results.front_right && !results.right && !results.rear_right
       && !results.rear && !results.rear_left && !results.left && !results.front_left)
    {
      bumpers.front_right = true;
      Serial.println("great. I see the front right bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.right_printed)
  {
    Serial.println("Please press only the right bumper.");
    bumpers.right_printed = true;
  }
  if(!bumpers.right)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(!results.front && !results.front_right && results.right && !results.rear_right
       && !results.rear && !results.rear_left && !results.left && !results.front_left)
    {
      bumpers.right = true;
      Serial.println("great. I see the right bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.rear_right_printed)
  {
    Serial.println("Please press only the rear right bumper.");
    bumpers.rear_right_printed = true;
  }
  if(!bumpers.rear_right)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(!results.front && !results.front_right && !results.right && results.rear_right
       && !results.rear && !results.rear_left && !results.left && !results.front_left)
    {
      bumpers.rear_right = true;
      Serial.println("great. I see the rear right bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.rear_printed)
  {
    Serial.println("Please press only the rear bumper.");
    bumpers.rear_printed = true;
  }
  if(!bumpers.rear)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(!results.front && !results.front_right && !results.right && !results.rear_right
       && results.rear && !results.rear_left && !results.left && !results.front_left)
    {
      bumpers.rear = true;
      Serial.println("great. I see the rear bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.rear_left_printed)
  {
    Serial.println("Please press only the rear left bumper.");
    bumpers.rear_left_printed = true;
  }
  if(!bumpers.rear_left)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(!results.front && !results.front_right && !results.right && !results.rear_right
       && !results.rear && results.rear_left && !results.left && !results.front_left)
    {
      bumpers.rear_left = true;
      Serial.println("great. I see the rear left bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.left_printed)
  {
    Serial.println("Please press only the left bumper.");
    bumpers.left_printed = true;
  }
  if(!bumpers.left)
  {
    clear_bumpers_results();

    results = krembot.Bumpers.read();
    if(!results.front && !results.front_right && !results.right && !results.rear_right
       && !results.rear && !results.rear_left && results.left && !results.front_left)
    {
      bumpers.left = true;
      Serial.println("great. I see the left bumper was pressed");
      Serial.println("------------------------------------\n");
      wait(1000);
    }
    else
    {
      return false;
    }
  }

  if(!bumpers.front_left_printed)
  {
    Serial.println("Please press only the front left bumper.");
    bumpers.front_left_printed = true;
  }
  if(!bumpers.front_left)
  {
    clear_bumpers_results();
    results = krembot.Bumpers.read();
    if(!results.front && !results.front_right && !results.right && !results.rear_right
       && !results.rear && !results.rear_left && !results.left && results.front_left)
    {
      bumpers.front_left = true;
      Serial.println("great. I see the front left bumper was pressed");
      Serial.println("------------------------------------\n");
      return true;
      wait(1000);
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
  int result = 9;
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
  if(krembot.imu_init_ok)
  {
    if(!imu.imu_printed)
    {
      Serial.println("Now we will check the IMU");
      Serial.println("The roll, pitch and yaw of the IMU will be printed on the screen.");
      Serial.println("It will be printed until you press 's'");
      Serial.println("Please make sure that the values changes when you tilt the robot");
      Serial.println("------------------------------------\n");
      wait(5000);
      imu.imu_printed = true;
    }
    ImuData data;
    krembot.imu_sensor.read(data);
    Serial.print("Roll: ");
    Serial.print(data.roll);
    Serial.print(", Pitch: ");
    Serial.print(data.pitch);
    Serial.print(", Yaw: ");
    Serial.println(data.yaw);
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
    Serial.println("Last but not least - the RGB sensors");
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
    Serial.println("");
    if(Serial.available())
    {
      input = Serial.read();
      if(input == 's')
      {
        Serial.println("O.K. moving to the next sensor.");
        return true;
      }
    }
  return false;
}

void wait(int period)
{
  CustomTimer timer;
  timer.setPeriod(period);
  timer.start();
  while (!timer.finished());
}

void clear_bumpers_results()
  {
    results.front = false,
    results.front_right = false,
    results.right = false,
    results.rear_right = false,
    results.rear = false,
    results.rear_left = false,
    results.left = false,
    results.front_left = false;
  }
