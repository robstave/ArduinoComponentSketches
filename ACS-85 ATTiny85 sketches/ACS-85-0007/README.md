# ACS-85-0007
Square VCO with freq ramping
==============

## Overview:
ATTiny85  Square wave with variable frequency.
The period changes in this case using counters. Each time the pin flips, the period is increased. The size of the increase is masked by 3,4 or 5 bits. Since the period increases, the frequency decreases.  

So for PB0, the ramp in frequency is subtle.  The counter is basically 3 bits, so
that is spinning fast and has a warble.  Sounds like a fatter Squarewave.

For PB1, we double the size of the counter (3 bits to 4) so the lowest frequency is lower and the period for the ramp is less (because its counting more)

Its much more of a dramatic ramp sound for PB2. More of a Raygun sound.
 

## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0007/images/ACS-85-0007.png)

## Equivalent:

## Examples:

https://soundcloud.com/user-692410397/85-0007a

Tests at each pin from PB0 (fat square) to PB2 (Raygun)
## Improvements

There is an open pin left that can be used to toggle either having the ramp go up  or down
