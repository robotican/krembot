
#include "rgb_led.h"

RGBLed::RGBLed()
{
  pinMode(RED_LEG ,OUTPUT);
  pinMode(GREEN_LEG ,OUTPUT);
  pinMode(BLUE_LEG ,OUTPUT);
}

void RGBLed::write(uint8_t red_val,
                   uint8_t green_val,
                   uint8_t blue_val)
{
  analogWrite(RED_LEG, red_val);
  analogWrite(GREEN_LEG, green_val);
  analogWrite(BLUE_LEG, blue_val);
}
