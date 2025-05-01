# ACS-85-0066

Drone: 3 Notes - Root, Fifth, and Octave with Detune

## Overview

This sketch generates a drone using three pins:
- **PB0**: Root note.
- **PB1**: One octave lower.
- **PB2**: A fifth above the lower octave.

All outputs are plain square waves.

There’s also a detune feature:
- **PB1** detunes downward.
- **PB2** detunes upward.

You can sum the outputs together using resistors or a proper op-amp summer. Feel free to tweak the detune settings as needed!

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0066/images/ACS-85-0066.png)

## Sample

Here is a sample with ACS-85-0012 driving as an LFO.
In this case, a Low Pass filter is needed
[[Sample](https://soundcloud.com/user-692410397/acs85-0012-0066)]
