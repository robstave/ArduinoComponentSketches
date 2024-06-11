# ACS-85-0560

Frequency Sampler

## Overview

Similar to the Luna Mod for the Pic chip.
Two knobs and two buttons.

There are 256 samples that are stored in memory.  Twisting the value knob and write, sets the value in the memory. You can increase the sample speed, but the freq remains the same...it just shortens the window (although the number of samples are the same).

Setting the clear button writes 0 to the memory.  To clear memory fast, turn the length to the minimum then clear.  You have to hold
down the whole cycle.  Using the reset pin is an option as well.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0560/images/ACS-85-0560.png)

## Sample

Here is a Sample of the sketch in action.
https://soundcloud.com/user-692410397/85-0560-1a

## Strategy

The sampling takes place in the loop, so its kinda a best effort.  Counters have been added to make sampling a high priority than loop length or the buttons.  Its not super smooth and has room for improvement.