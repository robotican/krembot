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

#include "rgba_sensor.h"

void RGBASensor::init(uint8_t addr)
{
  addr_ = addr;
  apds_ = SparkFun_APDS9960();
  i2cMuxSelectMe();

  if (!apds_.init() )
       Serial.println(F("[RGBA sensor]Something went wrong during APDS-9960 init sensor" ));

  if (!apds_.enableLightSensor(false) )
      Serial.println(F("[RGBA sensor]Something went wrong during light sensor init! sensor" ));

  if ( !apds_.setProximityGain(PGAIN_2X) )
    Serial.println(F("[RGBA sensor]Something went wrong trying to set PGAIN sensor sensor" ));

  if ( !apds_.enableProximitySensor(false) )
     Serial.println(F("[RGBA sensor]Something went wrong during sensor init! sensor"));
  //Serial.println();
}

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
  i2cMuxSelectMe();
  if (!apds_.readAmbientLight(result.Ambient))
  {
    result.AmbientError = true;
    Serial.print("[RGBA sensor] - Ambient sensor error");

  }
  if(!apds_.readRedLight(result.Red))
  {
    result.RedError = true;
    Serial.print("[RGBA sensor] - Red sensor error");
  }
  if(!apds_.readGreenLight(result.Green))
  {
    result.GreenError = true;
    Serial.print("[RGBA sensor] - Green sensor error");
  }
  if(!apds_.readBlueLight(result.Blue))
  {
    result.BlueError = true;
    Serial.print("[RGBA sensor] - Blue sensor error");
  }
  if(!apds_.readProximity(result.Proximity))
  {
    result.ProximityError = true;
    Serial.print("[RGBA sensor] - Proximity sensor error");
  }
  else
  { //convert proximity to distance (cm)
    if (result.Proximity < 20) //min bound - read below it is not reliable
      result.Proximity = 20;
    result.Distance = 117.55 * pow(result.Proximity, -0.51); //result min val is 6, and max is 25 cm
  }
  return result;
}

void RGBASensor::print()
{
  RGBAResult read_res = read();
  if (read_res.AmbientError && read_res.RedError && read_res.GreenError && read_res.AmbientError && read_res.BlueError && read_res.ProximityError)
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


HSVResult RGBASensor::rgbToHSV(RGBAResult in)
{
  HSVResult         out;
  double      min, max, delta;

  min = in.Red < in.Green ? in.Red : in.Green;
  min = min  < in.Blue ? min  : in.Blue;

  max = in.Red > in.Green ? in.Red : in.Green;
  max = max  > in.Blue ? max  : in.Blue;

  out.V = max;                                // v
  delta = max - min;
  if (delta < 0.00001)
  {
      out.S = 0;
      out.H = 0; // undefined, maybe nan?
      return out;
  }
  if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
      out.S = (delta / max);                  // s
  } else {
      // if max is 0, then r = g = b = 0
      // s = 0, h is undefined
      out.S = 0.0;
      out.H = NAN;                            // its now undefined
      return out;
  }
  if( in.Red >= max )                           // > is bogus, just keeps compilor happy
      out.H = ( in.Green - in.Blue ) / delta;        // between yellow & magenta
  else
  if( in.Green >= max )
      out.H = 2.0 + ( in.Blue - in.Red ) / delta;  // between cyan & yellow
  else
      out.H = 4.0 + ( in.Red - in.Green ) / delta;  // between magenta & cyan

  out.H *= 60.0;                              // degrees

  if( out.H < 0.0 )
      out.H += 360.0;

  return out;
}

Colors RGBASensor::WhichColor(RGBAResult rgbaIn, HSVResult hsvIn)
{

  if(hsvIn.S < 0.5)
  {
    return Colors::None;
  }
  else if(rgbaIn.Distance < 12 && rgbaIn.Ambient < 200)
  {
    return Colors::None;
  }

  else
  {
    if(hsvIn.H > 85 && hsvIn.H < 165)
    {
      return Colors::Green;
    }
    else if(hsvIn.H > 175 && hsvIn.H < 270)
    {
      return Colors::Blue;
    }

    else if(hsvIn.H > 330 || hsvIn.H < 30)
    {
      return Colors::Red;
    }

  }

  return Colors::None;

}
