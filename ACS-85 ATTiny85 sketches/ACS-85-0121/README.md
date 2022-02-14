# ACS-85-0121

 ATTiny85 Phase Distortion chainsaw

## Overview

Simple version of phase distortion.  See how its described here.
https://electricdruid.net/phase-distortion-synthesis/

In this case, its just the ramp again, but the phase ramp is reversed at a point indicated
by the cv. Effectively, it looks on the scope like a ramp down morphing into a ramp up.
Maybe more like this

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/drawing.png)

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/saw1.gif)

CV at 20%

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/saw2.gif)

CV at 50%

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/saw3.gif)

CV at 80%

https://theproaudiofiles.com/phase-distortion-synthesis/https://theproaudiofiles.com/phase-distortion-synthesis/

Note again, there seems to be a lot of definitions for this, but it is not
the one in the CZ-1 casio patent.  Part of what makes that one neat is that the 
way the phase change sounds like a filter sweep.  It is not, but it just turns out
that the math works that way.  

With little effort, you could add a wavetable for a sine wave and point each value to
that super easy. 

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/drawing2.png)

## References

https://electricdruid.net/phase-distortion-synthesis/

http://www.technoblogy.com/show?QVN

 

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/ACS-85-0121.png)

