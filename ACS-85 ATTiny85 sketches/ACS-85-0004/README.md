# ACS-85-0004
Two Square wave VCOs with Selectable range (LF to HF)
==============

## Overview:
Perhaps a bit more utilitarian than others.  
Two vcos with an analog pin to select the range from one of 6 ranges.  High Freq 1, High Freq 2, Medium Freq 1, Medium Freq 2 and Low Freq 1 and 2.


Range    | Low   | High 
--- | --- | ---
HF1  |  250 Hz  |  4Khz
HF2  |  166 Hz  |  666 Hz
MF1  |  62.5 Hz  |  222 Hz
MF2  |  38.5 Hz  |  133 Hz
LF1  |  1 Hz  | 16 Hz
LF2  |  .2 Hz  | 4 Hz

You can change the values, but keep in mind the HF and MF counter is an Integer.
For the low frequencies, the code was placed in another loop.  Changing the int to a long did not work well and I could not get above 1.5Khz.  I think this is because the work in the interrupt handler for longs takes way more clock cycles.
I also ended up copying the counter code for the LFO branch rather than add complexity (and clock cycles) making a handler that looks tight.


## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004/images/ACS-85-0004.png)] (https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004/images)

## Equivalent:
![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004/images/ACS-85-0004-overview.png)

Two 40106 oscillators with selectable RC constants


## Examples:

 
