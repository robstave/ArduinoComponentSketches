# ACS-85-0040

Square Wave VCO with PWM and Chiptune Sound

## Overview

This sketch implements a Voltage-Controlled Oscillator (VCO) using a 16-bit counter in a DDS-like fashion. For more details, check out [this article](http://www.technoblogy.com/show?QVN).

The VCO has two outputs:
- A main square wave.
- A secondary output with the same frequency but adjustable pulse width.

There’s also a chiptune mode! Flip pin 5, and the DDS gets scrambled, creating an arpeggiated sound (arp up). It’s chaotic but fun.

This sketch is a bit of a hot mess, but I included it in my hybrid-lunetta project anyway, and it has become a favorite.

For more refined versions, check out:
- [ACS-85-0041](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0041)
- [ACS-85-0042](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0042)

These sketches split the PWM and chiptune sounds into their respective implementations.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0040/images/ACS-85-0040.png)