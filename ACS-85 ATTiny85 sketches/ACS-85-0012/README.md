# ACS-85-0012

Random LFO PWM Output with Glide

## Overview

This sketch generates a PWM LFO output with glide.  

The output is similar to the Psycho LFO, but since it’s PWM, a low-pass filter is required.

The random values are generated using an LFSR ([Linear Feedback Shift Register](https://github.com/robstave/ArduinoComponentSketches/wiki/LFSR)).

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0012/images/acs-85-0012.png)

## Equivalent

This is roughly equivalent to the [Ken Stone Psycho LFO](http://www.cgs.synth.net/modules/psycho_lfo.html). It’s not an exact match, but it’s close.

## Strategy

- Use the LFSR to generate random numbers.
- Use Timer 0 for fast PWM.
- Use Timer 1 to set the next value.

## Examples

### Driving 0066

https://soundcloud.com/user-692410397/acs85-0012-0066

In this example, the LFO drives ACS-85-0066, a VCO that outputs the fundamental frequency, a fifth, and an octave above. It also has a spread variable for detuning.

![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0012/images/ACS-85-circuit-0012-0066.png)

### Driving 0210

https://soundcloud.com/user-692410397/acs85-0012-0210-a
