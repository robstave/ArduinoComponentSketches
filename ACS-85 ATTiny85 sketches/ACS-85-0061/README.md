# ACS-85-0061

Square wave VCO with Second frequency PWM modulated by LFO


## Overview

Uses a 16 counter in a DDS fashion like. http://www.technoblogy.com/show?QVN

The main difference is, rather than setting up the other counter for PWM, we are using it for an LFO.  The upper 8 bits would be used to point to a 256 size wave table, but since we are doing squarewaves, we can just use the uppermost bit.

There are two accumulators.  One is the main frequency and the other is basically an LFO controlled modulation of that frequency.

To mix, just sum across the pins with resistors  ( or use a real op amp / summer) 

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0061/images/ACS-85-0061.png)
 

## Equivalent

 
## Examples
 
 
