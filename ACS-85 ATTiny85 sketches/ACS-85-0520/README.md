# ACS-85-0520
16 Bit LFSR
==============

## Overview:
Linear Feedback Shift Register.

16 bit shift register that is externally clocked.  The values at certain taps are xor'ed together and feed back into the shift register.
The number of unique states should be 2^n -1.  In this case 2^16 -1 = 65535.

Can be used to produce pseudo random patterns. In this case, the last 4 taps are output to the pins to produce random 4 bit patterns.
To make this happen, we clock the LFSR 4x for every input clock so that all the bits in the nibble are pseudo randomized.

Interesting to note that clocking it four times will still give you the full state machine.
(I have tested this with serial output on an uno...Ill post that code sometime).

Clocking by 3 will give  you a reduced state machine as 655535 is divisible by 3.
 
The sound you get from this sketch does have a frequency for the most part.  Since it is clocked, the width of each cycle is based on that width, so this is by no means a white noise.
Rather, the power would be distributed along the same fundamentals as a squarewave or..well..maybe like a counter that divs by 2, 3, 4, 5, 6, 7 and so on.
Its been decades since I have done the math, but you can get most of what Im talking about by sampling the LFSR at a frequency and looking at its spectrum.
It will look similar to the squarewave with peaks at 3f, 5f and so on.  White noise is a flat plot.
 
This does lead to an interesting effect though because you can tune the lfsr by changing the clock speed.  Below is an example where a frequency ramp is put into the clock.
You still hear the ramp...its just fuzzy.  Very 8bit sounding.
 

This is the same LFSR as is described in the wikipedia. https://en.wikipedia.org/wiki/Linear_feedback_shift_register

See also https://github.com/robstave/ArduinoComponentSketches/wiki/LFSR



## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520/images/ACS-85-0520.png)

## Equivalent:

![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520/images/ACS-85-0520_ex.png)


## Examples:
Here is an example circuit that demonstrates how it sounds when we drive the LFSR with a signal in the audio range.
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520/images/ACS-85-circuit-0002-0520.png)

An audio example is here https://soundcloud.com/user-692410397/85-0001-0520mix

A second audio example is the 85-0003 driving the LFSR in the same manner

https://soundcloud.com/user-692410397/85-0003-0520mix

## Similar Circuits

* [ACS-85-0520](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520) is external clocked lfsr
* [ACS-85-0521](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0521) is internal clocked (variable controlled) VCO - audio frequency
* [ACS-85-0522](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0522) is internal clocked (variable controlled) LFO - low Frequency



