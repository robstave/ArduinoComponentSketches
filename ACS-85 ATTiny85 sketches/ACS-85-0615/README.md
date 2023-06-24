# ACS-85-0615

Random Gates

## Overview

The knob is just a value that is compared to the LFSR if its low then the check is the bottom 8 bits vs the value.

So a zero is pretty much always lower that the LFSR and is not a drop.

There is no debouncing, consider a lpf in from of the clock.  it does not take much

The CV adds to the value and  it rotates at 255 so 50% + 20% = 70%...but it will wrap.  This means that if your only interested in using the cv...turn the knob to zero.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0615/images/ACS-85-0615.png)
