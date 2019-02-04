# ACS-85-0540
4 Channel VCO/LFO for on/off
==============

## Overview:
The intent was to have a source of frequencies, similar to a counter where each pin is a octave below the last.  In this case, there is an analog input to set the frequency and there is a div by 3 as well to get a nice third below.

Each channel has an LFSR to control its on/off state. 
In addition, each channel can be set up to warble a bit.  There is a parameter to do this.

## Configuration:

```
#define WARBLE 0
#define VCO_LO 80   //A bigger number means a longer count = lower freq
#define VCO_HIGH 8
```

**VCO_LO** and **VCO_HIGH** set the min/max of the analog input.  The pin is only read 8 times / second so it
is not really super reactive (it was intended for providing tones to an ambient circuit....but hey..experiment).  You can bump OCR0A from 39 to like maybe 20 then readjust for more range.  I
really do not have a clue how small you can make it, but you will know it when you hear it.
Listen for the interrupt happening faster than the code in the ISR can complete.

**WARBLE** gives it an 8-bit sound.  If you set it to 0, its just a clean tone.
 

## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0540/images/ACS-85-0540.png)

## Equivalent:
Similar to pinouts of a CMOS counter, ANDed to LFSR ( when warble is 0)

## Examples:

 

