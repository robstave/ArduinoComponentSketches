# ACS-85-0350
Harmonizer
==============

## Overview:
Takes a medium to High frequnecy on pin 7 and outputs harmonic notes such as maj and min 3rd, 5th and 4th.
These are producable with just cmos counters, but you end up with intervals that are above notes that are 2 octaves below the carrier.

By dividing a note by two, you get an octave below.  Dividing by 3 ends up with a perfect fifth above two octaves below the carrier.
Ways around this involve a Phase locked loop to boost the carrier up in frequency before you divide it.






[![Notes](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0350/images/acs-85-0350_notes.png)]


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






 
## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0350/images/acs-85-0350.png)] (https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0350/images)

## Equivalent:

## Examples:
Sound cloud example
https://soundcloud.com/user-692410397/85-0350-1a


## Strategy:
The basic strategy for 0350-0353 is roughly the same.
We have a timer going to output notes like most of these sketches.  Flipping a pin when a counter value is reached gives us a squarewave.
Within that timer interrupt, we also just peg a counter.  I suppose you could use the counter value directly as well if you wished.


The base signal is sent to pin 7 where it has a pin change interrupt.  when the pin changes, we read the counter and take a difference from 
the previous value.  You get more consistency (and less of a quantization sound) if you do a rolling average, but its not needed.

That value is then used in the loop to figure out the next output frequency.

For the harmonizer, the ratios are set
 

## Improvements:


