#Bumpers
**KREMEBOt_2.0** has 8 bumpers which helps it to sense when it bumps into objects. 
The bumpers are located around the robot(front, front right, right, rear right, rear, rear left, left and front left), 
and can be accessed through the Bumpers class.

You can detect which bumper is pressed by invoking:
```c++
BumpersRes read();
```

This function returns BumpersRes struct that contains the following fields:
```c++
struct BumpersRes
{
    bool front,
         front_right,
         right,
         rear_right,
         rear,
         rear_left,
         left,
         front_left;
};
```

If a bumper is pressed, the value of the corresponding field will be true.

***

**KREMEBOt_1.0** has 4 bumpers which helps it to sense when it bumps into objects. 
The bumpers are located around the robot (front, right, left and rear) 
and can be accessed through the Bumpers class:

You can detect which bumper is pressed by invoking:

```c++
BumpersRes read();
```

This function returns BumpersRes struct that contains the following fields:
```c++
struct BumpersRes
{
  bool front,
       rear,
       right,
       left;
};
```


If a bumper is pressed, the value of the corresponding field will be true.





