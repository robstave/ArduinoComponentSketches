# ACS-85-0017

 ATTiny85 Clock/counter 4 bit  gate or trigger Like 4017

## Overview

CD4017  Like a baby 10, but a baby 4!

one inputs:

- clock

Three Outputs: like CD4017

- counter out bit q0  
- counter out bit q1
- counter out bit q2
- counter out bit q3

 

However there is also a software bool that toggles between gate mode ( regular ) and a trigger mode.
 

```
0001
0010
0100
1000
```



### Software parameters
  - Trigger mode
  - trigger length  - Not really in milliseconds, just kinda clock counts which are kinda arbitrary.
    Considering that the ATTINY in internal osc is a bit loosy, your results will vary.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0016/images/ACS-85-0017.png)

## Equivalent

CD4017

## Examples
