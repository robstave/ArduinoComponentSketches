# ACS-85-0101

 ATTiny85 Blip selectable

## Overview

Debounced input to trigger a bleep.  If you were ever looking for a simple chip that had a button to trigger a sound. This is it.

Similar to ACS-85-0101, without the noise channel.  Just selections.

There is a pitch and a note length.  From there, most of the configuration is internal.  The sound that is made is based on manipulating counters, although the generation is a super simple version of DDS.  DDS of a sqaure wave.

So, to create a sound of a certain pitch, you calculate the value of an offset (Note) which is added to an accumulator.  This indexes into a wave table.

The wave table is somewhat virtual though as you may have a 16 bit number, but only use the top bits to index into the actual table of values.  

In THIS case, we are only using the top bit, so there is not even the need for a table.  Its just a counter making a squarewave.

See the squarewave version here.
http://www.technoblogy.com/show?QVN

You can make all kinds of sounds on top of that by tweaking the accumulator.

Gernerally, its  
```
accumulator = accumulator + note
```

If you do things like modulate 'note' by multiplying by 2 or dividing by 2 over time, you get neat arp sounds.

Thats what is going on here.

There are 3 options.
 

## Output

Although it is DDS looking, its just a squarewave and not really PWM.  There is no "decay" or anything like that.


## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0101/images/ACS-85-0101.png)

## Pinout

![cat](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0101/images/cat-85-0101.jpg)

 