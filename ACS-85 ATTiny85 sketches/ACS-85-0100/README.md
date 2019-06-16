# ACS-85-0100

 ATTiny85 Blip

## Overview

Debounced input to trigger a bleep.  If you were ever looking for a simple chip that had a button to trigger a sound. This is it.

There is a pitch and a note length.  From there, most of the configuration is internal.  The sound that is made is based on manipulating counters, although the generation is a super simple version of DDS.

Generally, for a beep, you would calculate the pitch you want and add that to an accumulator. See the squarewave version here.
http://www.technoblogy.com/show?QVN

You can make all kinds of sounds on top of that by tweaking the accumulator.

Gernerally, its  
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


## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0100/images/ACS-85-0100.png)

 