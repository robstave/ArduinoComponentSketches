# ACS-85-0010

Simple LFO

## Overview

PWM LFO outputs.

- triangle
- ramp up
- ramp down
- sine.

The 4 modes are selected with an analog pot.

### Selection pin

A linear pot is best, but if you have a log pot you can change the code to handle that range.

Just set this value to 0 for non-linear log case:

```
    #define MAP_VALUES_AS_LINEAR 1
```

A smoothing capacitor is needed on the output as it is PWM.

There is an extra pin that I used for debugging...its not particularly useful.  Delete that part of the code if you want.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0010/images/acs-85-0010.png)

 
## Examples

Here is the LFO driving a VCO.

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0010/images/ACS-85-circuit-0010-0002.png)

 
