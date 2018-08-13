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


#ifndef IMUSENSOR_H
#define IMUSENSOR_H


#include "mpu9250/quaternionFilters.h"
#include "mpu9250/MPU9250.h"
#include "application.h"

#define MAG_CAL_ADDR 100

#define AHRS true         // Set to false for basic data read
#define SerialDebug false  // Set to true to get Serial output for debugging

struct ImuInitErrors
{
    bool imu_address_ok = false,
    	 imu_online = false,
    	 mag_address_ok = false;
};

struct ImuData
{
    float roll = 0,
          pitch = 0,
          yaw = 0;

    float ax = 0,
          ay = 0,
          az = 0;

    float gx = 0,
          gy = 0,
          gz = 0;
};

struct mag_bias_t
{
  int8_t id = 0;
  int16_t x = 0;
  int16_t y = 0;
  int16_t z = 0;
};


class IMUSensor
{
private:
  MPU9250 imu_;
  int16_t mag_temp[3] = {0, 0, 0};
  int16_t mag_min[3] = {0, 0, 0};
  int16_t mag_max[3] = {0, 0, 0};
  mag_bias_t mag_bias;
  static const uint8_t MAG_BIAS_ID = 123;

public:
  ImuInitErrors init();
  void loop();
  ImuData read();
  void print();
  void printRaw();
  void magCalLoop();
  bool calibrationDone = false;
  void resetMagCal();


  ImuInitErrors imuInitErrors;
};

#endif
