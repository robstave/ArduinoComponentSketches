# ACS-85-0210

VCO with 2 Delays

## Overview

This is a VCO similar to ACS-85-0041, but with a twist: the tuning word is stored and replayed slightly later. Think of it as a MIDI delay. The value of the potentiometer is played back with a slight delay in the other taps.

It’s a bit like an Atari Punk Console, but the effect really shines when it’s in motion. This could lead to other drone experiments in the future, where things are constantly modulated.

For now, it’s just two taps (plus the immediate output). The term "tap" is a bit overloaded here. There’s a buffer with 255 values, and we’re tapping from two other indices.

```
   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
   |434|434|455|500|500|500|522|522|533|.......|222|222|222|233|233|233|255|
   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
                   ^             ^                   ^
                  tap           tap1                tuning  --->
```

The tuning word (note in DDS) is stored round-robin in the samples array. To determine Port B, we use the tuning word, and bits 1 and 2 are just the spread and spread/2 mod 127. This isn’t PWM; we’re just flipping bits. It sounds better when all bits are set at once.

The size of 127 is chosen for simplicity in math, but there’s enough memory for 100 more samples. Unfortunately, not enough to make it a clean 256 integers.

When mixed, there are some phasing effects. Consider modulating the phase a bit to ensure it mixes well no matter what.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0210/images/ACS-85-0210.png)

## Examples

<https://soundcloud.com/user-692410397/acs-85-0210-sample>

<https://soundcloud.com/user-692410397/acs85-0012-0210-a>

## Cat

![Cat](https://lh3.googleusercontent.com/pw/AP1GczPr8oC_y8la2EA-zTQqu4a1mIA5reTaqYLhK0J7-bXLDeNTFpuWEwP0SywIvVl-zL0Hdb9UYsb8e_0eiBEs2r_NN-5NpfGe29n7inf6-ry5yPvAPV8=w2400)
