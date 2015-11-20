# ACS-85-0353
Sample and modultate with LFSR
==============

## Overview:
Takes a medium to High frequency on pin 7 and outputs a squarewave with variable frequency and width.
Kinda like the frequency was modulated by a square wave source.

The Width determines the size of the mask to add to the sampled frequency.
Speed is the duration of each step.

[![Notes](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0350/images/acs-85-0353_notes.png)]



The offset is determined by
 
    return lfsr & B00001111;
	
So we mask off in this case the lower 4 bits for a number from 1 to 15

then we add that to oscillator counter

     oscFreq1 = diff + getOffset();
     oscFreq2 = diff - getOffset();
	 
PB0 will have a counter value higher than the sampled counter from the carrier, resulting in a lower frequency.
PB0 will have a counter value lower than the sampled counter from the carrier, resulting in a higher frequency.

Mixing the two channels gives a fun, complementary random sound.  You might consider changing the value of the mask
so the offset is taken from other bits in the LFSR or just clocking the lfsr between lines.

     oscFreq1 = diff + getOffset();
	 clockLfsr();
     oscFreq2 = diff - getOffset();


	 


 
## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0353/images/acs-85-0353.png)] (https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0353/images)


## Equivalent:


## Examples:
 https://soundcloud.com/user-692410397/85-0352-1a

## Strategy:
 
The basic strategy for 0350-0353 is roughly the same.
We have a timer going to output notes like most of these sketches.  Flipping a pin when a counter value is reached gives us a squarewave.
Within that timer interrupt, we also just peg a counter.  I suppose you could use the counter value directly as well if you wished.


The base signal is sent to pin 7 where it has a pin change interrupt.  when the pin changes, we read the counter and take a difference from 
the previous value.  You get more consistency (and less of a quantization sound) if you do a rolling average, but its not needed.

That value is then used in the loop to figure out the next output frequency.

## Improvements:
   


