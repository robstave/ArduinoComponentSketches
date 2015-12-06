# ACS-85-0321
Divide by 2,3,4,5
==============

## Overview:
Basically a counter. If used with audio, div 2 is an octave lower
 
The div values are hard coded, but configurable in the software. Values other than 2-5 can be set with the following defines

    #define DIVA 2
    #define DIVB 3
    #define DIVC 4
    #define DIVD 5

 
## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0321/images/acs-85-0321.png)] (https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0321/images)


## Equivalent:

See Ken Stones divider
http://www.cgs.synth.net/modules/subosc.html

However, this is one of those cases, where I think a microcontroller excels and can reduce your chip count...depending on your requirements.
Obviously...if your looking for super high frequencies, a dedicated cmos counter is the way to go.  For a stand alone, modular thing, you can even 
work your way up to 20mhz clock speeds if you want to put a nano in there.

For humble bread board experiments though, this really cant be beat.

## Examples:
 

## Strategy:
 

## Improvements:


