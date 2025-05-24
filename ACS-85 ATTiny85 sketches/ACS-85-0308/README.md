# ACS-85-0308

Pick-a-Gate

## Overview

This sketch allows you to select between AND, OR, NAND, and NOR gates on an ATTINY85 chip.

It can help reduce your circuit footprint or let you experiment with different sounds on your breadboard.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0308/images/acs-85-0308.png)

## Equivalent

The equivalent circuit would look something like this:

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0308/images/ACS-85-0308-pick.png)

## Strategy

All the logic is handled in the loop. The sketch reads values and outputs them without using interrupts. This means it’s not suitable for time-critical modulation, as there’s no guarantee signals will flip within a specific time frame.

## Improvements

The maximum frequency you can handle depends on how fast the loop executes.

- Wrapping the loop code into a `while` loop could save a few cycles since `loop` would only be called once.
- Accumulating the bits and masking off the last bit could simplify the logic.
- Saving the value of port A and breaking if nothing changes might improve efficiency.
