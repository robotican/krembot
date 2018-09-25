##Battery and Charge monitoring
The KREMEBOt battery and charging status can be read through the Bat class.

To read battery level (0-100%):
```c++
uint8_t getBatLvl();
```

To check if the robot is in charging state:
```c++
bool isCharging();
```
