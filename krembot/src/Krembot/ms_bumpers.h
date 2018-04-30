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

/* Author: Elchay Rauper and Yair Shlomi*/

#ifndef MS_BUMPERS_H

#define MS_BUMPERS_H
#include <Wire.h> //i2c
#include "SparkFunSX1509.h" //mux




#define MUX_ADDR 0x3E
#define BUMPER_FRONT 7
#define BUMPER_FRONT_RIGHT 0
#define BUMPER_RIGHT 1
#define BUMPER_REAR_RIGHT 2
#define BUMPER_REAR 3
#define BUMPER_REAR_LEFT 4
#define BUMPER_LEFT 5
#define BUMPER_FRONT_LEFT 6


/** 
  *   @brief 
  *  
  *   @param 
  *   
  *   @return 
  *
  */


struct BumpersRes
{
  bool front = false,
       front_right = false,
       right = false,
       rear_right = false,
       rear = false,
       rear_left = false,
       left = false,
       front_left = false;
};


class MSBumpers
{

private:
  SX1509 mux_;

public:

/* 
  *   brief Constructor 
  *  
  *   param 
  *   
  *   return 
  *
  */

  MSBumpers()
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
    
    //read();
  }

  
/** 
  *   @brief Read if bumpers were pressed
  *  
  *   @param 
  *   
  *   @return the bumpers that was pressed
  *
  */

  BumpersRes read()
  {
    BumpersRes res;
    if(!mux_.digitalRead(BUMPER_FRONT))
    {
      res.front = true;
    }
    if(!mux_.digitalRead(BUMPER_FRONT_RIGHT))
    {
      res.front_right = true;
    }
    if(!mux_.digitalRead(BUMPER_RIGHT))
    {
      res.right = true;
    }
    if(!mux_.digitalRead(BUMPER_REAR_RIGHT))
    {
      res.rear_right = true;
    }
    if(!mux_.digitalRead(BUMPER_REAR))
    {
      res.rear = true;
    }
    if(!mux_.digitalRead(BUMPER_REAR_LEFT))
    {
      res.rear_left = true;
    }
    if(!mux_.digitalRead(BUMPER_LEFT))
    {
      res.left = true;
    }
    if(!mux_.digitalRead(BUMPER_FRONT_LEFT))
    {
      res.front_left = true;
    }
    return res;
  }

/** 
  *   @brief prints which bumpers were pressed
  *  
  *   @param 
  *   
  *   @return 
  *
  */

  void print()
  {
    Serial.print("Bumpers Pressed: ");
    BumpersRes res;
    res = read();
    if (res.front)
      Serial.print("|   FRONT   |");
    if (res.front_right)
      Serial.print("|FRONT RIGHT|");
    if (res.right)
      Serial.print("|   RIGHT   |");
    if (res.rear_right)
      Serial.print("|REAR  RIGHT|");
    if (res.rear)
      Serial.print("|   REAR    |");
    if (res.rear_left)
      Serial.print("| REAR LEFT |");
    if (res.left)
      Serial.print("|   LEFT    |");
    if (res.front_left)
      Serial.print("|FRONT LEFT |");
    Serial.println();
  }

  
};



#endif
