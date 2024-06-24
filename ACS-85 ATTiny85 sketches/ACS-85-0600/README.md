# ACS-85-0600

Simple Sequencer with analog scene select

## Overview

I was going for a programmable LFO but pretty much ended with a very simple sequencer.

Two analog inputs.  

* Speed
* scene select

The scenes are hard coded in the form

```c
    #define PATTERN_0_SIZE 8
    boolean  pattern_0[PATTERN_0_SIZE] = {HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW};
    #define PATTERN_1_SIZE 8
    boolean  pattern_1[PATTERN_1_SIZE] = { HIGH, HIGH, LOW, LOW, HIGH, LOW, HIGH, LOW};
```
 
The other pin is just the clock.

TODO: Maybe a sample of whats going on here. But, meh, its not too interested. Kinda abandoning this one.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0600/images/ACS-85-0600.png)

## Equivalent

 Counter and switches (todo)
