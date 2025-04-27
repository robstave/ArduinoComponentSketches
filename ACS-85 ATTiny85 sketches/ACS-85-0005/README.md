# ACS-85-0005 - Three VCOs Detuned

## Overview

This sketch creates three square wave oscillators with a single frequency control and a detune control. To hear the drones, you can sum their outputs using a simple resistor summing network.

- **PB0**: Base frequency
- **PB1**: Frequency - detuned
- **PB2**: Frequency - 2x detuned

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0005/images/ACS-85-0005.png)

## Equivalent

![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0005/images/ACS-85-0005eq.png)

Imagine 3 40106 squarewave generators.  All 3 driven by a 3 gang pot.
Then, the second and third have another 3 gang pot with two of the deals in the third oscillator so that you have detune * 2.

The outputs for each pin is just a square wave. You need to sum with resistors to hear the drones. Or perhaps you could push out the squarewaves to other things first like counter or gates or whatever.

## Examples

Here’s an example of the XOR being driven by some drones (ACS-85-0305):

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0005/images/ACS-85-circuit-0005-0305.png)
