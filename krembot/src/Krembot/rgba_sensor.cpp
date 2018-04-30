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

/* Author:  Elchay Rauper*/

#include "rgba_sensor.h"
/** 
  *   @brief 
  *  
  *   @param 
  *   
  *   @return 
  *
  */

/** 
  *   @brief  Initializes the sensor.
  *  
  *   @param  addr is a uint8_t containing the address of the sensor (I2C protocol address)
  *   
  *   @return void
  */    

void RGBASensor::init(uint8_t addr)
{
  addr_ = addr;
  apds_ = SparkFun_APDS9960();
  i2cMuxSelectMe();

  if (!apds_.init() )
       Serial.println(F("Something went wrong during APDS-9960 init sensor" ));

  if (!apds_.enableLightSensor(false) )
      Serial.println(F("Something went wrong during light sensor init! sensor" ));

  if ( !apds_.setProximityGain(PGAIN_2X) )
    Serial.println(F("Something went wrong trying to set PGAIN sensor sensor" ));

  if ( !apds_.enableProximitySensor(false) )
     Serial.println(F("Something went wrong during sensor init! sensor"));
  //Serial.println();
}

/* 
  *   brief select to the rgba sensor using the SparkFun_APDS9960
  *  
  *   param 
  *   
  *   return true if the selection to the sensor was succesfull
  *
  */

bool RGBASensor::i2cMuxSelectMe()
{
  if (addr_ > 7)
    return false;
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << addr_);
  Wire.endTransmission();
  return true;
}

RGBAResult RGBASensor::read()
{
  RGBAResult result;
  result.IsReadOk = true;
  i2cMuxSelectMe();
  if (!apds_.readAmbientLight(result.Ambient))
  {
    result.IsReadOk = false;
    result.ErrCode = 0;
    Serial.print("error code 0");

  }
  if(!apds_.readRedLight(result.Red))
  {
    result.IsReadOk = false;
    result.ErrCode = 1;
    Serial.print("error code 1");
  }
  if(!apds_.readGreenLight(result.Green))
  {
    result.IsReadOk = false;
    result.ErrCode = 2;
    Serial.print("error code 2");
  }
  if(!apds_.readBlueLight(result.Blue))
  {
    result.IsReadOk = false;
    result.ErrCode = 3;
    Serial.print("error code 3");
  }
  if(!apds_.readProximity(result.Proximity))
  {
    result.IsReadOk = false;
    result.ErrCode = 4;
    Serial.print("error code 4");
  }
  else
  { //convert proximity to distance (cm)
    if (result.Proximity < 20) //min bound - read below it is not reliable
      result.Proximity = 20;
    result.Distance = 117.55 * pow(result.Proximity, -0.51); //result min val is 6, and max is 25 cm
  }
  return result;
}

/****************************void print()***************************
|@Goal: print rgba sensor values: ambient, red, green, blue, proximity
|@Comment: invoke read() before print() for updated values printing
********************************************************************/

void RGBASensor::print()
{
  RGBAResult read_res = read();
  if (read_res.IsReadOk)
  {
    Serial.println("------------RGBA Sensor Values------------");
    Serial.print("Ambient: "); Serial.print(read_res.Ambient);
    Serial.print(" | Red: ");   Serial.print(read_res.Red);
    Serial.print(" | Green: ");   Serial.print(read_res.Green);
    Serial.print(" | Blue: ");  Serial.print(read_res.Blue);
    Serial.print(" | Distance: ");  Serial.println(read_res.Distance);
  }
  else
  {
    Serial.println("[RGBSensor]: Error reading from sensor");
    
  }
}
