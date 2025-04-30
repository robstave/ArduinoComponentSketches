# ACS-85-0607

4-Channel Drum Patterns with Randomness

## Overview

This sketch generates three drum triggers with tempo control and a bit of randomness for flipping bits.

It’s similar to ![ACS-85-0603](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0603) and ![ACS-85-0604](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0604). However, unlike those, this version has no predefined patterns—the randomness is hardcoded. Just a tempo. Honestly, this one is my favorite.

The four outputs are configured as triggers, but you can tweak the trigger width in the code if needed.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0607/images/ACS-85-0607.png)

## Improvements

A sync feature would be a nice addition, but pin limitations make it tricky. If you need precise timing, consider using a clock signal instead of an analog potentiometer for tempo control.
