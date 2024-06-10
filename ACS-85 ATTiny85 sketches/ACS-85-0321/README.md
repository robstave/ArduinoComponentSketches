# ACS-85-0321

Divide by 2,3,4,5

## Overview

Basically a counter. If used with audio, div 2 is an octave lower.
 
The div values are hard coded, but configurable in the software. Values other than 2-5 can be set with the following defines

```
    #define DIVA 2
    #define DIVB 3
    #define DIVC 4
    #define DIVD 5
```
 
## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0321/images/acs-85-0321.png) 

## Equivalent

See [Ken Stones divider](http://www.cgs.synth.net/modules/subosc.html).

However, this is one of those cases, where I think a microcontroller excels and can reduce your chip count, depending on your requirements.

Obviously, if your looking for super high frequencies, a dedicated cmos counter is the way to go.  

For humble bread board experiments though, this really can't be beat.  Divide by odd numbers is a bit of circuitry to do on a breadboard.

## Examples

## Strategy

## Improvements
