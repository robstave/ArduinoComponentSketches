# ACS-85-0551
Melody Maker Upper 2
==============

## Overview:
One control for speed
Notes are played on three pins with each pin updating in a round robin fashion.
There are rests in this case.  Its still pretty tough to listen too.
Similar to ACS-85-0550

AND/OR/XOR them or or just sum them using resistors


## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0551/images/ACS-85-0551.png)

## Equivalent:
This is not like the 4017/4051 Melody Generator

## Examples:
This is a sample of all 3 pins summed up and playing at the same time.

https://soundcloud.com/user-692410397/85-0551a


One way to do this is similar to [ACS-85-0550](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0550) is external clocked lfsr


![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0550/images/ACS-85-0550-example.png)


## Improvement ideas.
There is a pin left.  Either as a mode input or and output (xor ?)


 
