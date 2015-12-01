# ACS-85-0700
Analog to Digital Decoder
==============

## Overview:
This sketch takes an analog input and converts it to a digital output on 3 pins.
Either as a single pin or 3 bit binary mode. 

### Binary mode (PB4 = HIGH)
It is really just a 3 bit AD converter.
The value on the analog pin is converted to 3 bits.
{000, 001, 010, ..., 111}
  
Use to load values into a shift register or counter. A great way to use a pot to control a CD4051 and select an analog channel.


![Binary](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0700/images/mode2.png)



### Single select mode  (PB4 = Low)
The value converts to one of three values.
{001, 010, 100}
Can be used to select things as well.
This can be used to convert a pot to control channels, gates or whatever. An example below uses the sketch and nands to make a signal selector.
![Single](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0700/images/mode1.png)


## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0700/images/acs-85-0700.png)] (https://github.com/robstave/ArduinoComponentSketches)

## Equivalent:
In Binary mode, it really is just a 3 bit A/D converter
In select mode, its l

## Examples:
 
 Sketch in Binary mode so that a pot can be used to address channels in a 8 channel analog mux/demux.
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0700/images/ACS-85-0700-example1.png)](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0700/images)

Sketch in Single mode to control ANDs to gate separate signals
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0700/images/ACS-85-0700-example2.png)](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0700/images)


( 