##IMU

KREMEBOt_2.0 has 9 DOF IMU sensor. In order to read it, use the Imu object.
```c++
ImuData read();
```
ImuData is a struct that contains the raw and fused data from the Imu sensor:

```c++
struct ImuData
{
    //fused data
    float roll,
          pitch,
          yaw;

    //accelerometer values
    float ax,
          ay,
          az;

    //gyro values
    float gx,
          gy,
          gz;

    //magnetometer values
    float mx,
          my,
          mz;

};
```