# ACS-85-0521
16 Bit LFSR
==============

## Overview:
Linear Feedback Shift Register.

See ACS-85-0520


16 bit shift register that is externally clocked.  The values at certain taps are xor'ed together and feed back into the shift register.
The number of unique states should be 2^n -1.  In this case 2^16 -1 = 65535.

Can be used to produce pseudo random patterns. In this case, the last 4 taps are output to the pins to produce random 4 bit patterns.
To make this happen, we clock the LFSR 4x for every input clock so that all the bits in the nibble are pseudo randomized.

Similar to ACS-85-0520 except that the clock is internal.  The voltage on A1 determines the frequency of the LFSR.
The result is four pins that all sound similar as noise, but are different.  They are just different taps in the LFSR.
 

This is the same LFSR as is described in the wikipedia. https://en.wikipedia.org/wiki/Linear_feedback_shift_register



## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0521/images/ACS-85-0521.png)] (https://github.com/robstave/ArduinoComponentSketches)

## Equivalent:

[![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0521/images/ACS-85-0521_ex.png)] 


## Similar Circuits
85-0520 is external clocked lfsr
85-0521 is internal clocked (variable controlled) VCO - audio frequency
85-0521 is internal clocked (variable controlled) LFO - low Frequency

## Examples:
 




