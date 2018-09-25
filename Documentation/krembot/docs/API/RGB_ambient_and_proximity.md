##RGB ambient and proximity

KREMEBOt sense the world through 8 color sensors, which are able to read RGB values, 
proximity (distance) from sensor, and ambient value.

The sensor instances are named by the physical location on the KREMEBOt (8 directions):
**RgbaFront**  
**RgbaFrontRight**  
**RgbaRight**  
**RgbaRearRight**  
**RgbaRear**  
**RgbaRearLeft**  
**RgbaLeft**  
**RgbaFrontLeft**  

Each of the above is a RGBASensor class, and contains reading data is done using the read command:

```c++
RGBAResult read();
```
returns RGBAResult struct with the following fields:

```c++
struct RGBAResult
{
uint16_t Ambient ;  // The Ambience of the light
uint16_t Red;       // The Red value of the light
uint16_t Green;     // The Green value of the light
uint16_t Blue;      // The Blue value of the light
uint8_t Proximity;  // The Raw value of the proximity sensor
float Distance;     // The calculated distance to the object in cm
};

```

RGBASensor class also supports the HSV color format.
To convert the RGB values to HSV values, use:
```c++
static HSVResult rgbToHSV(RGBAResult in);
```

this functions takes a RGBAResult and returns HSVResult strucrt, contains:
```c++
double H,S, V; //Hue, Saturation, Value
```

To extract a color from the HSV values, use:
```c++
static Colors WhichColor(RGBAResult rgbaIn, HSVResult hsvIn);
```
which takes the RGBAResult and HSVResult structs as parameters and return a Color enum value:
```c++
enum Colors
{
  Red,
  Green,
  Blue,
  None
};
```
