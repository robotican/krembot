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

/* Author: Elchay Rauper and Yair Shlomi*/

#ifndef KREMBO_H
#define KREMBO_H

#include "application.h"
#include "battery.h"
#include "mobile_base.h"
#include "rgb_led.h"
#include "rgba_sensor.h"
#include "ms_bumpers.h"
#include "imu_sensor.h"
#include "timer.h"

/*TODO:
1. add connection between photons
2. add errors checking in code
4. implement connection between master and photon (protocol)
5. add documentation
6. add documentation of Kiril wirings
7. add documentation of how to solve problems like breathing green, and how to flash version 0.6.1
8. add photon light functions from photon api
9. add photon timer functions from photon api
10. add photon logging functions from photon api
11. add documentation of wkp protocol
12. add getting photon details functions from photon api (like name)
13. change rgba_sensor to color_sensor
14. initiate Serial.begin inside photon and encapsulte Serial methods
*/

#define SEND_DATA_INTERVAL 100 //ms

enum class RGBAAddr //TODO: extract this to krembot, and make constructor here take int8
{
  Front = 0,
  FrontRight = 1,
  Right = 2,
  RearRight = 3,
  Rear = 4,
  RearLeft = 5,
  Left = 6,
  FrontLeft = 7
};

class Krembot
{
private:

  String my_name_;




  void saveMyName(const char *topic, const char *data);


public:

  RGBASensor RgbaFront; ///< front color sensor
  RGBASensor RgbaRear; ///< rear color sensor
  RGBASensor RgbaRight; ///< right color sensor
  RGBASensor RgbaLeft; ///< left color sensor
  RGBASensor RgbaFrontRight; ///< front right color sensor
  RGBASensor RgbaFrontLeft; ///< front left color sensor
  RGBASensor RgbaRearRight; ///< rear right color sensor
  RGBASensor RgbaRearLeft; ///< rear left color sensor

  MobileBase Base; ///< controls the motors 
  MSBumpers Bumpers; ///< controls the bumper sensors
  Battery Bat; ///< controls the battery
  RGBLed Led; ///< controls the rgb leds

  /** 
  *   @brief  Resets the robot  
  *  
  *   @param  topic is a char array containing the topic
  *   @param  data is a char array containing the data
  *   @return void
  */  
  

  void reset(const char *topic, const char *data) {
    if (strcmp(topic,"reset")==0 && (strcmp(data,"all")==0 || strcmp(data,getName().c_str())==0) ) {
      System.reset();
    }
  }

  /** 
  *   @brief  Setups the robot with the controllers ip and port.
  *  
  *   @param  master_ip is a char array containing the controllers ip
  *   @param  port is an uint16_t containing the controllers port
  *   @return void
  */  

  void setup();

  /** 
  *   @brief  Gets the ID of the robot. 
  *  
  *   @return String, the ID of the robot
  */  
  String getID() { return System.deviceID(); }

  /** 
  *   @brief  Gets the name of the robot. 
  *  
  *   @return String, the name of the robot
  */  
  String getName() { return  my_name_; }

  /** 
  *   @brief  Checks if the robot has name. 
  *  
  *   @return bool, true if the robot's name length is longer then 0, false otherwise.
  */  
  bool have_name() {return my_name_.length()>0 ;}


  /* 
  *   Publishes and prints the battery level of the robot. 
  *  
  *   
  */  
  void pub_battery() {
    char str[50];
    int level= Bat.getBatLvl();
    sprintf(str,"%d",level);
    Particle.publish("Battery level",str);
  }

};

#endif //KREMBO_H
