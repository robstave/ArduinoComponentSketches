# ACS-85-0360

Counter with selectable divisors.  Defaults at divide by 8.

## Overview

Counter that can divide an input by 8, 4, 2 or 1 and output with a duty of 50% or just one clock (trigger).

The intent was to use in series with a medium speed clock to make an LFO that you can press buttons to speed up by 2, 4 and 8.

So say you have a clock going in at 8 beats/second.
The output would clock at 1 beat/second.

If you flip the divide, by 2 bit high, the speed will go faster
to 2 beats/second.

If you flip the other bit, you get 4 beats/second and if you
flip both, you get 8/second.

The effect that I was trying to achieve was a beat repeat sound.
Only, you need to feed in the clock at the fastest beat repeat you want.

At higher frequencies it will sound like an octave up or more.





## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0360/images/acs-85-0360.png)







