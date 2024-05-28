# ACS-85-0621

Timed Gate


## Overview

Takes a logical input and Ands it with a timed gate When the trigger starts the gate 
it goes for a length defined by A3.

For the length of that gate, PB0 is true and PB1 reflects the output of pin 7.

If the trigger is high longer than the timer, then its just on...but the gate will diable when the trigger does.

Best for Logical LFO and gates.  Prob not super good for audio as the value. of the input is written to the output in a loop.
   
An Optimization would be to set the output pin to the value of the input in the interrupt.
   (Actually I tried it and had issues so I gave up..its fine for low freqs)

![timin](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0621/images/0621.png)


## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0621/images/ACS-85-0621.png)




![Pinout](https://lh3.googleusercontent.com/pw/AP1GczNAEJUEg95uHpI8qy54ZwSy99SEWPdFA3lOAdKmxtcaeFBIrpO396svzrKC7tRPodbwYTKuWhdHCsl4DFD08tF5fbIH6CLrLWvdvZxSS-BF9f6ibnU=w2400)



## Timing references
https://nathantypanski.com/blog/2014-10-29-tikz-timing.html