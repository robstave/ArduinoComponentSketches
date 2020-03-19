# ACS-85-0220

Debounced trigger to gate

## Overview

Basically a trigger to gate, with some coding to handle debouncing.

You can control the length of the gate, but additionally, you can subdivide the gate.
So for example, if the triggered gate was 500ms and you had the ticks to 1 ( just ground the pin),
it would be like a normal trigger to gate.

If you had the ticks set to 5, it would divide that 500ms into 100ms intervals and alternate between them in
an on, off fashion.  The intent was to have like a half note gate that I could turn into 1/16 notes on the fly.
The length of the total does not change, but the last interval could be silence.

This does not produce audio, just gates.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0220/images/ACS-85-0220.png)
 
## Examples
