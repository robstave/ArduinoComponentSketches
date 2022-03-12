# ACS-85-0030

 ATTiny85 4 Channel PWM LFO  (with silence)

## Overview

One input:

- Speed

Four Outputs:

- LFO as PWM(4)
  Maybe an LFO is not quite the explaination here. It has a triangle ramp, with a random period for each ramp, but also a random pause between each ramp.  Consider using with LDRs to fade in and out ambient sounds.

This is good for ambient stuff that needs lots of slow lfos/envelopes.  
Keep in mind though that the PWM is pretty low.  800Hz.  its better suited for LDR/LED things or use a much lower pass freq for a LPF.

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0030/images/waveform.png)

Each interrupt increments both a counter oscCounter1 and readTimer. oscCounter1 is just a counter that rolls over at 255

PWM is done by checking the value of the channel vs the count.  If the counter is less than the value, the bit is 1.

Speed is 8,000,000 / ( 8 prescaler * (4+1)) so 200k
But the counter is 255 ( or later 128) steps...so its really like 784 hz
   
I dropped the resolution by incrementing by 128.  That helped, but again, this is not super good for fast things.  Slow slews...fine. With that Im getting 900hz pwm.  You would think it would double the PWM, but the interrupt is doing way more work than it needed to for a regular pwm situation and those cycles have a higher priority that "loop".

Still, you look at it as a black box that does what it does...np

ReadTimer increments every interrupt.  When it reaches the value of A3 ( Speed) it rolls over and thats a "tick".

V1 is slower than regular pwm for the trade off of 4 channels and pretty easy code,  but its probably best with led/ldrs rather than a lpf.  Or use a larger capacitor in the LPF.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0030/images/ACS-85-0030.png)

## Examples
