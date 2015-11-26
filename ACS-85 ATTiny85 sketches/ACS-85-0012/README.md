# ACS-85-0012
Random LFO PWM out w Glide
==============

## Overview:
PWM lfo Output  With glide

Similar output to Psycho LFO

The Glide is achieved by averaging previous values 
Output is PWM so a Low pass filter is needed.

The random value is determined from a LFSR
https://github.com/robstave/ArduinoComponentSketches/wiki/LFSR



## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0012/images/acs-85-0012.png)] 

## Equivalent:

Its a basic equivalent to the ken stone Psycho lfo
Kinda sorta
http://www.cgs.synth.net/modules/psycho_lfo.html

## Strategy:
Use the LFSR to get a random number.
Use timer 0 to do fast PWM 
Use timer 1 to set the next value

## Examples:

 
