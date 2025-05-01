# ACS-85-0060

Square Wave VCO with Second Frequency Modulated by LFO

## Overview

This sketch implements a Voltage-Controlled Oscillator (VCO) using a 16-bit counter in a DDS-like fashion. For more details, check out [this article](http://www.technoblogy.com/show?QVN).

The key difference here is that instead of using the second counter for PWM, it’s used as an LFO. The upper 8 bits would typically point to a 256-size wavetable, but since we’re working with square waves, we can simply use the uppermost bit.

There are two accumulators:
- One for the main frequency.
- One for LFO-controlled modulation of the main frequency.

To mix the outputs, you can sum them across the pins using resistors or a proper op-amp summer.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0060/images/ACS-85-0060.png)

## Related

Check out [ACS-85-0061](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0061) for a variation with pulse-width modulation.
