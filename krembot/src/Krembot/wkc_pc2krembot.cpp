

#include "wkc_pc2krembot.h"

void WKCPC2Krembot::fromBytes(byte bytes_arr[])
{
  //extract flags
  data_req = BitConverter::getBitInByte(bytes_arr[FLAGS_INDX],
                                        DATA_REQ_BIT);
  toggle_led = BitConverter::getBitInByte(bytes_arr[FLAGS_INDX],
                                          TOGGLE_LED_BIT);
  joy_control = BitConverter::getBitInByte(bytes_arr[FLAGS_INDX],
                                           JOY_CTRL_BIT);
  base_offset = BitConverter::getBitInByte(bytes_arr[FLAGS_INDX],
                                           BASE_OFFSET_BIT);
  bumps_calib = BitConverter::getBitInByte(bytes_arr[FLAGS_INDX],
                                          BUMPS_CALIB_BIT);
  linear_vel_positive = BitConverter::getBitInByte(bytes_arr[FLAGS_INDX],
                                                   LINEAR_VEL_POSITIVE);
  angular_vel_positive = BitConverter::getBitInByte(bytes_arr[FLAGS_INDX],
                                                    ANGULAR_VEL_POSITIVE);

  //extract joystick axes
  joy_x = (uint8_t)bytes_arr[JOY_X_INDX];
  joy_y = (uint8_t)bytes_arr[JOY_Y_INDX];

  led_red = (uint8_t)bytes_arr[LED_RED_INDX];
  led_green = (uint8_t)bytes_arr[LED_GREEN_INDX];
  led_blue = (uint8_t)bytes_arr[LED_BLUE_INDX];

  base_right_offset = (uint8_t)bytes_arr[BASE_RIGHT_OFFSET];
  base_left_offset = (uint8_t)bytes_arr[BASE_LEFT_OFFSET];

  user_msg_size = (uint8_t)bytes_arr[USER_MSG_SIZE_INDX];
}

void WKCPC2Krembot::print()
{
  Serial.println("-------- WKCPC2Krembot pkg content: --------");
  Serial.printf("data_req?: %d\n", data_req);
  Serial.printf("toggle_led?: %d\n", toggle_led);
  Serial.printf("joy_control?: %d\n", joy_control);
  Serial.printf("linear_vel_positive?: %d\n", linear_vel_positive);
  Serial.printf("angular_vel_positive?: %d\n", angular_vel_positive);
  Serial.printf("base_offset?: %d\n", base_offset);
  Serial.printf("joy_x: %d\n", joy_x);
  Serial.printf("joy_y: %d\n", joy_y);
  Serial.printf("base_right_offset: %d\n", base_right_offset);
  Serial.printf("base_left_offset: %d\n", base_left_offset);
  Serial.printf("led_red: %d\n", led_red);
  Serial.printf("led_green: %d\n", led_green);
  Serial.printf("led_blue: %d\n", led_blue);
  Serial.printf("user_msg_size: %d\n", user_msg_size);
  Serial.println("-------------------------------------------");
}
