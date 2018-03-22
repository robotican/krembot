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
  int8_t mapByteToBaseVal(uint8_t val);
public:
  MobileBase();
  bool drive(int8_t linear_spd, int8_t angular_spd);
  void standby();
  void stop();
};

#endif
