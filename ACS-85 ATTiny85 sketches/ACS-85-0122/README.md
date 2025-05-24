# ACS-85-0122

ATTiny85 Phase Distortion XY

## Overview

This is a simple version of phase distortion. For more details, see:  
<https://electricdruid.net/phase-distortion-synthesis/>

This sketch explores a specific example of phase distortion. For a better description, see ACS-85-0120.

The diagram explains the phase distortion concept, where an (x, y) point is selected, and a line is drawn from (0, 0) to (x, y) to (255, 255).

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/drawing.png)

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/xy1.gif)

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/xy2.gif)

The phase transfer function is essentially the waveform. With a ramp, it’s not particularly interesting. The CZ-1 version is unique because the phase change sounds like a filter sweep. It’s not a filter, but the math creates that effect.

In this case, the ramp always ends with a big drop, which introduces harmonics. Sweeping the CV values won’t produce a low-pass filter sound with this setup.

It works better with a sine wave:

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/sinexy1.gif)

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/sinexy2.gif)

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/cos.png)

It seems to work even better with a cosine wave.

## References

- <https://thesnowfields.com/manuals/An%20Insider%27s%20Guide%20to%20Casio%20CZ%20Synthesizers.pdf>  
- <https://electricdruid.net/phase-distortion-synthesis/>  
- <http://www.technoblogy.com/show?QVN>

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/ACS-85-0121.png)
