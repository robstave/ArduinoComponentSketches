# ACS-85-0122

 ATTiny85 Phase Distortion xy

## Overview

Simple version of phase distortion.  See how its described here.
https://electricdruid.net/phase-distortion-synthesis/

In this case, it is truely the diagram that explains the phase distortion where it looks
like they picked and x,y point and drew a line from 0,0 to x,y to 255,255.

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/drawing.png)

The phase transfer function is truely the waveform too.  Gonna say though, with a ramp its not super interesting.  Part of what makes the CZ-1 version neat is that the 
way the phase change sounds like a filter sweep.  It is not, but it just turns out
that the math works that way.  In this case, you can see that no matter how you phase it,
there is still that big ramp drop at the end which is going to give you those harmonics.
It simply will not have a LPF sound with that.

With little effort, you could add a wavetable for a sine wave and point each value to
that super easy.

## References

https://electricdruid.net/phase-distortion-synthesis/

http://www.technoblogy.com/show?QVN

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/ACS-85-0121.png)
