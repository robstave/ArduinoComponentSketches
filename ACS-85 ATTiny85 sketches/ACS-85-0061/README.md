# ACS-85-0061

Square Wave VCO with Second Frequency Pulse-Width Modulated by LFO

## Overview

This sketch implements a Voltage-Controlled Oscillator (VCO) using a 16-bit counter in a DDS-like fashion. For more details, check out [this article](http://www.technoblogy.com/show?QVN).

The key difference here is that instead of using the second counter for PWM, it’s used as an LFO. The LFO distorts the "wavetable" slightly, creating a boundary case of wavetable distortion. Instead of using just the uppermost bit, the full top 8 bits are taken, and the LFO value is subtracted to determine whether we’re above or below the bit-flip point.

It’s a simple approach, but you might notice some noise. Feel free to experiment!

To mix the outputs, you can sum them across the pins using resistors or a proper op-amp summer.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0061/images/ACS-85-0061.png)

## Related

Check out [ACS-85-0060](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0060) for a variation with frequency modulation.
