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

/* Author: Elchay Rauper */


#ifndef RGBA_SENSOR_H
#define RGBA_SENSOR_H

#include "application.h"
#include "math.h"
#include "SparkFun_APDS9960.h"
#define MUX_ADDR 0x70


/**
* @brief Holds the values related to the color sensor
* 
*/
struct RGBAResult
{
  uint16_t Ambient;   /**< The Ambience of the light, (0-65535)*/
  uint16_t Red;       /**< The Red value of the light, (0-65535)*/
  uint16_t Green;     /**< The Green value of the light, (0-65535)*/
  uint16_t Blue;      /**< The Blue value of the light, (0-65535)*/
  uint8_t Proximity;  /**< The Raw value of the proximity sensor, (0-255)*/
  float Distance;     /**< The calculated distance to the object, in cm*/
  uint8_t ErrCode;    /**< The error code returned by the read function*/
  bool IsReadOk;      /**<  True if the reading was successful, false otherwise*/
};


class RGBASensor
{
private:

  uint8_t addr_;

  SparkFun_APDS9960 apds_;
  bool i2cMuxSelectMe();


public:

  
  
  void init(uint8_t addr);  

  /** 
  *   @brief  Reads the values from sensor.
  *   
  *   @return RGBAResult, a struct that holds the values of the sensor
  */    

  RGBAResult read();

  /** 
  *   @brief  Prints the values of the sensor.
  *   
  *   @return void
  */    
  void print();
};


#endif
