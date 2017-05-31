# ACS-85-0010
Simple LFO
==============

## Overview:
PWM lfo outputs.
Has a triangle, ramp up, ramp down and sine.

The mode is selected with an analog pot.  So there are 4 modes. 
A linear pot is best, but if you have a log pot you can change the code to handle that range.

Set this value to 0 for non-linear log case:

```
    #define MAP_VALUES_AS_LINEAR 1
```

A smoothing capacitor is still needed on the output.  But you can always use
somethign like a LED and LDR too to make an variable resistor.

## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0010/images/acs-85-0010.png)

## Equivalent:

 Basically an LFO, but with a pot to select the waveform.
 
## Examples:

Here is the LFO driving a VCO.

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0010/images/ACS-85-circuit-0010-0002.png)

 
