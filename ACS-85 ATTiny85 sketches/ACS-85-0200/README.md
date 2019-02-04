# ACS-85-0200

 ATTiny85 Simple VCA

## Overview

Simple VCA that really only works on a squarewave.
Works in the same manner as the simple lunetta VCA.  Rather than adjusting the amplitude of an input single, we are "chopping" the value of the amplitude.

In this case, we are using fast PWM to output the value of the pot.  INT0 is used to read the value of the "clock" or the squarewave input.  When the state is high, the output is the value of the pot.

Note, this is not a good sketch for LOW frequency values.  The Interrupt is just listening for changes in the state, so it is not particularly interested in the actual high/low value.  If this is what you need, then you should read the value.

See a good example of this on [hackaday](https://hackaday.com/2015/04/10/logic-noise-more-cmos-cowbell/)

Two inputs:

- Square wave input
- Analog volume

One Output:

 - PWM output


## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0200/images/ACS-85-0200.png)

## Equivalent


![circuit](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0200/images/circuit.png)

