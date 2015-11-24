# ACS-85-0006
Square wave VCO with randomness added to period
==============

## Overview:
Basically, makes a fuzzy sounding square wave.


#PB0
The first pin, PB0 is the pure squarewave. 50% duty cycle.
This is calculated with counters that flip the bit every x interrupts.

#PB1
PB1 is calculated the same way, except that in addition to the usual next count, a random number is 
added making the square a little longer or shorter.  The number is found using the LFSR.
so you are getting a fixed period +/- a random number.

On average, the length is what the first pin is set for, so they sound similar.

#PB2
PB2 is a bit more deterministic in adding the length.  It sounds a bit more like a fast ramp on the frequency.
Each cycle is the previous count + 1 up to about 30 I think.

What you get is a square wave with some power in near frequencies.  This would be different sounding than if we had just
adjusted the duty cycle.  In that case, the frequency would sound much clearer.  If you have built the atari punk console, you will hear
in one case where you turn the knob and the frequency sounds the same, but its 'phasey'.  In this case, it sounds more fuzzy


[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0006/images/ACS-85-0006-period.png)]
 

## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0006/images/ACS-85-0006.png)] (https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0006/images)

## Equivalent:

## Examples:

 https://soundcloud.com/user-692410397/85-0006a

 ## Improvements:
 Originally, I had envisioned that rather than adding a random number with an equal distribution, that the distribution be binomial.
 you can do this by adding 4 random numbers and then dividing by 4  (or shift right 2).  This would make the fuzz a little more subtle.
 
 
 Consider using this concept as the basic for atari punk console.  The squarewave is fuzzy and even the one shot calculated has some randomness to it.
 