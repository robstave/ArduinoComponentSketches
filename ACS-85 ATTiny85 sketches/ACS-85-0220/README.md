# ACS-85-0220

Debounced Trigger to Gate

## Overview

This sketch converts a trigger into a gate, with some added logic to handle debouncing.

You can control the length of the gate and subdivide it into smaller intervals. For example, if the gate is 500ms and the ticks are set to 1 (just ground the pin), it behaves like a normal trigger-to-gate.

If the ticks are set to 5, the 500ms gate is divided into 100ms intervals, alternating between on and off states. This allows you to turn a half-note gate into 1/16 notes on the fly. The total length remains the same, but the last interval could be silence.

This sketch doesn’t produce audio, just gates.

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0220/images/drawing.png)

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0220/images/ACS-85-0220.png)

## Examples
