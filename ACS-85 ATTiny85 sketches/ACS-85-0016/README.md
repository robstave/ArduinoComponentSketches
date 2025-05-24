# ACS-85-0016

ATTiny85 Clock/Counter 4-bit Gate or Trigger

## Overview

This sketch provides:

### Input:

- Clock

### Outputs (like a standard 4-bit counter):

- Counter output bit q0  
- Counter output bit q1
- Counter output bit q2
- Counter output bit q3

It’s similar to a CD40163 counter. Just a simple counter.

However, there’s a twist! A software boolean toggles between two modes:

- **Gate mode**: Regular operation.
- **Trigger mode**: Outputs a short pulse when the bit flips. This is adjustable in software and works well for standalone drum triggers.

```
Gate     Trigger
0000     0000
0001     0001
0010     0010
0011     0001
0100     0100
0101     0001
0110     0010
0111     0001
1000     1000
1001     0001
1010     0010
1011     0001
1100     0100
1101     0001
1110     0010
1111     0001
```

### Software Parameters

- **Trigger mode**
- **Trigger length**: Not measured in milliseconds but in arbitrary clock counts. The ATTiny’s internal oscillator can be a bit imprecise, so results may vary.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0016/images/ACS-85-0016.png)

## Equivalent

This is equivalent to a CMOS counter.

## Examples

