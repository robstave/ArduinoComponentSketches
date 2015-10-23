# ACS-85-0530
Drunk Walk
==============

## Overview:
A simulation of the drunk walk as an audio output.
Start at a set frequency

Uses the LFSR to determine a random number that we compare against. If we are above or below that number, increment or decrement the frequency respectively.
You do get a very wondering effect. Although the responsiveness of the parameters leave a little to be desired.  You can tell that you have the skew set or 
stagger on...but its not too much different.  Its not an active knob turner..but certainly ambient changes would work well here.

## Configuration:

The highest and lowest frequency are set with these parameters.
Each time the timer interrupts, we increment a counter. When the counter reaches its number,
the pin is flipped. So LOW kinda really means the highest frequency as we are not counting as much.

The start frequency is the frequency we start at when the walk it reset.

    //High low and middle freq
    #define FREQ_LOW 7
    #define FREQ_HIGH 155
    #define FREQ_START 88

These set the step speed. This is how often the note changes

    //Step speed
    #define SPEED_LOW 700
    #define SPEED_HIGH 4000

The Stagger can be changed as well.  Currently, the code does steps as [1,2,3,4,5,1,2,3,4,5..]
Making this number larger will make it more exaggerated.

    //step size
    #define MAX_STAGGER 5




## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0530/images/acs-85-0530.png)] (https://github.com/robstave/ArduinoComponentSketches)

## Equivalent:
A kinda equivalent circuit would be something like this.  The buttons represent the random up and down triggers...where every they may come from.
This counts the circuit up or down until it reaches the top or bottom (carry or borow).  That forces the reload (aka set back to the middle).  The state is 
translated to a frequency via the r2r->vco part.

[![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0530/images/ACS-85-0530_overview.png)]  

## Examples: