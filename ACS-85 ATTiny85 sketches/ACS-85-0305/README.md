# ACS-85-0305
Collection of Xors
==============


## Overview:
Two Xors. Used to simulate an XOR in your signal path.  Rather than have to have pile of chips,
just program the ones you want!

Outputs are either `A ^ B` or `A ^ B ^ C`

Should work fine for audio frequencies but I would not use this an as real cmos logic chip.

 
## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0305/images/acs-85-0305.png)

## Equivalent:
The equivalent circuit would look something like this
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0305/images/ACS-85-0305-xor.png)

**PB0** is only going to **xor** the first 2 bits and **PB1** adds the next one.
 

## Examples:
 
Here is an example of the xor being driven by two squarewaves. In this case, the ACS-85-0004
 
https://soundcloud.com/user-692410397/85-0305xor-0004 
 
![Example](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0305/images/ACS-85-0305-0004.png)

 

## Strategy:

In this case, all the logic is in the loop. We are just reading the values and outputting them.  No interrupts or anything like that.
So this should be used as a non-time critical modulation.  There is no guarantee that signals will flip within a certain time limit.

## Improvements:

The highest frequencies that you can handle are going to depend on how fast the loop executes.  

 - Wrap the loop code into a while loop.   That would shave a few cycles off as loop would only be called once.  
 - Accumulate the bits and just mask off the last bit rather than all this ^ business.
 - Maybe save the value of port a and break if nothing changed?

