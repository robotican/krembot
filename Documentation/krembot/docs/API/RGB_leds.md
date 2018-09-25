
##RGB Leds

In order to set the KREMEBOt color, use the Led class with the write function:

```c++
void write(uint8_t red_val, uint8_t green_val, uint8_t blue_val);
```

Each color argument is in the rang of 0-255.

For example, to set the robot color to blue:

```c++
krembot.Led.write(0,0,255);
```
