# ACS-85-0612

Coin Flip clocked

## Overview

Similar to Mutable instruments Branches.
Triggers on rising edge of clock and outputs on either a or b pin based on a set probability.  

If the prob is 50%, then a trigger on pb02 will result in either PB0 or PB1 going high evenly.
Changes in the prob skews the results towards one pin or the other.

A Toggle pin sets the out to be either a trigger or a latched output.

Length of trigger, if that mode is selected, is set in the code.

In Latched mode, the pin remains high until the value on A/B changes.

Similar to ![ACS-85-0613](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0613), and ![ACS-85-0614](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0614).

![Diagram](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0612/images/diagram.png)

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0612/images/ACS-85-0612.png)

