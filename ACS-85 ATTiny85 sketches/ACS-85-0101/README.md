# ACS-85-0101

ATTiny85 Blip selectable

## Overview

Debounced input to trigger a bleep. If you were ever looking for a simple chip that had a button to trigger a sound. This is it.

Similar to ACS-85-0101, without the noise channel. Instead you can select the beepy bloopy sounds you want.

There is a pitch and a note length. From there, most of the configuration is internal.  
Change some code, recompile for each variation. ( See ASC-85-0101) to allow you to
select.

The sound that is made is based on DDS, kinda. There is no waveform, just the counter and
the top bit is the squarewave. Other sounds are based on this and manipulating counters.
Feel free to add others.  

See the squarewave version here.
<http://www.technoblogy.com/show?QVN>

You can make all kinds of sounds on top of that by tweaking the accumulator.

Gernerally, its

```c
accumulator = accumulator + note
```

If you do things like modulate 'note' by multiplying by 2 or dividing by 2 over time, you get neat arp sounds.

Thats what is going on here.

There are 3 options.

## Output

Although it is DDS looking, its just a squarewave and not really PWM. There is no "decay" or anything like that.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0101/images/ACS-85-0101.png)
