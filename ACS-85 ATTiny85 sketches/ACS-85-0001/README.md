# ACS-85-0001

Simple Squarewave Oscillator

## Overview

This is a down and dirty oscillator that you can use to test other circuits. No configuration pots or pins needed.

**The tuning fork of sketches.**

The frequencies are fixed, so if you want some control over the sound, this is not what you are looking for.  However, it is a good indicator if your cpu fuses are where they are supposed to be.

They should be in the 430-440 range.

- Two High Frequency square waves
- Two LFO and a ramping frequency

Note that the frequency here is not going to be exact. We are using the internal 8mhz clock..so you will see as much as +/- 15HZ on what the sketch says.  Its not too important here.  Again, we are just looking for a squarewave version of _Hello World_ or blinky light.  If you need more
accuracy, you will need to tie up some pins to a crystal.

The prescaler is 64 and OCR0A is 5 so interrupts are at 20.8 Khz.

```
8Mhz/(64*(OCR0A +1)) = 8mhz/(64*6) = 20833
```

We are just counting a value up and flipping a bit.

One of the counters is 24 so

```
20833 / (24 * 2) = 434 hz
```

If you are off by a lot, check the frequency...if it is close to 50 hz, then you have not set up your ATTINY85 fuses and your chip is still running at the factory set 1Mhz.  Run bootloader perhaps?


## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0001/images/acs-85-0001.png)

## Equivalent

Mostly used to replace a 40106 oscillator for a quick audio check.

![Equivalent](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0001/images/ACS-85-0001-overview.png) 

## Examples

[Sample](https://soundcloud.com/user-692410397/85-0001a) Not really stellar.  Its just a square wave.  But at the end is the ramp.