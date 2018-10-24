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



#include "cbumpers.h"


CBumpers::CBumpers()
{

}

void CBumpers::init(Version hwVersion)
{
	version = hwVersion;

	if(version == Version::V2)
	{
		mux_.begin(MUX_ADDR);

		mux_.pinMode(BUMPER_FRONT, INPUT_PULLUP);
		mux_.pinMode(BUMPER_FRONT_RIGHT, INPUT_PULLUP);
		mux_.pinMode(BUMPER_RIGHT, INPUT_PULLUP);
		mux_.pinMode(BUMPER_REAR_RIGHT, INPUT_PULLUP);
		mux_.pinMode(BUMPER_REAR, INPUT_PULLUP);
		mux_.pinMode(BUMPER_REAR_LEFT, INPUT_PULLUP);
		mux_.pinMode(BUMPER_LEFT, INPUT_PULLUP);
		mux_.pinMode(BUMPER_FRONT_LEFT, INPUT_PULLUP);
	}

	else if (version == Version::V1)
	{
		pinMode(BUMPERS_LEG, INPUT);
		EEPROM.get(2, bumps_calib_);
	}
}

BumpersRes CBumpers::read()
{
	BumpersRes res;

	if(version == Version::V2)
	{
		res.front = BumperState::UNPRESSED;
		res.front_right = BumperState::UNPRESSED;
		res.right = BumperState::UNPRESSED;
		res.rear_right = BumperState::UNPRESSED;
		res.rear = BumperState::UNPRESSED;
		res.rear_left = BumperState::UNPRESSED;
		res.left = BumperState::UNPRESSED;
		res.front_left = BumperState::UNPRESSED;

		if (!mux_.digitalRead(BUMPER_FRONT))
		{
			res.front = BumperState::PRESSED;
		}
		if (!mux_.digitalRead(BUMPER_FRONT_RIGHT))
		{
			res.front_right =  BumperState::PRESSED;
		}
		if (!mux_.digitalRead(BUMPER_RIGHT))
		{
			res.right =  BumperState::PRESSED;
		}
		if (!mux_.digitalRead(BUMPER_REAR_RIGHT))
		{
			res.rear_right =  BumperState::PRESSED;
		}
		if (!mux_.digitalRead(BUMPER_REAR))
		{
			res.rear =  BumperState::PRESSED;
		}
		if (!mux_.digitalRead(BUMPER_REAR_LEFT))
		{
			res.rear_left =  BumperState::PRESSED;
		}
		if (!mux_.digitalRead(BUMPER_LEFT))
		{
			res.left =  BumperState::PRESSED;
		}
		if (!mux_.digitalRead(BUMPER_FRONT_LEFT))
		{
			res.front_left = BumperState::PRESSED;
		}
	}
	else if (version == Version::V1)
	{

		res.front = BumperState::UNPRESSED;
		res.right = BumperState::UNPRESSED;
		res.rear = BumperState::UNPRESSED;
		res.left = BumperState::UNPRESSED;

		uint16_t read_val = readRaw();

	  if (read_val > (bumps_calib_.left - TOLERANCE) &&
	           read_val < (bumps_calib_.left + TOLERANCE))
	           res.left = BumperState::PRESSED;
	  else if (read_val > (bumps_calib_.right - TOLERANCE) &&
	           read_val < (bumps_calib_.right + TOLERANCE))
	           res.right = BumperState::PRESSED;
	  else if (read_val > (bumps_calib_.right_left - TOLERANCE) &&
	           read_val < (bumps_calib_.right_left + TOLERANCE))
	           {
	             res.right = BumperState::PRESSED;
	             res.left = BumperState::PRESSED;
	           }
	  else if (read_val > (bumps_calib_.rear - TOLERANCE) &&
	          read_val < (bumps_calib_.rear + TOLERANCE))
	          res.rear = BumperState::PRESSED;
	  else if (read_val > (bumps_calib_.rear_left - TOLERANCE) &&
	           read_val < (bumps_calib_.rear_left + TOLERANCE))
	           {
	             res.rear = BumperState::PRESSED;
	             res.left = BumperState::PRESSED;
	           }
	  else if (read_val > (bumps_calib_.rear_right - TOLERANCE) &&
	          read_val < (bumps_calib_.rear_right + TOLERANCE))
	          {
	            res.rear = BumperState::PRESSED;
	            res.right = BumperState::PRESSED;
	          }
	  else if (read_val > (bumps_calib_.rear_left_right - TOLERANCE) &&
	           read_val < (bumps_calib_.rear_left_right + TOLERANCE))
	           {
	             res.rear = BumperState::PRESSED;
	             res.left = BumperState::PRESSED;
	             res.right = BumperState::PRESSED;
	           }
	  else if (read_val > (bumps_calib_.front - TOLERANCE) &&
	          read_val < (bumps_calib_.front + TOLERANCE))
	          res.front = BumperState::PRESSED;
	  else if (read_val > (bumps_calib_.front_left - TOLERANCE) &&
	           read_val < (bumps_calib_.front_left + TOLERANCE))
	           {
	             res.front = BumperState::PRESSED;
	             res.left = BumperState::PRESSED;
	           }
	  else if (read_val > (bumps_calib_.front_right - TOLERANCE) &&
	          read_val < (bumps_calib_.front_right + TOLERANCE))
	          {
	            res.front = BumperState::PRESSED;
	            res.right = BumperState::PRESSED;
	          }
	  else if (read_val > (bumps_calib_.front_right_left - TOLERANCE) &&
	           read_val < (bumps_calib_.front_right_left + TOLERANCE))
	           {
	             res.front = BumperState::PRESSED;
	             res.right = BumperState::PRESSED;
	             res.left = BumperState::PRESSED;
	           }
	  else if (read_val > (bumps_calib_.front_rear - TOLERANCE) &&
	          read_val < (bumps_calib_.front_rear + TOLERANCE))
	          {
	            res.front = BumperState::PRESSED;
	            res.rear = BumperState::PRESSED;
	          }
	  else if (read_val > (bumps_calib_.front_rear_left - TOLERANCE) &&
	           read_val < (bumps_calib_.front_rear_left + TOLERANCE))
	           {
	             res.front = BumperState::PRESSED;
	             res.rear = BumperState::PRESSED;
	             res.left = BumperState::PRESSED;
	           }
	  else if (read_val > (bumps_calib_.front_rear_right - TOLERANCE) &&
	          read_val < (bumps_calib_.front_rear_right + TOLERANCE))
	          {
	            res.front = BumperState::PRESSED;
	            res.rear = BumperState::PRESSED;
	            res.right = BumperState::PRESSED;
	          }
	  else if (read_val > (bumps_calib_.front_rear_right_left - TOLERANCE) &&
	           read_val < (bumps_calib_.front_rear_right_left + TOLERANCE))
	           {
	             res.front = BumperState::PRESSED;
	             res.rear = BumperState::PRESSED;
	             res.right = BumperState::PRESSED;
	             res.left = BumperState::PRESSED;
	           }
	}

	return res;
}

void CBumpers::print()
{
	Serial.print("Raw Bumpers: "); Serial.println(analogRead(BUMPERS_LEG));

	Serial.print("[Bumpers] Bumpers Pressed: ");
	BumpersRes res;

	res = read();
	if (res.front == BumperState::PRESSED)
		Serial.print("|   FRONT   |");

	if (res.front_right == BumperState::PRESSED)
		Serial.print("|FRONT RIGHT|");

	if (res.right == BumperState::PRESSED)
		Serial.print("|   RIGHT   |");

	if (res.rear_right == BumperState::PRESSED)
		Serial.print("|REAR  RIGHT|");

	if (res.rear == BumperState::PRESSED)
		Serial.print("|   REAR    |");

	if (res.rear_left == BumperState::PRESSED)
		Serial.print("| REAR LEFT |");

	if (res.left == BumperState::PRESSED)
		Serial.print("|   LEFT    |");

	if (res.front_left == BumperState::PRESSED)
		Serial.print("|FRONT LEFT|");

	Serial.println();

}


/*
* publishes the bupmper read result
* to the cloud. starts from the front
* bumper, clockwise.
* 1 indicates pressed bumpers, 0 indicates unpressed bumpers
*/
void CBumpers::publish()
{
	String publishStr = "[Bumpers]: ";
	BumpersRes res;

	res = read();
	if (res.front == BumperState::PRESSED)
	{
		publishStr.concat("1, ");
	}
	else if (res.front == BumperState::UNPRESSED)
	{
		publishStr.concat("0, ");
	}

	if (res.front_right == BumperState::PRESSED)
	{
		publishStr.concat("1, ");
	}
	else if (res.front_right == BumperState::UNPRESSED)
	{
		publishStr.concat("0, ");
	}


	if (res.right == BumperState::PRESSED)
	{
		publishStr.concat("1, ");
	}
	else if (res.right == BumperState::UNPRESSED)
	{
		publishStr.concat("0, ");
	}

	if (res.rear_right == BumperState::PRESSED)
	{
		publishStr.concat("1, ");
	}
	else if (res.rear_right == BumperState::UNPRESSED)
	{
		publishStr.concat("0, ");
	}

	if (res.rear == BumperState::PRESSED)
	{
		publishStr.concat("1, ");
	}
	else if (res.rear == BumperState::UNPRESSED)
	{
		publishStr.concat("0, ");
	}

	if (res.rear_left == BumperState::PRESSED)
	{
		publishStr.concat("1, ");
	}
	else if (res.rear_left == BumperState::UNPRESSED)
	{
		publishStr.concat("0, ");
	}


	if (res.left == BumperState::PRESSED)
	{
		publishStr.concat("1");

	}
	else if (res.left == BumperState::UNPRESSED)
	{
		publishStr.concat("0");
	}

	if(version == Version::V2)
	{
		publishStr.concat(", ");
	}

	if (res.front_left == BumperState::PRESSED)
	{
		publishStr.concat("1");
	}
	else if (res.front_left == BumperState::UNPRESSED)
	{
		publishStr.concat("0");
	}

	Particle.publish("bumpers", publishStr, PRIVATE);

}

void CBumpers::countDown(uint8_t duration)
{
  for (int i=0; i< duration; i++)
  {
    Serial.printf("%i...", i + 1);
    delay(1000);
  }
  Serial.println();
}

uint16_t CBumpers::readRaw()
{
	return analogRead(BUMPERS_LEG);
}

void CBumpers::printCalib()
{
  Serial.println("----------------BUMPS CALIB----------------");
  Serial.printf("left: %i \n", bumps_calib_.left);
  Serial.printf("right: %i \n", bumps_calib_.right);
  Serial.printf("right_left: %i \n", bumps_calib_.right_left);
  Serial.printf("rear: %i \n", bumps_calib_.rear);
  Serial.printf("rear_left: %i \n", bumps_calib_.rear_left);
  Serial.printf("rear_right: %i \n", bumps_calib_.rear_right);
  Serial.printf("rear_left_right: %i \n", bumps_calib_.rear_left_right);
  Serial.printf("front: %i \n", bumps_calib_.front);
  Serial.printf("front_left: %i \n", bumps_calib_.front_left);
  Serial.printf("front_right: %i \n", bumps_calib_.front_right);
  Serial.printf("front_right_left: %i \n", bumps_calib_.front_right_left);
  Serial.printf("front_rear: %i \n", bumps_calib_.front_rear);
  Serial.printf("front_rear_left: %i \n", bumps_calib_.front_rear_left);
  Serial.printf("front_rear_right: %i \n", bumps_calib_.front_rear_right);
  Serial.printf("front_rear_right_left: %i \n", bumps_calib_.front_rear_right_left);
  Serial.println("-------------------------------------------");
}

//return true when done calibration
bool CBumpers::calib()
{
	switch (calib_stage_)
  {
    case 0:
      Serial.println("0. Press LEFT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.left = readRaw();
      calib_stage_++;
      return false;
    break;

    case 1:
      Serial.println("1. Press RIGHT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.right = readRaw();
      calib_stage_++;
      return false;
    break;

    case 2:
      Serial.println("2. Press RIGHT_LEFT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.right_left = readRaw();
      calib_stage_++;
      return false;
    break;

    case 3:
      Serial.println("3. Press REAR");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.rear = readRaw();
      calib_stage_++;
      return false;
    break;

    case 4:
      Serial.println("4. Press REAR_LEFT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.rear_left = readRaw();
      calib_stage_++;
      return false;
    break;

    case 5:
      Serial.println("5. Press REAR_RIGHT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.rear_right = readRaw();
      calib_stage_++;
      return false;
    break;

    case 6:
      Serial.println("6. Press REAR_LEFT_RIGHT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.rear_left_right = readRaw();
      calib_stage_++;
      return false;
    break;

    case 7:
      Serial.println("7. Press FRONT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.front = readRaw();
      calib_stage_++;
      return false;
    break;

    case 8:
      Serial.println("8. Press FRONT_LEFT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.front_left = readRaw();
      calib_stage_++;
      return false;
    break;

    case 9:
      Serial.println("9. Press FRONT_RIGHT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.front_right = readRaw();
      calib_stage_++;
      return false;
    break;

    case 10:
      Serial.println("10. Press FRONT_RIGHT_LEFT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.front_right_left = readRaw();
      calib_stage_++;
      return false;
    break;

    case 11:
      Serial.println("11. Press FRONT_REAR");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.front_rear = readRaw();
      calib_stage_++;
      return false;
    break;

    case 12:
      Serial.println("12. Press FRONT_REAR_LEFT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.front_rear_left = readRaw();
      calib_stage_++;
      return false;
    break;

    case 13:
      Serial.println("13. Press FRONT_REAR_RIGHT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.front_rear_right = readRaw();
      calib_stage_++;
      return false;
    break;

    case 14:
      Serial.println("15. Press FRONT_REAR_RIGHT_LEFT");
      countDown(CALIB_TIMEOUT);
      bumps_calib_.front_rear_right_left = readRaw();
      calib_stage_++;
      return false;
    break;

    case 15: //done
      EEPROM.put(2, bumps_calib_);
      Serial.println("Done");
      calib_stage_ = 0;
      return true;
    break;
  }
}
