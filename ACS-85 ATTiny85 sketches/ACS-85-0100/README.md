# ACS-85-0100

ATTiny85 Blip

## Overview

This sketch provides a debounced input to trigger a bleep. If you’ve ever wanted a simple chip with a button to trigger a sound, this is it.

There’s a pitch and a note length, and most of the configuration is internal.  
You can change some code and recompile for each variation. (See ACS-85-0101 for a version that allows you to select sounds.)

The sound is based on DDS (Direct Digital Synthesis)—well, sort of. There’s no waveform, just a counter, and the top bit generates the square wave. Other sounds are created by manipulating the counters.

See the square wave version here:  
[Technoblogy Square Wave DDS](http://www.technoblogy.com/show?QVN)

You can create all kinds of sounds by tweaking the accumulator. Generally, it’s:

```c
accumulator = accumulator + note;
```

If you modulate `note` by multiplying or dividing it over time, you get neat arpeggio sounds. That’s what’s happening here.

### Sound Modes

There are three sound modes:

```c
#define SINGLE_SOUND 0
#define ROTATE_SOUND 1
#define RANDOM_SOUND 2
```

- **SINGLE_SOUND**: Plays a selected sound. Set it in the `sound` variable.
- **ROTATE_SOUND**: Rotates through the different sounds in a round-robin fashion.
- **RANDOM_SOUND**: Picks a new sound for each note.

## PWM and Mixing

The DDS ultimately just flips a bit rather than setting the OCRA. It’s not really PWM, but you might consider adding a filter with a higher corner frequency—or not.

There’s also a noise pin, so you can use this for a drum sound and mix in noise too.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0100/images/ACS-85-0100.png)

## Cat

![Cat](https://lh3.googleusercontent.com/pw/AP1GczN1pW-NlZ_3pmSYMmXPf63qX8avGdPsXwuX4dK2z8Bi5cntuqmGGHDtP5wUjhqXfG5jtAKURfwlCSOam8pr-t-fd_ALhN9QF2WAr3IymBvfdopFy18=w2400)
