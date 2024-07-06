# ACS-85-0210

VCO with 2  delays 

## Overview

Its a VCO like ACS-85-0041, but the tuning word is stored off and replayed
a little later on.  Perhaps the closest description is a Midi delay.  
The value of the pot is played just a little later in the other taps.

Kinda like an atari punk console, the effect only really works when its in motion.
This will lead to other drone things in the future where things are just constantly modulated.

For now though, its just two taps (and the immediate output)
The word Tap is overloaded here.  There is a buffer with 255 values and we are tapping from 2 other indices.

```
   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
   |434|434|455|500|500|500|522|522|533|.......|222|222|222|233|233|233|255|
   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                   ^             ^                   ^
                  tap           tap1                tuning  --->
                  
```

The Tuning word (note in dds) is stored round robin in the samples array.
To determine port B, we use the tuning word and then bit 1 and bit 2 are just
the spread and spread/2 Mod 127.  It it not PWM, we are just flipping bits.
It sounds better if all bits are set at once.

The size of 127 is chosen because the math is easy...but there is enough memory for 100 more
samples.  Alas...not enough to make it a easy 256 ints.

When mixed, there is some phasing effects.  Consider modulating the phase a bit to make it
mix no matter what.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0210/images/ACS-85-0210.png)

## Examples

<https://soundcloud.com/user-692410397/acs-85-0210-sample>

<https://soundcloud.com/user-692410397/acs85-0012-0210-a>

## cat

 ![cat](https://lh3.googleusercontent.com/pw/AP1GczPr8oC_y8la2EA-zTQqu4a1mIA5reTaqYLhK0J7-bXLDeNTFpuWEwP0SywIvVl-zL0Hdb9UYsb8e_0eiBEs2r_NN-5NpfGe29n7inf6-ry5yPvAPV8=w2400)
