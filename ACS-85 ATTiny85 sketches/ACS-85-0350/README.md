# ACS-85-0350

Harmonizer

## Overview

Takes a medium to High frequency on pin 7 and outputs harmonic notes such as maj and min 3rd, 5th and 4th.

These are producible with just cmos counters, but you end up with intervals that are above notes that are 2 octaves below the carrier.

By dividing a note by two, you get an octave below.  Dividing by 3 ends up with a perfect fifth above two octaves below the carrier.

This works, but puts everything an octave below to start with. There are ways around this such as using a Phase Locked Loop perhaps to boost the carrier up in frequency before you divide it.

In this case, we sample the frequency and start off with that. Simply using math on the interrupt counts gives better resolution and higher available frequencies.  Even ones higher than the clock.

![Harmonizer](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0350/images/acs-85-0350_notes.png)

### Minor 3rd

A major third in just intonation corresponds to a pitch ratio of 6:5.  

https://en.wikipedia.org/wiki/Minor_third
 
### Major 3rd

A major third corresponds to a pitch ratio of 5:4 

https://en.wikipedia.org/wiki/Major_third


### Perfect fifth

a perfect fifth is the musical interval corresponding to a pair of pitches with a frequency ratio of 3:2.
This one is easier to do with cmos counters.  By dividing by 3, you end up with a fifth above two octaves below.

*Note in this case, the sketch is providing a fifth interval BELOW the carrier, so you just flip the ratio.*

### Perfect fourth

A perfect fourth in just intonation corresponds to a pitch ratio of 4:3

*Note in this case, the sketch is taking the note from the perfect fifth below and bumping it up an octave to be a fourth from the carrier.*

 
## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0350/images/ACS-85-0350.png) 


## Examples

Sound cloud example
https://soundcloud.com/user-692410397/85-0350-1a


## Strategy

The basic strategy for 0350-0353 is roughly the same.

We have a timer going to output notes like most of these sketches.  Flipping a pin when a counter value is reached gives us a squarewave.

Within that timer interrupt, we also just peg a counter.  I suppose you could use the counter value directly as well if you wished.

So that part is like 0001...just an oscillator and a completely arbirary number gives a 
frequency.  Just a number.

The clock signal is sent to pin 7 where it has a pin change interrupt as well.

When the clock pin changes, we read the output counter and take a difference from the previous value.  
That way we can track the counter and find a base number that makes the output
the same as the input. (Or a ratio in our case)  

If you had it set up, you could clock out at the same speed
as the input because you are in effect not really sampling the input frequency but
really sampling the output loop at the speed of the input clock.  Ya..its abstract.  

Its compensating too..if you added more clock cycles to the output loop, it would
be compensated because we are just setting our frequencies as ratios of the actual
output interrupt.

You get more consistency (and less of a quantization sound) if you do a rolling average, but its not needed.
 
For the harmonizer, the ratios are set.  We adjust the timer counters (frequecy)
in the loop which is interruptable and can take its sweet time to the do the math. 

Its probibly not the best way to do it..and not super fast. But again, we are going for 
simple fun experimental sounds and not precision.

