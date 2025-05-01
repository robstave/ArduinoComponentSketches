# ACS-85-0041

Square Wave VCO with PWM

## Overview

This sketch implements a Voltage-Controlled Oscillator (VCO) using a 16-bit counter in a DDS-like fashion. For more details, check out [this article](http://www.technoblogy.com/show?QVN).

The VCO has two outputs:
- A main square wave.
- A secondary output with the same frequency but adjustable pulse width.

Originally, ACS-85-0040 handled this functionality along with other features, but it became a bit too busy. This sketch focuses solely on the square wave VCO with PWM for simplicity.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0041/images/ACS-85-0041.png)