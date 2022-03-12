# ACS-85-0012

Random LFO PWM out with glide

## Overview

PWM LFO Output with glide.

Similar output to Psycho LFO. Output is PWM so a Low pass filter is needed.

The glide is achieved by averaging previous values.

The random value is determined from a LFSR [Linear Feedback Shift Register](https://github.com/robstave/ArduinoComponentSketches/wiki/LFSR)

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0012/images/acs-85-0012.png)

## Equivalent

Its a basic equivalent to the [Ken Stone Psycho LFO](http://www.cgs.synth.net/modules/psycho_lfo.html).

Kinda sorta.

## Strategy

- Use the LFSR to get a random number.
- Use timer 0 to do fast PWM 
- Use timer 1 to set the next value

## Examples

### Driving 0066

https://soundcloud.com/user-692410397/acs85-0012-0066

in this case, we have the LFO driving ACS 85 0066 which is a vco that puts out 
the fundemental, a fifth and octave above.  It also has a spread variable that detunes things.

![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0012/images/ACS-85-circuit-0012-0066.png)

### Driving 0210

https://soundcloud.com/user-692410397/acs85-0012-0210-a
