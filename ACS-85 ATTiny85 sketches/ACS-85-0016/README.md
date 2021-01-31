# ACS-85-0016

 ATTiny85 Clock/counter 4 bit  gate or trigger

## Overview

one inputs:

- clock

Three Outputs: like your standard 4 bit counter

- counter out bit q0  
- counter out bit q1
- counter out bit q2
- counter out bit q3

Like a CD40163 I think.   Just a counter

However there is also a software bool that toggles between gate mode ( regular ) and a trigger mode.
This is a little more interesting. It only really toggles when the bit flips, and is a trigger (Adjustable in software).  If you wanted to use this for a drum thing, it would work a little better standalone.

```
Gate     Trigger
0000     0000
0001     0001
0010     0010
0011     0001
0100     0100
0101     0001
0110     0010
0111     0001
1000     1000
1001     0001
1010     0010
1011     0001
1100     0100
1101     0001
1110     0010
1111     0001
```



### Software parameters
  - Trigger mode
  - trigger length  - Not really in milliseconds, just kinda clock counts which are kinda arbitrary.
    Considering that the ATTINY in internal osc is a bit loosy, your results will vary.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0016/images/ACS-85-0016.png)

## Equivalent

A cmost counter

## Examples
