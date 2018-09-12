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
/* Author:  Elhay Rauper*/

#include "battery.h"

Battery::Battery()
{
	pinMode(BATTERY_LVL_LEG, INPUT);
	pinMode(CHARGING_LVL_LEG, INPUT);
	pinMode(IS_FULL_CHARGE_LEG, INPUT);
	pinMode(IS_CHARGINE_LEG, INPUT);
	battery_voltage = readBatLvl();
	timer.start(BATTERY_SAMPLE_INTERVAL);
	String bat =  String(battery_voltage);
  Particle.publish("battery", bat, PRIVATE);
}

float Battery::readBatLvl()
{
	return (analogRead(BATTERY_LVL_LEG) * 3.3 * 1.5 * 1.0108) / 4095.0;
}

float Battery::readChargelvl()
{
	return (analogRead(CHARGING_LVL_LEG) * 3.3 * (5.0 / 3.0)) / 4095.0;
}

void Battery::print()
{
	//TODO: use printf instead of println
	Serial.println("------------Battry Values------------");
	Serial.print("Raw Battery read: "); Serial.print(analogRead(BATTERY_LVL_LEG));
	Serial.print(" | Battery level: "); Serial.print(getBatLvl());
	Serial.print(" | Charge Level: "); Serial.print(getChargeLvl());
	Serial.print(" | Is Charging: "); Serial.print(isCharging() == false ? "No" : "Yes");
	Serial.print(" | Is Full: "); Serial.println(isFull() == false ? "No" : "Yes");
}

bool Battery::isCharging()
{
	return digitalRead(IS_CHARGINE_LEG) == LOW ? true : false;
}

bool Battery::isFull()
{
	return digitalRead(IS_FULL_CHARGE_LEG) == LOW ? true : false;
}

uint8_t Battery::getBatLvl()
{
	int8_t battery_level = (uint8_t)((battery_voltage - MIN_BAT_LVL) / (MAX_BAT_LVL - MIN_BAT_LVL) * 100);
	if(battery_level > 100)
	{
		return 100;
	}
	else if(battery_level < 0)
	{
		return 0;
	}

	return (uint8_t)battery_level;
}

uint8_t Battery::getChargeLvl()
{
	return (uint8_t)((readChargelvl() / MAX_CHRG_LVL) * 100);
}

void Battery::loop()
{
	if(timer.finished())
	{
			Lpf(readBatLvl());
			timer.startOver();
	}

}

float Battery::getBatVolt()
{
	return battery_voltage;
}

void Battery::Lpf(float read)
{
	battery_voltage = ((alpha*read) + ((1-alpha)*battery_voltage));
}
