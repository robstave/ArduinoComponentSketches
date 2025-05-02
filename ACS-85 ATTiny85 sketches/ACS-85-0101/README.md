# ACS-85-0101

ATTiny85 Blip selectable

## Overview

This sketch provides a debounced input to trigger a bleep. If you’ve ever wanted a simple chip with a button to trigger a sound, this is it.

This version is similar to ACS-85-0100 but without the noise channel. Instead, you can select the beepy, bloopy sounds you want.

There’s a pitch and a note length, and most of the configuration is internal.  
You can change some code and recompile for each variation.

The sound is based on DDS (Direct Digital Synthesis)—well, sort of. There’s no waveform, just a counter, and the top bit generates the square wave. Other sounds are created by manipulating the counters. Feel free to add your own!

See the square wave version here:  
[Technoblogy Square Wave DDS](http://www.technoblogy.com/show?QVN)

You can create all kinds of sounds by tweaking the accumulator. Generally, it’s:

```c
accumulator = accumulator + note;
```

If you modulate `note` by multiplying or dividing it over time, you get neat arpeggio sounds. That’s what’s happening here.

## Output

Although it looks like DDS, it’s just a square wave and not really PWM. There’s no "decay" or anything like that.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0101/images/ACS-85-0101.png)
