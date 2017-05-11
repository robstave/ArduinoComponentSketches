# ACS-85-0002
Two Frequency VCO
==============

## Overview:
Two controllable frequencies and a pin that is a two tone combination. Values from 0 to 5 volts on the input pins produces audio squarewaves on the output pins.
The final pin alternates between the two frequencies at a fixed interval. 

ACS-85-0003 allows you to play with the alternating frequencies more. 


## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0002/images/ACS-85-0002.png)

## Equivalent:

![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0002/images/ACS-85-0002-overview.png)

Two 40106 oscillators and a pin that is a mix. 

## Examples:

[[Sample 1](https://soundcloud.com/user-692410397/85-0002-q1)] Changing knob value for one of the VCOs

[[Sample 2](https://soundcloud.com/user-692410397/85-0002-q3)] Two Tone pinout.  Switches between the sound of the first two pins. 
 
 Here is the sketch driving the clock to a [[LFSR](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520)]
[[Sample](https://soundcloud.com/user-692410397/85-0001-0520mix)]
[![Mix](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520/images/ACS-85-circuit-0002-0520.png)]
