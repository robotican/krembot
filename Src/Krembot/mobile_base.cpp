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


#include "mobile_base.h"

MobileBase::MobileBase()
{
  //  Serial.println("initialization engines");
  pinMode(LEFT_MOTOR_DIR_LEG1, OUTPUT);
  pinMode(LEFT_MOTOR_DIR_LEG2, OUTPUT);
  pinMode(LEFT_MOTOR_PWM_LEG, OUTPUT);

  pinMode(RIGHT_MOTOR_DIR_LEG1, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR_LEG2, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM_LEG, OUTPUT);

  pinMode(MOTOR_STBY_LEG, OUTPUT);
  digitalWrite(MOTOR_STBY_LEG, LOW);
}

void MobileBase::setMotorDirection(Motor motor, Direction direction)
{
  if (motor == LEFT)
  {
    if (direction == Direction::FORWARD)
    {
      digitalWrite(LEFT_MOTOR_DIR_LEG1, HIGH);
      digitalWrite(LEFT_MOTOR_DIR_LEG2, LOW);
    }
    else //backward
    {
      digitalWrite(LEFT_MOTOR_DIR_LEG1, LOW);
      digitalWrite(LEFT_MOTOR_DIR_LEG2, HIGH);
    }
  }
  else //right motor
  {
    if (direction == Direction::FORWARD)
    {
      digitalWrite(RIGHT_MOTOR_DIR_LEG1, LOW);
      digitalWrite(RIGHT_MOTOR_DIR_LEG2, HIGH);
    }
    else //backward
    {
      digitalWrite(RIGHT_MOTOR_DIR_LEG1, HIGH);
      digitalWrite(RIGHT_MOTOR_DIR_LEG2, LOW);
    }
  }
}

bool MobileBase::drive(int8_t linear_spd, int8_t angular_spd)
{
  //convert x and y to linear and angular speeds
  if ((linear_spd < -100 || linear_spd > 100) ||
     (angular_spd < -100 || angular_spd > 100))
     return false;

  //Serial.printf("linear_spd: %d\n", linear_spd);
  //Serial.printf("angular_spd: %d\n", angular_spd);

  int8_t right_offset = EEPROM.read(BASE_RIGHT_OFFSET_ADDR);
  int8_t left_offset = EEPROM.read(BASE_LEFT_OFFSET_ADDR);

  //Serial.printf("right_offset: %d\n", right_offset);
  //Serial.printf("left_offset: %d\n", left_offset);

  digitalWrite(MOTOR_STBY_LEG, HIGH);

  int linear_scale = int((linear_spd / 100.0) * 255.0);
  int angular_scale = int((angular_spd / 100.0) * 255.0);

  //Serial.print("linear_scale: "); Serial.println(linear_scale);
  //Serial.print("angular_scale: "); Serial.println(angular_scale);

  int left_cmd = linear_scale + angular_scale;
  int right_cmd = linear_scale - angular_scale;

  //Serial.print("left cmd: "); Serial.println(left_cmd);
  //Serial.print("right cmd: "); Serial.println(right_cmd);



  if (left_cmd > 0)
    setMotorDirection(Motor::LEFT, Direction::FORWARD);
  else
  {
    setMotorDirection(Motor::LEFT, Direction::BACKWARD);
    left_cmd = -left_cmd;
  }
  if (right_cmd > 0)
    setMotorDirection(Motor::RIGHT, Direction::FORWARD);
  else
  {
    setMotorDirection(Motor::RIGHT, Direction::BACKWARD);
    right_cmd = -right_cmd;
  }

 // we decided on direction for each wheel,
 // now we can use absolute values to calculate
 // final values after offset additions
 left_cmd = abs(left_cmd);
 right_cmd = abs(right_cmd);

 if (left_cmd != 0)
   left_cmd += left_offset;
 if (right_cmd != 0)
   right_cmd += right_offset;

 if (left_cmd > 255)
  left_cmd = 255;
 else if (left_cmd < -255)
  left_cmd = -255;

 if (right_cmd > 255)
  right_cmd = 255;
 else if (right_cmd < -255)
  right_cmd = -255;

  //Serial.print("left cmd = : "); Serial.println(left_cmd);
  //Serial.print("right cmd = : "); Serial.println(right_cmd);

  analogWrite(LEFT_MOTOR_PWM_LEG, left_cmd);
  analogWrite(RIGHT_MOTOR_PWM_LEG, right_cmd);

  return true;
}

void MobileBase::stop()
{
  digitalWrite(MOTOR_STBY_LEG, LOW);
}
