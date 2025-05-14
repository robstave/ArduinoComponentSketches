# ACS-85-0305

Collection of XORs

## Overview

This sketch provides two XOR operations, simulating an XOR in your signal path. Instead of using a pile of chips, you can program the ones you need!

Outputs are either `A ^ B` or `A ^ B ^ C`.

It should work fine for audio frequencies, but don’t rely on it as a real CMOS logic chip.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0305/images/acs-85-0305.png)

## Equivalent

The equivalent circuit would look something like this:

![Equivalent Circuit](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0305/images/ACS-85-0305-xor.png)

**PB0** performs an XOR on the first two bits, while **PB1** adds the third bit to the operation.

## Examples

Here’s an example of the XOR being driven by two square waves (ACS-85-0004):

<https://soundcloud.com/user-692410397/85-0305xor-0004>

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0305/images/ACS-85-0305-0004.png)

Here’s another example, driven by some drones (ACS-85-0005):

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0305/images/ACS-85-circuit-0005-0305.png)

## Strategy

All the logic is in the loop. We’re just reading values and outputting them—no interrupts or anything fancy. This makes it suitable for non-time-critical modulation. There’s no guarantee that signals will flip within a specific time limit.

## Improvements

The highest frequencies you can handle depend on how fast the loop executes.

- Wrap the loop code into a `while` loop to shave off a few cycles (the loop would only be called once).
- Accumulate the bits and mask off the last bit instead of using multiple XOR operations.
- Save the value of Port A and break early if nothing changes.
