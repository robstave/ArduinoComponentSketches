# ACS-85-0122

 ATTiny85 Phase Distortion xy

## Overview

Simple version of phase distortion.  See how its described here.
https://electricdruid.net/phase-distortion-synthesis/

Thats just a part of what phase distortion synthesis is.  Im just exploring that
little example.  See ACS-85-120 for a better discription

In this case, it is truely the diagram that explains the phase distortion where it looks
like they picked and x,y point and drew a line from 0,0 to x,y to 255,255.

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/drawing.png)

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/xy1.gif)

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/xy2.gif)

The phase transfer function is truely the waveform too.  Gonna say though, with a ramp its not super interesting.  Part of what makes the CZ-1 version neat is the way the phase change sounds like a filter sweep.  It is not, but it just turns out that the math works that way.  

In this case, you can see that no matter how you phase it, there is still that big ramp drop at the end which is going to give you those harmonics. Sweeping the cv values simply will not have a LPF sound with that.

It does work better with a sine though

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/sinexy1.gif)

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/sinexy2.gif)

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0122/images/cos.png)

It does seem to work a bit better with cosine.

## References

https://thesnowfields.com/manuals/An%20Insider%27s%20Guide%20to%20Casio%20CZ%20Synthesizers.pdf

https://electricdruid.net/phase-distortion-synthesis/

http://www.technoblogy.com/show?QVN

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/ACS-85-0121.png)
