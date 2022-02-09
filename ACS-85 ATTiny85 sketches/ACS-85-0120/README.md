# ACS-85-0120

 ATTiny85 Phase Distortion Simple v1

## Overview

Simple version of phase distortion.  It is almost exactly like the first 2 steps in
https://en.wikipedia.org/wiki/Phase_distortion_synthesis

In this case, its just the ramp.  There is no envelope to the wave.  The Hard stop is there.
It would be dead simple to have this point to a wave table, the but phase accumulator is basically used as a ramp with a little right shifting of the bits.
 

## References

There is some code from Jan Ostman https://github.com/4-3is4-me/CZ1

That is supposed to be a version of the CZ1.  Im gonna say Im not sure how that math works out.
Again, there are a many strategies for Phase distortion, this is more like the simplest.

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/drawing.png)

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/ACS-85-0120.png)

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/ACS-85-0120.png)