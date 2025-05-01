# ACS-85-0034 ATTiny85

Lorenz Attractor

## Overview

This sketch implements the Lorenz Attractor. It’s not groundbreaking, but it seems to exhibit the correct behavior.  I think?

I initially tried generating this with ChatGPT, but while the math was solid, the implementation didn’t work. So, this is a rewrite. Feel free to experiment and improve it!

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0034/images/ACS-85-0034.png)

You’ll need a low-pass filter on the PWM pins for smoother output.

## Notes

I’ve enhanced the `analogWrite` function by tweaking the timer, but this interferes with `delay()`. To compensate, I created a custom delay-like function using Timer 2.





