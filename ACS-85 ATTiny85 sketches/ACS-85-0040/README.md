# ACS-85-0040

Square wave VCO with PWM and Chiptune sound

## Overview

Uses a 16 counter in a DDS fashion like. <http://www.technoblogy.com/show?QVN>

This is a vco with outputs.  One is the mainsqaurewave and the other is the same
frequency, but adjustable width.

There is a mode that makes a chiptune sound too which happens if you flip pin 5. This basically scrambles the DDS. ( arp up)

This is really a bit of a hot mess, but I put it into my hybrid-lunetta project anyways and has become a favorite of mine.

See ![ACS-85-0041](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0041)  and ![ACS-85-0042](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0042) where the pwm and chip sounds were split into their respective sketches.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0040/images/ACS-85-0040.png)