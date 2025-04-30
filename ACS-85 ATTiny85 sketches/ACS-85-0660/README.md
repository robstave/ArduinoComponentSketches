# ACS-85-0660

3-Channel Tone Patterns

## Overview

This sketch provides tempo and scene selection for pre-programmed tone patterns.

The three outputs generate tones that start at a set frequency and detune downward. If you're feeling adventurous, you can add a few more accumulators to create cowbell-like sounds.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0660/images/ACS-85-0660.png)

## Improvements

Adding a sync feature would be great, but there are only so many pins available. If you need precise timing, consider driving this with a clock signal instead of relying on an analog potentiometer for tempo control. This sketch has room to evolve—perhaps with an ATtiny84 or even a Nano for more flexibility.

