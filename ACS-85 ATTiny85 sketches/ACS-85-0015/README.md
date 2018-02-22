# ACS-85-0015

Two Square wave LFOs

## Overview

Copy pasted from [85-0004](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004).  No frills two LFOs with different ranges.  Eliminates the need for a range pot.  Its just LFO.

These are just digital LFOs...no smoothing needed.  But if you were driving a an analog circuit you could use a capacitor for a glide.

Range    | Low   | High 
--- | --- | ---
LF1  |  1 Hz  | 16 Hz
LF2  |  .2 Hz  | 4 Hz

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0015/images/ACS-85-0015.png) 

## Equivalent

See [ACS-85-0004](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004) LFOs

## Improvements

There is a free pin.  Perhaps we need a sync.