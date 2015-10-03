# ACS-84-0305
Collection of Xors
==============

## Overview:
A collection of Xors that can be used to modulate signals.
You can send in up to 8 digital signals. There are 3 outputs.
The first is just A and B Xor'd.
The second is A, B and C.
The last is ALL the pins.

Prob not an exact match if your looking for super high frequencies, but a decent way to explore the sounds of xor modulation if you can not get a hold of one. 

## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-84-0305/images/acs-84-0305.png)] (https://github.com/robstave/ArduinoComponentSketches)

## Equivalent:
The equivalent circuit would look something like this
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-84-0305/images/ACS-84-0305-xor.png)] (https://github.com/robstave/ArduinoComponentSketches)

PB0 is only going to xor the first 2 bits and PB1 adds the next one.
PB2 is the xor of all the inputs.  For this, you can pretty much hook into any input pin.

## Strategy:
In this case, all the logic is in the loop. We are just reading the values and outputting them.  No interrupts or anything like that.
So this should be used as a non-time critical modulation.  There is no guarantee that signals will flip within a certain time limit.

## Improvements:
The highest frequencies that you can handle are going to depend on how fast the loop executes.  
 - Wrap the loop code into a while loop.   That would shave a few cycles off as loop would only be called once.  
 - Accumulate the bits and just mask off the last bit rather than all this ^ business.
 - Maybe save the value of port a and break if nothing changed?

