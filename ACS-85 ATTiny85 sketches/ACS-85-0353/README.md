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

This may help or confuse you more....dunno.

![Notes](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0353/images/ACS-85-0353_notes.png)


The code is sampling the input clock to find effectively the same frequency that is being put in.
Basically there is a counter that is always running really fast.  The clock is going at a slower speed.
If it takes 50 counts per clock cycle, then that is my output counter speed as well.

Similar to the harmonizer.

In addition we periodically calculate a random number with the LFSR.  This is used to find
an offset to increase or decrease the frequency.

The offset is determined by masking bits of the random number.

The lowest three bits would be a number from 0 to 7. So for example:
 
```
    return lfsr & B00001111;
```

In this case, we mask off the lower 4 bits for a number from 0 to 15. Then we add that to oscillator counter or substract for a lower or higher frequncy.

```
     oscFreq1 = diff + getOffset();
     oscFreq2 = diff - getOffset();
```

**PB0** will have a counter value higher than the sampled counter from the carrier, resulting in a lower frequency.

**PB1** will have a counter value lower than the sampled counter from the carrier, resulting in a higher frequency.

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


 


