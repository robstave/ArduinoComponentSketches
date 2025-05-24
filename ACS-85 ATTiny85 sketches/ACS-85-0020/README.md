# ACS-85-0020

ATTiny85 Squarewave LFO with Duty Cycle

## Overview

This sketch provides two inputs:

- Frequency
- Duty cycle

And three outputs:

- LFO as a squarewave
- !LFO as an inverted squarewave
- LFO at half the frequency as a squarewave

Additionally, there’s a pin outputting the LFO at half the frequency. It’s just a free pin, but it might come in handy.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0020/images/ACS-85-0020.png)

## Equivalent

This is somewhat equivalent to a 555 timer circuit. However, one major advantage here is that the duty cycle is independent of the frequency, which is a common limitation of 555 circuits.

 