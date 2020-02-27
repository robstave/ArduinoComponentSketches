# ACS-85-0570

Analog Shift register.  One output.


## Overview

Implements an analog shift register.  Of course, there are not a lot of pins to work with here, so
you can think of it more like a sample and hold with a delay of x registers?  
There is a write pin, so sequences can be played back.

The Write sets values from the input pin, otherwise they are rotated back from the last regsiter in the set.

You will need a LPF to convert the pwm to analog.
a 1k w 2.2uf should work fine in this case (the speed of analog write was boosted at the expense of
making delay() not usable.  Since its not used...no biggee.


![Overview](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0570/images/drawing.png)

## Pinout
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0570/images/ACS-85-0570.png)
 

## Equivalent
 
 
## Examples

https://soundcloud.com/user-692410397/acs-85-0570

In this case, I have an LFO ( Ramp Down ) feeding into the register and the output is going to a squarewave out VCO

 
