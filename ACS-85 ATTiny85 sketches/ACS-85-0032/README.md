# ACS-85-0032

 ATTiny85 Clocked LFO

## Overview

LFO that uses a clock signal to set the tempo.

Despite the simple approach, it works pretty ok.
Its not well debounced ( none really), so consider a LPF on that pin. ( 1kohm/100nf or whatever)

Basically, I have a counter that acts like millis(), but instead of millis, I generally utilize both timer 0 and 1 and just add a counter to one of em.  Its not milliseconds, but its usually linear and workable.  

Each Clock trigger will reset the wavetable index and find the delta which is used to calculate the time until the next wavetable index.

There are 7 waveforms

Ramp up
Ramp Down
Triangle
Sine
Square
Random ( 4 notes per clock)
Steps ( random but alt with 0's)
  Example 0 33 0 29 0 143 0 200

There is also a mod that speeds things up.  Not really incrementally.  its a Goose it button that doubles or triples the frequency.

Feel free to tweak!

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0032/images/ACS-85-0032.png)

You Will need a low pass filter on the pwm pins.

## Notes

Do consider a LPF on the clock to do the debouncing for you.
