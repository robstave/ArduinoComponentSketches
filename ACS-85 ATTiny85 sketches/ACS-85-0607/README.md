# ACS-85-0607

4 Channel drum patterns with randomness

## Overview

Three drum triggers selected with tempo and a randomness for flipping bits.

Similar to ![ACS-85-0603](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0603), and ![ACS-85-0604](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0604). however, there are no patterns and the randomness is set in the code.
Just a tempo.  Honestly, this is my favorite

Thw four outputs are set as triggers, but the width of the trigger can be modified in the code.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0607/images/ACS-85-0607.png)

## Improvements

A Sync would be nice, but there are only so many pins to work with.  Consider driving this with a clk rather than an analog pot/tempo if thats what you need.
