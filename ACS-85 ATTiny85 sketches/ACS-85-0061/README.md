# ACS-85-0061

Square wave VCO with Second frequency pulse width modulated by LFO

## Overview

Uses a 16 bit counter in a DDS fashion like. <http://www.technoblogy.com/show?QVN>

The main difference is, rather than setting up the other counter for PWM, we are using it for an LFO.  The upper 8 bits would be used to point to a 256 size wave table, but since we are doing squarewaves, we can just use the uppermost bit.

The LFO is used to distort the "wavetable" a bit.  Perhaps this is a boundry case of wave table distortion.  Rather than using the upper bit, the full top 8 bits are taken and the lfo value is subtracted to determine if we are above or below the bit flip point.  Its pretty simple and Im hearing some noise I think.  Hard to tell.

To mix, just sum across the pins with resistors  ( or use a real op amp / summer)

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0061/images/ACS-85-0061.png)

## Related

![ACS-85-0060](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0060)
