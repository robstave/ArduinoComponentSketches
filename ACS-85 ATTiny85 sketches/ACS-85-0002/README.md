# ACS-85-0002 - Two Frequency VCO utility

==============

## Overview

This is meant to be something that you drop into a breadboard to test something else like a Low Pass filter or have it drive another arduino circuit.  There are plenty of times I just need an audio squarewave as the source of something else I would like to test.

- Two controllable frequencies and a pin that is a two tone combination.
- Values from 0 to 5 volts on the input pins produces audio squarewaves on the output pins.
- The final pin alternates between the two frequencies at a fixed interval.
- Loop averages over 4 samples so there actually is some delay in twisting the knob and the final frequecy. 

  Its resolution is limited and should not be considered as a core for a circuit as the range is small and the resolution kinda sucks at high frequencies.  Some might find that charming.

Check a deep dive on how this sketch works and the limitations of a strictly counter based algorthim ![here](https://github.com/robstave/ArduinoComponentSketches/wiki/Interrupt-Overflow).

ACS-85-0003 allows you to play with the alternating frequencies more.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0002/images/ACS-85-0002.png)

## Equivalent

![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0002/images/ACS-85-0002-overview.png)

Two 40106 oscillators and a pin that is a mix.

## Notes

The numbers 5 and 108 are kinda arbitrary.  The smaller the number, the smaller the counter and thus higher the frequency.  The counter is a byte to make the interrupt fast, so dont go above 127.

## Examples

[[Sample 1](https://soundcloud.com/user-692410397/85-0002-q1)] Changing knob value for one of the VCOs

[[Sample 2](https://soundcloud.com/user-692410397/85-0002-q3)] Two Tone pinout.  Switches between the sound of the first two pins.

 Here is the sketch driving the clock to a [[LFSR](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520)]
[[Sample](https://soundcloud.com/user-692410397/85-0001-0520mix)]
[![Mix](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520/images/ACS-85-circuit-0002-0520.png)]

Example of the VCO being driven by the LFO (ACS-85-0011).

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0002/images/ACS-85-circuit-0011-0002-1.png)

Example of the VCO being driven by the LFO (ACS-85-0011). In this case we are
also inverting the the PWM to give an alternate 
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0002/images/ACS-85-circuit-0011-0002-2.png)