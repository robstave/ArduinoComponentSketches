# ACS-85-0307
or/and
==============

## Overview:
An OR and an AND
Can be used to hear how they modulate/filter squarewave audio.  In general, your fundemental frequencies are not going to change.
Using and/or for audio will not really change that, but it will change the width of some of the waves.

To play with a single wave, tie one of the gates low or high.
For example, to experiment with just the or gate, set C to high, and output 1 is just an or of A and B.
To play with the And, set either A or B to  Low.  The value of the other bit will just pass on through  to the AND gate.


A  |  B  |  C  |  Out1  |   Out 2
--- | --- | --- | --- | ---
0 | 0 | 0 | 0 | 0
0 | 1 | 0 | 1 | 0
1 | 0 | 0 | 1 | 0
1 | 1 | 0 | 1 | 0
0 | 0 | 1 | 0 | 0  
0 | 1 | 1 | 1 | 1
1 | 0 | 1 | 1 | 1
1 | 1 | 1 | 1 | 1 
 
## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0307images/acs-85-0307.png)] (https://github.com/robstave/ArduinoComponentSketches)


## Equivalent:
The equivalent circuit would look something like this
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0307/images/ACS-85-0307-andor.png)] (https://github.com/robstave/ArduinoComponentSketches)
 

## Examples:
 

## Strategy:
In this case, all the logic is in the loop. We are just reading the values and outputting them.  No interrupts or anything like that.
So this should be used as a non-time critical modulation.  There is no guarantee that signals will flip within a certain time limit.

## Improvements:
The highest frequencies that you can handle are going to depend on how fast the loop executes.  
 - Wrap the loop code into a while loop.   That would shave a few cycles off as loop would only be called once.  
 - Accumulate the bits and just mask off the last bit rather than all this ^ business.
 - Maybe save the value of port a and break if nothing changed?

