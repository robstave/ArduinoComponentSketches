# ACS-85-0605
1 bit waveform selector
==============

## Overview:
Clock driven.

three pins selects from one of 8 waveforms.
Meant for higher frequency sounds, however it is clocked, so you can go as low as you would like.

![Waveforms](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0605/images/waveforms.png)

So, I had no idea how this would even sound, but it makes sense once you hear it.

Every subsequent waveform that you can select has a little longer pattern.  `0101` vs `010011`.
This turns out adding more bass for each pattern that adds longer cycles.  Maybe a bit like
clocking a counter and working your way down the divide pins, but a little different.

It will have a "divide by" sound, but still be a bit "octavish" as the divides ultimately fall on the fundemental of the clock signal and are not evenly spaced.

Here are some samples taken from this circuit:

![Waveforms](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0605/images/ACS-85-0700-605.png)

This is just a simple squarewave clocking the component and the wave forms are selected 0 - 7
https://soundcloud.com/user-692410397/85-0605-1a

This is PB2 of the ACS-85-0002 clocking the component.  There is a lot more going on in that signal.
https://soundcloud.com/user-692410397/85-0605-0002-1a



## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0605/images/ACS-85-0605.png)

 
## Improvements:
Consider moving them around for a more audible progression. 

 
