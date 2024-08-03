# ACS-85-0100

ATTiny85 Blip

## Overview

Debounced input to trigger a bleep. If you were ever looking for a simple chip that had a button to trigger a sound. This is it.

There is a pitch and a note length. From there, most of the configuration is internal.  
Change some code, recompile for each variation. ( See ASC-85-0101) to allow you to
select.

The sound that is made is based on DDS, kinda. There is no waveform, just the counter and
the top bit is the squarewave. Other sounds are based on this and manipulating counters.

See the squarewave version here.
<http://www.technoblogy.com/show?QVN>

You can make all kinds of sounds on top of that by tweaking the accumulator.

Generally, its:

```
accumulator = accumulator + note
```

If you do things like modulate 'note' by multiplying by 2 or dividing by 2 over time, you get neat arp sounds.

Thats what is going on here.

There are 3 options.

```
#define SINGLE_SOUND 0
#define ROTATE_SOUND 1
#define RANDOM_SOUND 2
```

Use _SINGLE_SOUND_ to just play a selected sound. Set it in _sound_ and thats what you get.

Use _ROTATE_SOUND_ to rotate through the different sounds round robin.

Use _RANDOM_SOUND_ to pick a new sound for each note.

## PWM and Mixing

The DDS is ultimatately just flipping a bit rather that setting the OCRA. Its not really PWM, but you might consider a filter but with a higher corner frequency.

Or not.

There is a noise pin too...so you can use this for a drum and mix in noise too.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0100/images/ACS-85-0100.png)

## cat

![cat](https://lh3.googleusercontent.com/pw/AP1GczN1pW-NlZ_3pmSYMmXPf63qX8avGdPsXwuX4dK2z8Bi5cntuqmGGHDtP5wUjhqXfG5jtAKURfwlCSOam8pr-t-fd_ALhN9QF2WAr3IymBvfdopFy18=w2400)
