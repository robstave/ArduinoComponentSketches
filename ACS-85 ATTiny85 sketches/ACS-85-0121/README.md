# ACS-85-0121

ATTiny85 Phase Distortion Chainsaw

## Overview

This is a simple version of phase distortion. For more details, see:  
<https://electricdruid.net/phase-distortion-synthesis/>

In this case, the ramp is reversed at a point determined by the CV. On a scope, it looks like a ramp down morphing into a ramp up. Something like this:

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/drawing.png)

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/saw1.gif)

CV at 20%

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/saw2.gif)

CV at 50%

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/saw3.gif)

CV at 80%

<https://theproaudiofiles.com/phase-distortion-synthesis/>

Note: There are many definitions of phase distortion, but this is not the one described in the CZ-1 Casio patent. What makes that version unique is how the phase change sounds like a filter sweep. It’s not a filter, but the math creates that effect.

With minimal effort, you could add a wavetable for a sine wave and easily point each value to it.

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/drawing2.png)

## References

- <https://electricdruid.net/phase-distortion-synthesis/>  
- <http://www.technoblogy.com/show?QVN>  
- <https://thesnowfields.com/manuals/An%20Insider%27s%20Guide%20to%20Casio%20CZ%20Synthesizers.pdf>

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0121/images/ACS-85-0121.png)
