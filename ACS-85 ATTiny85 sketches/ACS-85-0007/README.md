# ACS-85-0007
Square VCO with freq ramping
==============

## Overview:
ATTiny85  Square wave with variable frequency.
The period changes in this case using counters. Each time the pin flips, the period is increased. The size of the increase is masked by 3,4 or 5 bits. Since the period increases, the frequncy decreases.  

Its much more of a dramatic ramp sound for PB2
 

## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0007/images/ACS-85-0007.png)] (https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004/images)

## Equivalent:

## Examples:

https://soundcloud.com/user-692410397/85-0007a

## Improvements

There is an open pin left that can be used to toggle either having the ramp go up  or down
