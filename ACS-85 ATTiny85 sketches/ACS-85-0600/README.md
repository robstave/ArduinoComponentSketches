# ACS-85-0600
Simple Sequencer with  analog scene select
==============

## Overview:
Was going for a programmable LFO but pretty much ended with a very simple sequencer.

Two Analog inputs.  One is speed, the other is a scene select.
The scenes are hard coded in the form

```
    #define PATTERN_0_SIZE 8
    boolean  pattern_0[PATTERN_0_SIZE] = {HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW};
    #define PATTERN_1_SIZE 8
    boolean  pattern_1[PATTERN_1_SIZE] = { HIGH, HIGH, LOW, LOW, HIGH, LOW, HIGH, LOW};
```
	
The other pin is just the clock.



## Pinout:
![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0600/images/ACS-85-0600.png)

## Equivalent:
 
 Counter and switches (todo)
 
## Strategy:
 

## Examples:

 
