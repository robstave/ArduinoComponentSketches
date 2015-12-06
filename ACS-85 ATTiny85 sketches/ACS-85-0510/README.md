# ACS-85-0510
Shift Register
==============

## Overview:

Short Shift register
These are used in lunettas outputed to r2r ladders sometimes. One bit delay perhaps.  
This Sketch will not give you any more functionality that a CD4014, but its a good start for you to add on for other similar sketches.
 
This version is just a simple register using a long and outputing the 0, 4 and 8th bits.  Internally, its a unsigned int so thats 16 bits.

You can configure the output bits with the following code

    #define Q0 0
    #define Q1 8
    #define Q2 12

 
ACS-85-0512 does this with a much larger register
 
 
## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0510/images/acs-85-0510.png)]  


## Examples:

Here is an example that did not turn out super well...but still is a good example of chaining the sketches.
*The first Sketch is an LFO.  This clocks the shift register. 
*The button enters values into the shift register.
*At various taps, there are resistors to sum of the voltages. An r2r ladder is ideal.  I just threw something together.
*The sum of the volage goes to the VCO.

Clicking the button ends up with three outputs..each with a delay. This creates a voltage that drives the VCO.

![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0510/images/ACS-85-0015-0510-0006.png)  

The sound is here
https://soundcloud.com/user-692410397/85-0510-0006-0015a


## Other:

Here is a fun tutorial on using Shift Registers to get patterns and such. You should be able to use this sketch in a very similar manner.
http://hackaday.com/2015/05/04/logic-noise-taming-the-wild-shift-register/

 