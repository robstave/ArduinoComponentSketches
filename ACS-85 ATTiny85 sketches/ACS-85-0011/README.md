# ACS-85-0011
Random LFO PWM out  (Psycho LFO)
==============

## Overview:
PWM LFO Output.

Similar output to Psycho LFO.

Output is PWM so a Low pass filter is needed.

The random value is determined from a LFSR (Linear Feedback Shift Register)
https://github.com/robstave/ArduinoComponentSketches/wiki/LFSR


## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0011/images/acs-85-0011.png)

## Equivalent:

Its a basic equivalent to the [Ken stone Psycho lfo](http://www.cgs.synth.net/modules/psycho_lfo.html).

Kinda sorta.


## Strategy:

- Use the LFSR to get a random number.
- Use timer 0 to do fast PWM 
- Use timer 1 to set the next value

## Examples:

 
