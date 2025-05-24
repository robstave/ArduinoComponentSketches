# ACS-85-0307

AND / OR

## Overview

This sketch implements an AND gate and an OR gate.

It can be used to explore how these gates modulate or filter squarewave audio. While the fundamental frequencies won’t change, the width of some waves will.

Combining squarewaves with logic gates is a common concept in Lunettas. Many chips come with four gates, but if you want a full complement of options, you often end up wasting gates. This sketch could help reduce your circuit footprint.

To experiment with a single gate:
- Tie one of the inputs low or high.  
  For example, to test the OR gate, set C to high. Output 1 will then be the OR of A and B.
- To test the AND gate, set either A or B to low. The other input will pass through to the AND gate.

A  |  B  |  C  |  Out1  |   Out2
--- | --- | --- | --- | ---
0 | 0 | 0 | 0 | 0
0 | 1 | 0 | 1 | 0
1 | 0 | 0 | 1 | 0
1 | 1 | 0 | 1 | 0
0 | 0 | 1 | 0 | 0  
0 | 1 | 1 | 1 | 1
1 | 0 | 1 | 1 | 1
1 | 1 | 1 | 1 | 1

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0307/images/acs-85-0307.png)

## Equivalent

The equivalent circuit would look something like this:

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0307/images/ACS-85-0307-andor.png)

## Strategy

All the logic is handled in the loop. The sketch reads values and outputs them without using interrupts. This means it’s not suitable for time-critical modulation, as there’s no guarantee signals will flip within a specific time frame.

## Improvements

The maximum frequency you can handle depends on how fast the loop executes.

- Wrapping the loop code into a `while` loop could save a few cycles since `loop` would only be called once.
- Accumulating the bits and masking off the last bit could simplify the logic.
- Saving the value of port A and breaking if nothing changes might improve efficiency.
