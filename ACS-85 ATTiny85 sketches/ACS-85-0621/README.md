# ACS-85-0621

Timed Gate

## Overview

This sketch takes a logical input and ANDs it with a timed gate. When the trigger starts, the gate activates. The output remains active for a duration defined by A3.

During the gate's active period:
- PB0 is set to true.
- PB1 reflects the state of pin 7.

If the trigger remains high longer than the timer, the output stays on. However, the gate will disable as soon as the trigger goes low.

This is ideal for logical LFOs and gates. It's not optimized for audio, as the input value is written to the output in a loop.

For optimization, you could set the output pin to the input value in the interrupt. (I tried this but ran into issues, so I left it as-is. It works fine for low frequencies.)

![Timing Diagram](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0621/images/0621.png)

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0621/images/ACS-85-0621.png)

<br>
<br>
<br>

![Pinout](https://lh3.googleusercontent.com/pw/AP1GczNAEJUEg95uHpI8qy54ZwSy99SEWPdFA3lOAdKmxtcaeFBIrpO396svzrKC7tRPodbwYTKuWhdHCsl4DFD08tF5fbIH6CLrLWvdvZxSS-BF9f6ibnU=w2400)

## Timing References

For more on timing diagrams, check out this resource:  
<https://nathantypanski.com/blog/2014-10-29-tikz-timing.html>
