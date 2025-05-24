# ACS-85-0017

ATTiny85 Clock/Counter 4-bit Gate or Trigger (Like a 4017)

## Overview

This sketch is like a mini version of the CD4017. Think of it as a "baby 10," but with only 4 outputs!

### Inputs:

- Clock

### Outputs (similar to the CD4017):

- Counter output bit q0  
- Counter output bit q1
- Counter output bit q2
- Counter output bit q3

Additionally, there’s a software boolean that toggles between two modes:

- **Gate mode**: Regular operation.
- **Trigger mode**: Outputs a short pulse when the bit flips.

```c
0001
0010
0100
1000
```

### Software Parameters

- **Trigger mode**
- **Trigger length**: Not measured in milliseconds but in arbitrary clock counts. Keep in mind that the ATTiny’s internal oscillator can be a bit imprecise, so your results may vary.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0017/images/ACS-85-0017.png)

## Equivalent

This is equivalent to a CD4017, but with fewer outputs.

## Cat

![Pinout](https://lh3.googleusercontent.com/pw/AP1GczO08Bf2Slzg2iBTKbaGH_2BjXIy2HDOWUolclef5130hK0OrXo-b3mFdiUwAUemvSolPc7voonlrYOwCLp7HCt7M8Z8Vd2KZTkEdeK2RIq_7yXkpSY=w2400)
