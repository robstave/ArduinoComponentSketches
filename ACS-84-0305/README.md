# ACS-84-0305
Collection of Xors
==============

## Overview:
A collection of Xors that can be used to modulate signals.
You can send in up to 8 digital signals. There are 3 outputs.
The first is just A and B Xor'd.
The second is A, B and C.
The last is ALL the pins.

## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-84-0305/images/acs-84-0305.png)] (https://github.com/robstave/ArduinoComponentSketches)

## Equivalent:
The equivalent circuit would look something like this

## Strategy:
In this case, all the logic is in the loop. We are just reading the values and outputting them.  No interrupts or anything like that.
So this should be used as a non-time critical modulation.  There is no guarantee that signals will flip within a certain time limit.
 
