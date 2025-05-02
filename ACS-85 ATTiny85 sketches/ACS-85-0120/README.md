# ACS-85-0120

ATTiny85 Phase Distortion Simple v1

## Overview

Phase distortion synthesis is an interesting alternative to classic subtractive synthesis. Rather than using a VCO/DCO with a harmonically rich waveform that you pass through a filter (subtracting harmonics) to achieve a timbre, it uses a simpler waveform that is distorted either by tweaking the phase in a wavetable or shaping the wave to effectively add harmonics.

It turns out there are even ways to achieve cutoffs, resonance (in a sense), and filter sweeps. This is then fed into your typical VCAs with all the envelopes.

The nice thing is that it can all be done with just a microcontroller. No super difficult DSP or analog filtering is needed. However, the ATTiny85 is still not exactly a good fit for this. Still, it makes for an interesting exploration.

This first sketch is just me looking at the WIKI and seeing if I can do part of that.  
It is almost exactly like the first two steps in <https://en.wikipedia.org/wiki/Phase_distortion_synthesis> without the windowing.

An additional bool pin (pin 0) applies dampening to the last few values as in step 3 of the above.  
It’s subtle but makes the resonance sweep a bit more evident.

In this case, the value of the waveform is just the ramp. There is no envelope to the wave, so it is just a piece of it.

It would be dead simple to have this point to a wavetable (see ACS-85-121).

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/drawing.png)

## References

There is some code from Jan Ostman: <https://github.com/4-3is4-me/CZ1>  
It is supposed to be a version of the CZ1. I’m not sure how that math works out.  
Again, there are many strategies for phase distortion. This is more like the simplest.

<https://thesnowfields.com/manuals/An%20Insider%27s%20Guide%20to%20Casio%20CZ%20Synthesizers.pdf>

![Wave](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/wave.png)

![Attune](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/attune.png)

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0120/images/ACS-85-0120.png)
