# ACS-85-0011 - Random LFO PWM Output (Psycho LFO)

## Overview

This sketch generates a PWM-based LFO output with a random modulation effect, inspired by the "Psycho LFO." The output is PWM, so you’ll need a low-pass filter to convert it into an analog signal.

The randomness is driven by an LFSR (Linear Feedback Shift Register). Learn more about LFSRs [here](https://github.com/robstave/ArduinoComponentSketches/wiki/LFSR).

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0011/images/acs-85-0011.png)

## Equivalent Circuit

This is a basic equivalent to the [Ken Stone Psycho LFO](http://www.cgs.synth.net/modules/psycho_lfo.html)
Well, kinda sorta.

## How It Works

- The LFSR generates a random number.
- Timer 0 handles fast PWM.
- Timer 1 sets the next value.

## Examples

Here is an example of the VCO being driven by the LFO (ACS-85-0011).

![Example Circuit](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0011/images/ACS-85-circuit-0011-0002-1.png)

Here’s another example where the PWM is inverted to provide an alternate voltage to another pin. The complementary PWM will be 100%-x. For instance, if the main pin is at 70% PWM, the inverted pin will be at 30%.

![Example Circuit](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0011/images/ACS-85-circuit-0011-0002-2.png)

 
