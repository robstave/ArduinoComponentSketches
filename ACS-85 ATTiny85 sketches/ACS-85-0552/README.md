# ACS-85-0552

Melody Maker Upper 3

## Overview

One control for speed

Notes are played on three pins with each pin updating in a round robin fashion.
There are rests in this case and the notes go up and down.  Its a bit more melodic, perhaps.

It is similar to [ACS-85-0550](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0550)

Each output is independant so you can AND/OR/XOR them or or just sum them using resistors.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0552/images/ACS-85-0552.png)


## Examples

This is a sample of all 3 pins summed up and playing at the same time. (sorry sound is kinda crappy)

https://soundcloud.com/user-692410397/85-0552a

One way to do this is similar to [ACS-85-0550](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0550)

## Other Examples

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0550/images/ACS-85-0550-example.png)


Here is a sample of just one of the pins (pb2)
https://soundcloud.com/user-692410397/85-0552-1a


## Improvement ideas

There is a pin left.  Either as a mode input or and output (xor ?)
