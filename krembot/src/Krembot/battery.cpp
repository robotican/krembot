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
