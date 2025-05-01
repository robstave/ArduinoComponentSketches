# ACS-85-0612

Clocked Coin Flip

## Overview

This sketch is similar to Mutable Instruments' Branches. It triggers on the rising edge of a clock signal and outputs on either the A or B pin based on a set probability.

If the probability is 50%, a trigger on PB02 will result in either PB0 or PB1 going high with equal likelihood. Adjusting the probability skews the results toward one pin or the other.

A toggle pin determines whether the output is a trigger or a latched signal:

- In **Trigger mode**, the length of the trigger is set in the code.
- In **Latched mode**, the pin remains high until the A/B value changes.

This sketch is similar to ![ACS-85-0613](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0613) and ![ACS-85-0614](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0614).

![Diagram](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0612/images/diagram.png)

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0612/images/ACS-85-0612.png)

