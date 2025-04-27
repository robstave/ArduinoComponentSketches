# ACS-85-0007 - Square VCO with Frequency Ramping

## Overview

This sketch demonstrates a square wave generator with a variable frequency ramping effect, using the ATTiny85. The frequency changes dynamically by adjusting the period with counters. Each time the pin flips, the period increases, which lowers the frequency. The size of the increase is masked by 3, 4, or 5 bits, creating unique sound effects for each pin.

- **PB0**: Subtle frequency ramp with a "fat square wave" sound. The counter is 3 bits, so it spins fast and creates a warble effect.
- **PB1**: A more dramatic ramp with a 4-bit counter. The lowest frequency is lower, and the ramp period is longer.
- **PB2**: The most dramatic ramp, producing a "raygun" sound with a 5-bit counter.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0007/images/ACS-85-0007.png)

## Examples

[Listen on SoundCloud](https://soundcloud.com/user-692410397/85-0007a)

This recording tests each pin, from PB0 (fat square wave) to PB2 (raygun).

## Improvements

There’s still room for experimentation! You could use the open pin to:

- Toggle between ramping up or down.
- Change the clock divider by 2 or 4 for even more variety.
