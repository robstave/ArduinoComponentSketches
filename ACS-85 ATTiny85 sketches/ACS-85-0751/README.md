# ACS-85-0751

I2C Client - Button

## Overview

I2C Client that transmits the value of a button (0 or 255). In this sketch, we are just sending the value of a pin over the I2C Bus.

It is waiting for the request, so it's not pushing data; it's being polled.

This is a proof-of-concept (POC), although it can be a starting point for learning or building something interesting.

Requires the ATTINYCORE library. That is a good one that implements `Wire.h`. So just go with it. Don't mess with TinyWireM/S, although I'm sure it has useful features. It's mostly that I could not get it to work any other way (85 <--> 85).

Originally, the internet landed me here: <https://thewanderingengineer.com/>  
It was a good start, and I'll have another sketch for rotary encoders that uses the code there.

Still, my advice is to stick to ATTINYCORE for ATTiny85 and its included `Wire.h`.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0751/images/acs-85-0751.png)

## Examples

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0750/images/i2c.png)

## Related

![I2C Master 1 Byte](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0750)

![I2C Client Button Byte](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0751)

![I2C Client Rotary Byte](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0252)

## Cat

![Pinout](https://lh3.googleusercontent.com/pw/AP1GczOfxbZ4TU_b8q7HvcPrikV8SOJfFngaHDzCiDPOmRM90dqBlgp7oeCiRpHvFp7umPD6rWYtiLo3i0gusfAPpiLueFSoO3c2A27s2xU14TOck28uCTY=w600-h315-p-k)
