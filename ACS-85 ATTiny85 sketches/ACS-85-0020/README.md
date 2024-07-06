# ACS-85-0020

 ATTiny85 Squarewave LFO with Duty Cycle

## Overview

Two inputs:

- Frequency
- Duty cycle

Three Outputs:

- LFO as squarewave
- !LFO as squarewave
- LFO at 1/2 freq as squarewave

There is another pin that is putting out the LFO at 1/2 the freq, but thats just a free pin.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0020/images/ACS-85-0020.png)

## Equivalent

A 555 circuit perhaps.  Note, my biggest beef with using a 555 is that the duty cycle is usually dependent on the frequency.

## Examples
