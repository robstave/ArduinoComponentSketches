# ACS-84-0010
Simple Squarewave Oscillator - Beeps
==============

## Overview:
This is a down and dirty oscillator that you can use to test other circuits.  
The frequencies are fixed.

Provides an assortment of arps and beeps

Pin    |  Description  
--- | --- 
pin 2 |  LFO (Trigger by arp array size)
pin 3 |  LFO (Trigger by swell array size)
pin 5 |  LFO (med freq, mix of Random 3)
pin 6 | Rand notes (all same length)
pin 7 | Rand notes2 (all same length)
pin 8 | Arp1
pin 9 | Arp2
pin 10 | Swell
pin 11 | Swell high
pin 12 | Swell low
pin 13 | Random notes 3 (mixed length)

Its probably easier to just hear the sounds than explain them.
but a quick visual could be these


 [![ArpUp](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-84%20ATTiny84%20sketches/ACS-84-0010/images/arpRampUp.png)] (https://github.com/robstave/ArduinoComponentSketches)
 [![ArpDown](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-84%20ATTiny84%20sketches/ACS-84-0010/images/arpRampDown.png)] (https://github.com/robstave/ArduinoComponentSketches)


## Pinout:
[![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-84%20ATTiny84%20sketches/ACS-84-0010/images/acs-84-0010.png)] (https://github.com/robstave/ArduinoComponentSketches)

## Notes:
This probably stretches the limits on how much work you can do in the interrupt.  It should be fast. 
If you modify and find your frequencies a bit lower, check the code by remove chucks of the interrupt handler.  If it drops in frequency, your handler is too big.
 

## Examples:
 
