#ifndef DAC_BUMPERS_H
#define DAC_BUMPERS_H

#define BUMPERS_LEG A0
#define TOLERANCE 10
#define CALIB_TIMEOUT 5

#define NO_BUMP 0

/** 
  *   @brief 
  *  
  *   @param 
  *   
  *   @return 
  *
  */

struct BumpCalib
{
  uint16_t left,
          right,
          right_left,
          rear,
          rear_left,
          rear_right,
          rear_left_right,
          front,
          front_left,
          front_right,
          front_right_left,
          front_rear,
          front_rear_left,
          front_rear_right,
          front_rear_right_left;
};

struct BumpersRes
{
  bool front = false,
       rear = false,
       right = false,
       left = false;
};

class DacBumpers
{

private:

  uint8_t calib_stage_ = 0;
  BumpCalib bumps_calib_;

public:

/* 
  *   brief Constructor reads the bumper calibration fron the photon's eeprom
  *  
  *   param 
  *   
  *   return 
  *
  */

  DacBumpers()
  {
    pinMode(BUMPERS_LEG, INPUT);
    EEPROM.get(2, bumps_calib_);
  }

/* 
  *   @brief Prints the calibration data of the bumpers
  *  
  *   @param 
  *   
  *   @return 
  *
  */
  void printCalib()
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
/* 
  *   brief calibrate the numpers
  *  
  *   param 
  *   
  *   return 
  *
*/
  //return true when done calibration
  bool calib()
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

/* 
  *   @brief 
  *  
  *   @param 
  *   
  *   @return 
  *
  */

  void countDown(uint8_t duration)
  {
    for (int i=0; i< duration; i++)
    {
      Serial.printf("%i...", i + 1);
      delay(1000);
    }
    Serial.println();
  }


/* 
  *   brief read raw data from bumper
  *  
  *   param 
  *   
  *   return 
  *
  */

  uint16_t readRaw()
  {
    return analogRead(BUMPERS_LEG);
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
    Serial.print("Raw Bumpers: "); Serial.println(analogRead(BUMPERS_LEG));
    Serial.print("Bumpers Pressed: ");
    BumpersRes res;
    res = read();
    if (res.front)
      Serial.print("|FRONT|");
    if (res.rear)
      Serial.print("|REAR|");
    if (res.right)
      Serial.print("|RIGHT|");
    if (res.left)
      Serial.print("|LEFT|");
    Serial.println();
  }

  
};



#endif
