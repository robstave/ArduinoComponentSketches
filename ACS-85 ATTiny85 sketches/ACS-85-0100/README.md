# ACS-85-0100

 ATTiny85 Blip

## Overview

Debounced input to trigger a bleep.  If you were ever looking for a simple chip that had a button to trigger a sound. This is it.

There is a pitch and a note length.  From there, most of the configuration is internal.  The sound that is made is based on manipulating counters, although the generation is a super simple version of DDS.  DDS of a sqaure wave.

So, to create a sound of a certain pitch, you calculate the value of an offset (Note) which is added to an accumulator.  This indexes into a wave table.

The wave table is somewhat virtual though as you may have a 16 bit number, but only use the top bits to index into the actual table of values.  

In THIS case, we are only using the top bit, so there is not even the need for a table.  Its just a counter making a squarewave.

See the squarewave version here.
http://www.technoblogy.com/show?QVN

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
Use *SINGLE_SOUND* to just play a selected sound.  Set it in *sound* and thats what you get.

Use *ROTATE_SOUND* to rotate through the different sounds round robin.

Use *RANDOM_SOUND* to pick a new sound for each note.

## PWM and Mixing

The DDS is ultimatately just flipping a bit rather that setting the OCRA.  Its not really PWM, but you might consider a filter but with a higher corner frequency.

Or not.

There is a noise pin too...so you can use this for a drum and mix in noise too.


## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0100/images/ACS-85-0100.png)

## cat

![cat](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0100/images/catbeep.jpg)


 