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

#ifndef BATTERY_H
#define BATTERY_H

#include "application.h"
#include "SandTimer/SandTimer.h"

#define BATTERY_LVL_LEG A4
#define CHARGING_LVL_LEG A5
#define IS_FULL_CHARGE_LEG D6
#define IS_CHARGINE_LEG DAC

#define MAX_BAT_LVL 4.12
#define MIN_BAT_LVL 3.3
#define MAX_CHRG_LVL 5.0
#define MIN_CHRG_LVL 0.0

#define BATTERY_SAMPLE_INTERVAL 100

class Battery
{
private:
  float battery_voltage;
  float alpha = 0.1;
  void Lpf(float read);
  SandTimer timer;
public:

  Battery();
  float readBatLvl(); //lvl in Volt
  uint8_t getBatLvl(); //lvl in %
  float readChargelvl(); //lvl in Volt
  uint8_t getChargeLvl(); //lvl in %
  bool isCharging();
  bool isFull();
  void print();
  void loop();
  float getBatVolt();

};

#endif //BATTERY_H
