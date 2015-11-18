# ACS-85-0352
Sample and Vibrato (ramps)
==============

## Overview:
Takes a medium to High frequnecy on pin 7 and outputs a squarewave with variable frequency and width.

Vibrato is when you modulate the frequency of the signal (as opposed to tremolo where you modulate the amplitude)

This is not as spectacular as I had hoped, but has promise


 
 
## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0352/images/acs-85-0352.png)] (https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0352/images)


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
This could use some help
Its just ramps
A triangle perhaps.  


