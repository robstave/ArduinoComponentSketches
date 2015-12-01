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
 