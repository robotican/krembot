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
#ifdef krembot_V2
	mux_.begin(MUX_ADDR);

	mux_.pinMode(BUMPER_FRONT, INPUT_PULLUP);
	mux_.pinMode(BUMPER_FRONT_RIGHT, INPUT_PULLUP);
	mux_.pinMode(BUMPER_RIGHT, INPUT_PULLUP);
	mux_.pinMode(BUMPER_REAR_RIGHT, INPUT_PULLUP);
	mux_.pinMode(BUMPER_REAR, INPUT_PULLUP);
	mux_.pinMode(BUMPER_REAR_LEFT, INPUT_PULLUP);
	mux_.pinMode(BUMPER_LEFT, INPUT_PULLUP);
	mux_.pinMode(BUMPER_FRONT_LEFT, INPUT_PULLUP);
#endif

#ifdef krembot_V1
	pinMode(BUMPERS_LEG, INPUT);
	EEPROM.get(2, bumps_calib_);
#endif
}

BumpersRes CBumpers::read()
{
	BumpersRes res;

#ifdef krembot_V2
	if (!mux_.digitalRead(BUMPER_FRONT))
	{
		res.front = true;
	}
	if (!mux_.digitalRead(BUMPER_FRONT_RIGHT))
	{
		res.front_right = true;
	}
	if (!mux_.digitalRead(BUMPER_RIGHT))
	{
		res.right = true;
	}
	if (!mux_.digitalRead(BUMPER_REAR_RIGHT))
	{
		res.rear_right = true;
	}
	if (!mux_.digitalRead(BUMPER_REAR))
	{
		res.rear = true;
	}
	if (!mux_.digitalRead(BUMPER_REAR_LEFT))
	{
		res.rear_left = true;
	}
	if (!mux_.digitalRead(BUMPER_LEFT))
	{
		res.left = true;
	}
	if (!mux_.digitalRead(BUMPER_FRONT_LEFT))
	{
		res.front_left = true;
	}
#endif

#ifdef krembot_V1
	uint16_t read_val = readRaw();

  if (read_val > (bumps_calib_.left - TOLERANCE) &&
           read_val < (bumps_calib_.left + TOLERANCE))
           res.left = true;
  else if (read_val > (bumps_calib_.right - TOLERANCE) &&
           read_val < (bumps_calib_.right + TOLERANCE))
           res.right = true;
  else if (read_val > (bumps_calib_.right_left - TOLERANCE) &&
           read_val < (bumps_calib_.right_left + TOLERANCE))
           {
             res.right = true;
             res.left = true;
           }
  else if (read_val > (bumps_calib_.rear - TOLERANCE) &&
          read_val < (bumps_calib_.rear + TOLERANCE))
          res.rear = true;
  else if (read_val > (bumps_calib_.rear_left - TOLERANCE) &&
           read_val < (bumps_calib_.rear_left + TOLERANCE))
           {
             res.rear = true;
             res.left = true;
           }
  else if (read_val > (bumps_calib_.rear_right - TOLERANCE) &&
          read_val < (bumps_calib_.rear_right + TOLERANCE))
          {
            res.rear = true;
            res.right = true;
          }
  else if (read_val > (bumps_calib_.rear_left_right - TOLERANCE) &&
           read_val < (bumps_calib_.rear_left_right + TOLERANCE))
           {
             res.rear = true;
             res.left = true;
             res.right = true;
           }
  else if (read_val > (bumps_calib_.front - TOLERANCE) &&
          read_val < (bumps_calib_.front + TOLERANCE))
          res.front = true;
  else if (read_val > (bumps_calib_.front_left - TOLERANCE) &&
           read_val < (bumps_calib_.front_left + TOLERANCE))
           {
             res.front = true;
             res.left = true;
           }
  else if (read_val > (bumps_calib_.front_right - TOLERANCE) &&
          read_val < (bumps_calib_.front_right + TOLERANCE))
          {
            res.front = true;
            res.right = true;
          }
  else if (read_val > (bumps_calib_.front_right_left - TOLERANCE) &&
           read_val < (bumps_calib_.front_right_left + TOLERANCE))
           {
             res.front = true;
             res.right = true;
             res.left = true;
           }
  else if (read_val > (bumps_calib_.front_rear - TOLERANCE) &&
          read_val < (bumps_calib_.front_rear + TOLERANCE))
          {
            res.front = true;
            res.rear = true;
          }
  else if (read_val > (bumps_calib_.front_rear_left - TOLERANCE) &&
           read_val < (bumps_calib_.front_rear_left + TOLERANCE))
           {
             res.front = true;
             res.rear = true;
             res.left = true;
           }
  else if (read_val > (bumps_calib_.front_rear_right - TOLERANCE) &&
          read_val < (bumps_calib_.front_rear_right + TOLERANCE))
          {
            res.front = true;
            res.rear = true;
            res.right = true;
          }
  else if (read_val > (bumps_calib_.front_rear_right_left - TOLERANCE) &&
           read_val < (bumps_calib_.front_rear_right_left + TOLERANCE))
           {
             res.front = true;
             res.rear = true;
             res.right = true;
             res.left = true;
           }
#endif
	return res;
}

void CBumpers::print()
{
#ifdef krembot_V1
	Serial.print("Raw Bumpers: "); Serial.println(analogRead(BUMPERS_LEG));
#endif

	Serial.print("[Bumpers] Bumpers Pressed: ");
	BumpersRes res;
	res = read();
	if (res.front)
		Serial.print("|   FRONT   |");

#ifdef krembot_V2
	if (res.front_right)
		Serial.print("|FRONT RIGHT|");
#endif

	if (res.right)
		Serial.print("|   RIGHT   |");

#ifdef krembot_V2
	if (res.rear_right)
		Serial.print("|REAR  RIGHT|");
#endif

	if (res.rear)
		Serial.print("|   REAR    |");

#ifdef krembot_V2
	if (res.rear_left)
		Serial.print("| REAR LEFT |");
#endif

	if (res.left)
		Serial.print("|   LEFT    |");

#ifdef krembot_V2
	if (res.front_left)
		Serial.print("|FRONT LEFT|");
#endif
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
	if (res.front)
	{
		publishStr.concat("1, ");
	}
	else
	{
		publishStr.concat("0, ");
	}

#ifdef krembot_V2
	if (res.front_right)
	{
		publishStr.concat("1, ");
	}
	else
	{
		publishStr.concat("0, ");
	}
#endif

	if (res.right)
	{
		publishStr.concat("1, ");
	}
	else
	{
		publishStr.concat("0, ");
	}

#ifdef krembot_V2
	if (res.rear_right)
	{
		publishStr.concat("1, ");
	}
	else
	{
		publishStr.concat("0, ");
	}
#endif

	if (res.rear)
	{
		publishStr.concat("1, ");
	}
	else
	{
		publishStr.concat("0, ");
	}

#ifdef krembot_V2
	if (res.rear_left)
	{
		publishStr.concat("1, ");
	}
	else
	{
		publishStr.concat("0, ");
	}
#endif

	if (res.left)
	{
		publishStr.concat("1");

#ifdef krembot_V2
	publishStr.concat(", ");
#endif

	}
	else
	{
		publishStr.concat("0");

#ifdef krembot_V2
		publishStr.concat(", ");
#endif

	}

#ifdef krembot_V2
	if (res.front_left)
	{
		publishStr.concat("1");
	}
	else
	{
		publishStr.concat("0");
	}
#endif

	Particle.publish("bumpers", publishStr, PRIVATE);

}

#ifdef krembot_V1

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
#endif
