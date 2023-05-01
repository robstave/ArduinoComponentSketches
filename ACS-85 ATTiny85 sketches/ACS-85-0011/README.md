# ACS-85-0011

Random LFO PWM out  (Psycho LFO)

## Overview

PWM LFO Output.

Similar output to Psycho LFO.

Output is PWM so a Low pass filter is needed to get an analog signal.

The random value is determined from a LFSR (Linear Feedback Shift Register)
https://github.com/robstave/ArduinoComponentSketches/wiki/LFSR

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0011/images/acs-85-0011.png)

## Equivalent

Its a basic equivalent to the [Ken stone Psycho lfo](http://www.cgs.synth.net/modules/psycho_lfo.html).

Kinda sorta.

## Strategy

- Use the LFSR to get a random number
- Use timer 0 to do fast PWM
- Use timer 1 to set the next value

## Examples

Here is an example of the VCO being driven by the LFO (ACS-85-0011).

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0011/images/ACS-85-circuit-0011-0002-1.png)

Here is another example of the VCO being driven by the LFO (ACS-85-0011). In this case we are
also inverting the the PWM to give an alternate voltage to the other pin.

The complementary PWM will be 100%-x. So if the main pin is pwm at 70%, the not of that is 30%

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0011/images/ACS-85-circuit-0011-0002-2.png)

## --

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0011/images/catlfo.jpg)
