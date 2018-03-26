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

#include "battery.h"

/** 
  *   @brief 
  *  
  *   @param 
  *   
  *   @return 
  *
  */

/*
  *    Constructor
  */

Battery::Battery()
{
  pinMode(BATTERY_LVL_LEG, INPUT);
  pinMode(CHARGING_LVL_LEG, INPUT);
  pinMode(IS_FULL_CHARGE_LEG, INPUT);
  pinMode(IS_CHARGINE_LEG, INPUT);
}

/** 
  *   @brief Reads the battery level
  *  
  *   @param 
  *   
  *   @return float, the battery level in Volts
  *
  */


float Battery::readBatLvl()
{
  return (analogRead(BATTERY_LVL_LEG) * 3.3 * 1.5 * 1.0201) / 4095.0;
}

/** 
  *   @brief Reads the charge level
  *  
  *   @param 
  *   
  *   @return float, the charge level in Volts
  *
  */

float Battery::readChargelvl()
{
  return (analogRead(CHARGING_LVL_LEG) * 3.3 * (5.0 / 3.0)) / 4095.0;
}


/** 
  *   @brief Prints data about the battery
  *  
  *   @param 
  *   
  *   @return 
  *
  */
void Battery::print()
{
  //TODO: use printf instead of println
  Serial.println("------------Battry Values------------");
  Serial.print("Raw Battery read: "); Serial.print(analogRead(BATTERY_LVL_LEG));
  Serial.print(" | Battery level: "); Serial.print(readBatLvl());
  Serial.print(" | Charge Level: "); Serial.print(readChargelvl());
  Serial.print(" | Is Charging: "); Serial.print(isCharging() == false ? "No" : "Yes" );
  Serial.print(" | Is Full: "); Serial.println(isFull() == false ? "No" : "Yes" );
}


/** 
  *   @brief Checks if the battery is charging 
  *  
  *   @param 
  *   
  *   @return true if the battery is charging, false otherwise
  *
  */

bool Battery::isCharging()
{
  return digitalRead(IS_CHARGINE_LEG) ==  LOW ? true : false;
}

/** 
  *   @brief Checks if the battery is full 
  *  
  *   @param 
  *   
  *   @return true if the battery is full, false otherwise
  *
  */


bool Battery::isFull()
{
  return digitalRead(IS_FULL_CHARGE_LEG) == LOW ? true : false;
}

/** 
  *   @brief gets the battery level in percentage
  *  
  *   @param 
  *   
  *   @return uint8_t, the battery level
  *
  */

uint8_t Battery::getBatLvl()
{
  return (uint8_t)((readBatLvl() - MIN_BAT_LVL) / (MAX_BAT_LVL - MIN_BAT_LVL) * 100);
}
/** 
  *   @brief gets the battery charge level in percentage
  *  
  *   @param 
  *   
  *   @return uint8_t, the charge level
  *
  */
uint8_t Battery::getChargeLvl()
{
  return (uint8_t)((readChargelvl() / MAX_CHRG_LVL) * 100);
}
