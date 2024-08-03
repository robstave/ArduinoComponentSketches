# ACS-85-0102

ATTiny85 Synth Drum

## Overview

Simple version of EHX space drum. Ah...really...simple. More of an inspiration really.

That drum has a start and end frequency, which is what Im trying to emulate. Poorly.

There is a "parameter" value too that will be used for "length", but feel free to change that.

## Output

Although it is DDS looking, its just a squarewave and not really PWM. There is no "decay" or anything like that.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0102/images/ACS-85-0102.png)

## Improvements

This is one that I think could be explored a bit more on the ATTINY84.  Perhaps with some noise mixed in...or a PWM version.  If I were to mix in noise, I would just use another  pin.
Maybe even 4 pins...one for noise, one for the above and two for a cowbell effect and mix them all in.

Also, note that contrary to my usual habit of putting the trigger on PB2, its on PB0.
Im not sure if there is a reason for that. Its just how it turned out.  Looks like this was
2020 before I was doing that.


## Related

[ACS-85-0102](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0102)

[ACS-85-0103](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0103)

## Cat

![cat](https://lh3.googleusercontent.com/pw/AP1GczN4V7c3jaEn1PuVa0TQKbjTyCYEcNe_QkAw2lVm4O-vy85D3_AqJ9p4BKirR1JyaW-S3ajkoGR29aRTJnx553KPvsPc9X7VuOXEaIKhtnNYntgxl0A=w2400)
