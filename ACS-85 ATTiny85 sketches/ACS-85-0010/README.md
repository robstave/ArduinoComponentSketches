# ACS-85-0010

Simple LFO

## Overview

This sketch provides PWM LFO outputs in four modes:

- Triangle
- Ramp up
- Ramp down
- Sine

The mode is selected using an analog potentiometer.

### Selection Pin

A linear potentiometer works best, but if you have a logarithmic pot, you can adjust the code to handle the range. Just set this value to `0` for a non-linear (logarithmic) case:

```
#define MAP_VALUES_AS_LINEAR 1
```

A smoothing capacitor is required on the output since it’s PWM.

There’s also an extra pin used for debugging. It’s not particularly useful, so feel free to remove that part of the code.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0010/images/acs-85-0010.png)

## Examples

Here’s the LFO driving a VCO:

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0010/images/ACS-85-circuit-0010-0002.png)


