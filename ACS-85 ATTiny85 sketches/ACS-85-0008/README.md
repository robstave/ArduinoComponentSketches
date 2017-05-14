# ACS-85-0008
 ATTiny85 Squarewave with Vibrato

In a sense, kinda like 0007.  In that case, it was a ramp down. A better vibrato
is when you modulate the frequency up and down with a sine or triangle wave.

## Overview:

 Two inputs.  One for Freq and the other for Vibrato width.
 The outputs are:

 - **PB0** - Frequency ramps down
 - **PB1** - Frequency Ramps Up
 - **PB2** - Frequency goes up and down as a Triangle wave

## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0008/images/ACS-85-0008.png)

## Equivalent:

Really...just a LFO driving a VCA.

## Examples:

 https://soundcloud.com/user-692410397/85-0008a
 
 
## Improvements
Its all pretty in your face and could be a little more nuanced.
85-0009 is reserved for sine wave when I get around to fine tuning this a little better.  Or, Im open for submits
