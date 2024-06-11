# ACS-85-0601

3 Pin Pattern Sequencer with analog scene select (Read only)

## Overview

Simple programmable patterns  (read only) that can be selected using the analog voltage.

Basically...its hard coded to be a drumm machine.

The patterns are programmed with 3 bit values in an array indicating which pin / state to set.

|     | 3 | 0 | 1 | 0 | 7 | 0 | 1 |   |
|-----|---|---|---|---|---|---|---|---|
| PB0 | x |   | x |   | x |   | x |   |
| PB1 | x |   |   |   | x |   |   |   |
| PB2 |   |   |   |   | x |   |   |   |

```c
    #define PATTERN_0_SIZE 16
    byte  pattern_0[PATTERN_0_SIZE] = {3, 0, 1, 0, 7, 0, 1, 0, 3, 0, 1, 0, 7, 0, 1, 0};
```

The voltage on A3 sets the clock speed and the voltage on A2 selects which pattern to play.

Its up to you to add more sequences in the code. It is not a tap and learn kinda thing.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0601/images/ACS-85-0601.png)

## Equivalent

Kinda like a bank of baby 10s?  But hard coded.

## Examples

Configure the circuit like this.

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0601/images/ACS-85-0601-ex.png)

## Other links

Can be used to trigger other circuits...maybe something like this

<http://www.sdiy.org/richardc64/new_drums/dr110/clap_etc.html>
