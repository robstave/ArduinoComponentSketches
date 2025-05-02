# ACS-85-0102

ATTiny85 Synth Drum

## Overview

This is a simple version of the EHX Space Drum. Well, more of an inspiration, really.

The drum has a start and end frequency, which this sketch tries to emulate—poorly, but it’s fun! There’s also a "parameter" value used for "length," but feel free to change that.

## Output

Although it looks like DDS, it’s just a square wave and not really PWM. There’s no "decay" or anything like that.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0102/images/ACS-85-0102.png)

## Improvements

This is a sketch that could be explored further on the ATTiny84. Perhaps add some noise mixed in—or a PWM version. If I were to mix in noise, I’d just use another pin. Maybe even four pins: one for noise, one for the above, and two for a cowbell effect, then mix them all in.

Also, note that contrary to my usual habit of putting the trigger on PB2, it’s on PB0. I’m not sure if there’s a reason for that—it’s just how it turned out. Looks like this was from 2020 before I started doing that.

## Related

- [ACS-85-0102](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0102)
- [ACS-85-0103](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0103)

## Cat

![Cat](https://lh3.googleusercontent.com/pw/AP1GczN4V7c3jaEn1PuVa0TQKbjTyCYEcNe_QkAw2lVm4O-vy85D3_AqJ9p4BKirR1JyaW-S3ajkoGR29aRTJnx553KPvsPc9X7VuOXEaIKhtnNYntgxl0A=w2400)
