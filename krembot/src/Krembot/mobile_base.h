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

#ifndef MOBILE_BASE_H
#define MOBILE_BASE_H

#include "application.h"

#define LEFT_MOTOR_DIR_LEG1 D4
#define LEFT_MOTOR_DIR_LEG2 D5
#define LEFT_MOTOR_PWM_LEG D2
#define RIGHT_MOTOR_DIR_LEG1 A3
#define RIGHT_MOTOR_DIR_LEG2 A2
#define RIGHT_MOTOR_PWM_LEG D3
#define MOTOR_STBY_LEG A1
#define BASE_RIGHT_OFFSET_ADDR 0
#define BASE_LEFT_OFFSET_ADDR 1

class MobileBase
{
private:
  enum Motor {RIGHT, LEFT};
  enum Direction {FORWARD, BACKWARD};
  void setMotorDirection(Motor motor, Direction direction);
  
public:
  MobileBase();
  bool drive(int8_t linear_spd, int8_t angular_spd);
  void standby();
  void stop();
};

#endif
