# ACS-85-0353

Sample and modulate with LFSR

## Overview

Takes a medium to high audio clock signal on pin 7 and outputs a squarewave with variable frequency and width.

Kinda like the frequency was modulated by a square wave source.

The width determines the size of the mask to add to the sampled frequency.
Speed is the duration of each step.

The basic strategy is the same as [ACS-85-0350](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0350) 


## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0353/images/ACS-85-0353.png)

## Notes

![Notes](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0353/images/ACS-85-0353_notes.png)


The offset is determined by
 
```
    return lfsr & B00001111;
```

So we mask off in this case the lower 4 bits for a number from 1 to 15

then we add that to oscillator counter

```
     oscFreq1 = diff + getOffset();
     oscFreq2 = diff - getOffset();
```

**PB0** will have a counter value higher than the sampled counter from the carrier, resulting in a lower frequency.

**PB1* will have a counter value lower than the sampled counter from the carrier, resulting in a higher frequency.

Mixing the two channels gives a fun, complementary random sound.  You might consider changing the value of the mask
so the offset is taken from other bits in the LFSR or just clocking the lfsr between lines.

```
     oscFreq1 = diff + getOffset();
	 clockLfsr();
     oscFreq2 = diff - getOffset();
```

 
## Examples

 https://soundcloud.com/user-692410397/85-0353-1a

## Strategy
 
The basic strategy for ACS-85-0353 is the same as [ACS-85-0350](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0350) 


 


