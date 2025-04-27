# ACS-85-0015 - Two Square Wave LFOs

## Overview

This sketch features two simple square wave LFOs (Low-Frequency Oscillators) with different frequency ranges. Each channel operates independently, making it a no-frills, straightforward digital LFO setup.

These LFOs are purely digital, so no smoothing is required. However, if you’re driving an analog circuit, you can add a capacitor to create a glide effect.

| Range | Low Frequency | High Frequency |
|-------|---------------|----------------|
| LF1   | 1 Hz          | 16 Hz          |
| LF2   | 0.2 Hz        | 4 Hz           |

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0015/images/ACS-85-0015.png)

## Equivalent Circuit

This setup is similar to the [ACS-85-0004](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004) LFOs.

## Improvements

There’s a free pin available—perhaps it could be used for a sync feature?