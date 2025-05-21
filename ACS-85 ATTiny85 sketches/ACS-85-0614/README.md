# ACS-85-0614

Multiple Coin Flip Clocked - Output Matches Clock

## Overview

This sketch is inspired by Mutable Instruments' Branches. It triggers on the rising edge of the clock and outputs on either the A or B pin based on the probability of two successive flips. Here's how it works:

- With a 50% probability:
  - A = 1, B = 0 occurs 25% of the time.
  - A = 0, B = 1 occurs 25% of the time.
  - A = 0, B = 0 occurs 50% of the time.

The output gate matches the input clock. It is neither a trigger nor a latch.

If the A pin is set, the sketch uses 2 or 3 flips. With 3 flips, the probabilities are reduced further, creating more "dead space." This can be great for ambient effects.

This sketch is similar to [ACS-85-0612](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0612) and [ACS-85-0613](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0613).

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0614/images/ACS-85-0614.png)
