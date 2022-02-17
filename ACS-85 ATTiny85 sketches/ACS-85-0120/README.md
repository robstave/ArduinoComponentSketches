# ACS-85-0120

 ATTiny85 Phase Distortion Simple v1

## Overview

Phase distortion synthesis is an interesting alternative to classic substractive synthesis.  Rather than a VCO/DCO with a harmonically rich waveform that you pass through a filter ( subtracting harmonics ) to achieve a timber, it uses usually a simpler waveform that is distorted either by tweaking the phase in a wavetable or shaping the wave to effectively add harmonics.  

It turns out that there are even ways to achieve cutoffs, resonance ( in a sense ) and filter sweeps.  This is then feed into your typical VCAs with all the envelopes.

The nice thing is that it all can be done with just a microcontroller.  No super difficult DSP or analog filtering needed, however the ATTINY is still not exactly a good fit for this.  Still, it makes for an interesting exploration.

The first sketch is just me looking at the WIKI and seeing if I can do part of that.
This is almost exactly like the first 2 steps in https://en.wikipedia.org/wiki/Phase_distortion_synthesis without the windowing.

In this case, the value of the waveform is just the ramp. There is no envelope to the wave so it is just a piece of it.  

It would be dead simple to have this point to a wave table,  ( See ACS-85-121 )

![example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/drawing.png)

## References

There is some code from Jan Ostman https://github.com/4-3is4-me/CZ1

That is supposed to be a version of the CZ1.  Im gonna say Im not sure how that math works out.
Again, there are a many strategies for Phase distortion, this is more like the simplest.

https://thesnowfields.com/manuals/An%20Insider%27s%20Guide%20to%20Casio%20CZ%20Synthesizers.pdf

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/ACS-85-0120.png)

