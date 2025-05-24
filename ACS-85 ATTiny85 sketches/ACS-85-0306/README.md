# ACS-85-0306

Collection of NANDs

## Overview

This sketch implements two NAND gates with the following outputs:

- `!(A & B)`  
- `!(!(A & B) & C)`

A  |  B   |  C   | Out1  |   Out2
--- | --- | --- | --- | ---
0  | 0  | 0  | 1 |   1
0  | 1  | 0  | 1 |   1
1  | 0  | 0  | 1 |   1
1  | 1  | 0  | 0 |   1
0  | 0  | 1  | 1 |   0  
0  | 1  | 1  | 1 |   0
1  | 0  | 1  | 1 |   0
1  | 1  | 1  | 0 |   1

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0306/images/acs-85-0306.png)

## Equivalent

The equivalent circuit would look something like this:

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0306/images/ACS-85-0306-nands.png)

## Examples

Here’s an example of the ACS-85-0004 driving the NAND gates:

<https://soundcloud.com/user-692410397/85-0306nand-0004>

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0306/images/ACS-85-0306-0004.png)

## Strategy

All the logic is handled in the loop. The sketch reads values and outputs them without using interrupts. This means it’s not suitable for time-critical modulation, as there’s no guarantee signals will flip within a specific time frame.

## Improvements

The maximum frequency you can handle depends on how fast the loop executes.

- Wrapping the loop code into a `while` loop could save a few cycles since `loop` would only be called once.
- Accumulating the bits and masking off the last bit could simplify the logic.
- Saving the value of port A and breaking if nothing changes might improve efficiency.
