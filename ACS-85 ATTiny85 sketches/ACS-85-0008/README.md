# ACS-85-0008

ATTiny85 Squarewave with Vibrato

This sketch is similar to 0007, but instead of a ramp-down, it modulates the frequency up and down using a sine or triangle wave for a better vibrato effect.

## Overview

### Inputs:

- Frequency
- Vibrato width

### Outputs:

- **PB0**: Frequency ramps down
- **PB1**: Frequency ramps up
- **PB2**: Frequency oscillates up and down as a triangle wave

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0008/images/ACS-85-0008.png)

## Equivalent

This is essentially an LFO driving a VCA.

## Examples

https://soundcloud.com/user-692410397/85-0008a

## Improvements

The output is quite bold and could use some refinement. Sketch 85-0009 is reserved for a sine wave version when I get around to fine-tuning this.  