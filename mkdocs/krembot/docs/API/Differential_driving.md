
##Differential Driving

KREMEBOt use different driving to control the use of Base class, along with the drive function:

```c++
bool drive(int8_t linear_spd, int8_t angular_spd);
```

where linear_spd and angular_spd are between -100 and 100.

Negative values means reverse for linear_spd, and clockwise rotation for angular_spd.

For example, to drive forward in full speed:

```c++
krembot.Base.drive(100,0);
```

and to turn the robot left(counter-clockwise) in full rotation speed:

```c++
krembot.Base.drive(0,100);
```

The drive function will return false if called with out of range values.

To stop the robot you can use the stop function;

```c++
krembot.Base.stop();
```