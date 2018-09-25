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

/* Author: Elhay Rauper */
/* Maintainer: Yair Shlomi */

#ifndef KREMBO_H
#define KREMBO_H

#include "application.h"
#include "battery.h"
#include "mobile_base.h"
#include "rgb_led.h"
#include "rgba_sensor.h"
#include "cbumpers.h"
#include "imu_sensor.h"
#include "SandTimer/SandTimer.h"


class Krembot
{
private:

  String my_name_;
  void saveMyName(const char *topic, const char *data);
  SandTimer battery_level_timer_;
  SandTimer battery_level_counter_;

public:

  RGBASensor RgbaFront;
  RGBASensor RgbaRear;
  RGBASensor RgbaRight;
  RGBASensor RgbaLeft;
  RGBASensor RgbaFrontRight;
  RGBASensor RgbaFrontLeft;
  RGBASensor RgbaRearRight;
  RGBASensor RgbaRearLeft;

  MobileBase Base;
  CBumpers Bumpers;
  Battery Bat;
  RGBLed Led;
  IMUSensor Imu;


  void reset(const char *topic, const char *data) {
    if (strcmp(topic,"reset")==0 && (strcmp(data,"all")==0 || strcmp(data,getName().c_str())==0) ) {
      System.reset();
    }
  }

  void setup();

  void loop();

  String getID() { return System.deviceID(); }

  String getName() { return  my_name_; }

  bool have_name() {return my_name_.length()>0 ;}
};

#endif
