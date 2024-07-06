# ACS-85-0031

 ATTiny85 3 phase ramp LFO Clocked

## Overview

OK...fine for drone/lunetta...but timer1 is 500hz pwm...meh

One input:

- Clock

Three Outputs:

- LFO as PWM(3)
  Three Ramp functions using PWM.

Note this uses fast PWM for 2 of the pins...but one of them is a bit lower in resolution.  
PWM is FAST for PB0/PB1 but much slower on PB4. Like 30khz vs 500 hz I think.

I might rewrite this because of that, but visually, and if your using LEDs, it looks fine.
A Good discussion is here: <https://forum.arduino.cc/t/pwm-frequency-change-attiny85/170321>

<https://forum.arduino.cc/t/attiny85-fast-pwm-on-timer-1/993958/14>

I would say not ready for prime time, but workable.

Not a bad lauching point though for you to experiment with.  For example:

- Ditch P4 and just use the 2 good PWMs
- Make one go half speed or double... or backwards
- Read from a wavetable with just one counter.  The counter that is clocked in the interrupt does not have to be directly coupled to the analog out by any means.  They can also represent a state

There is a spare pin...consider using that for a sync.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0031/images/ACS-85-0031.png)

You Will need a low pass filter on the pwm pins.

## Notes

## Cat

![Pinout](https://lh3.googleusercontent.com/pw/AP1GczOC1K8SlLH3o_JWJbeBLbpNGRXPGJOU2n_DlStPM9GIqVDu2-h75HmkeozqkKQSol8QVhl8gmYhv1EUyc8cFQQXYUesFKexASVdpav7LZkyZ5N6aDQ=w2400)
