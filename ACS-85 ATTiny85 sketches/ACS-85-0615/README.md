# ACS-85-0615

Random Gates

## Overview

This sketch generates random gates based on a knob and an LFSR (Linear Feedback Shift Register). Here's how it works:

- The knob sets a value compared to the LFSR. If the knob value is low, the check compares the bottom 8 bits of the LFSR to the knob value.
- A zero knob value almost always results in no drops.
- The CV input adds to the knob value. Note: the value wraps at 255, so 50% + 20% = 70%, but it will wrap around. If you only want to use the CV, set the knob to zero.

There is no debouncing, so consider adding a low-pass filter in front of the clock.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0615/images/ACS-85-0615.png)
