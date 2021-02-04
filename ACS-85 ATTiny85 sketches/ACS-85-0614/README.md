# ACS-85-0614

Multiple coin flip clocked - output matches clock


## Overview

Similar to Mutable instruments Branches.
Triggers on rising edge of clock and outputs on either a or b pin based on probability of
two sucessive flips.  
With double flips ( or triple), high values are a little more sporatic.

So if you set the prob to 50%, then

- A = 1, B = 0 25% of the time
- A = 0, B = 1 25% of the time
- A = 0, B = 0 50% of the time

Output gate is the same as the input. It is not a trigger nor a latch

A pin sets the value to be 2 or 3 flips really.
basically, if its set for 50%  A will flip 25% and B will flip 25% of the time
The other 50 is no value.

With 3 flips set its 12.5% on either side.  A lotta dead space, but nice for ambient.

Similar to ![ACS-85-0612](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0612), and ![ACS-85-0613](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0613).

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0614/images/ACS-85-0614.png)

 