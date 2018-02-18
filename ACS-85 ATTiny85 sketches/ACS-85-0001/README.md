# ACS-85-0001
Simple Squarewave Oscillator
==============

## Overview:
This is a down and dirty oscillator that you can use to test other circuits. 

No configuration pots or pins needed.

**The tuning fork of sketches.**

The frequencies are fixed, so if you want some control over the sound, this is not what you are looking for.  However, it is a good indicator if your
cpu fuses are where they are supposed to be.

- Two High Frequency square waves
- Two LFO and a ramping frequency

Note that the frequency here is not going to be exact. We are using the internal 8mhz clock..so you will see as much as +/- 15HZ on what the sketch says.  Its not too important here.  Again, we are just looking for a squarewave version of _Hello World_ or blinky light.  If you need more
accuracy, you will need to tie up some pins to a crystal.

## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0001/images/acs-85-0001.png)

## Equivalent:

Mostly used to replace a 40106 oscillator for a quick audio check.

![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0001/images/ACS-85-0001-overview.png) 

## Examples:
[Sample](https://soundcloud.com/user-692410397/85-0001a) Not really stellar.  Its just a square wave.  But at the end is the ramp.