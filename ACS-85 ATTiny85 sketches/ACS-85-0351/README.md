# ACS-85-0351

Detune

## Overview

Takes a medium to high audio range clock signal on pin 7 and outputs a detuned frequnecy above or below the note on two pins.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0351/images/ACS-85-0351.png)

## Examples

 <https://soundcloud.com/user-692410397/85-0351-1a>

## Strategy

The basic strategy for 0350-0353 is roughly the same.

We have a timer going to output notes like most of these sketches.  Flipping a pin when a counter value is reached gives us a squarewave.
Within that timer interrupt, we also just peg a counter.  I suppose you could use the counter value directly as well if you wished.

The clock signal is sent to pin 7 where it has a pin change interrupt.  
When the pin changes, we read the counter and take a difference from  the previous value.  
We are in effect, sampling the output counter at the input clock speed.  That will give us
the timer count that we need to output a signal at the same frequency as the input.

Once we know that, we can tweek that to make any relative frequency.

You get more consistency (and less of a quantization sound) if you do a rolling average, but its not needed.

That value is then used in the loop to figure out the next output frequency.  We read the pots to get a difference and just apply it to the counter
