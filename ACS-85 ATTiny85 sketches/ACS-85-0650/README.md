# ACS-85-0650

Bytebeat (Bitbeat v1)

## Overview

Bytebeat refers to a self-generating sequence of bytes using a small equation. It's a fascinating way to create sound with minimal code.

This sketch is based on a snippet I found here:  
<https://www.youtube.com/watch?v=8HQUXlStqK8>  
<https://www.noizhardware.com/index.html>

For a great introduction to Bytebeat, check out this article:  
<http://nightmachines.tv/the-absolute-beginners-guide-to-coding-bytebeats.html>  
It covers the history and references key contributors like Viznut, who helped popularize Bytebeat in the early 2010s.

The original snippet can be found here:  
<https://pastebin.com/mfnKNWVD>

The code is simple yet powerful. Each loop reads a potentiometer, calculates an output byte, and increments `t`. It's a fun and compact program for the ATTiny85.

## Issues and Improvements

1. **Audio in the Loop**:  
   Audio processing works best with a constant-speed interrupt. In this sketch, the potentiometer is read in the loop, which affects pitch. A simple trick is to read the potentiometer less frequently to save CPU cycles.

2. **DigitalWrite Output**:  
   The sketch uses `digitalWrite`, which outputs only the least significant bit as a square wave. While it works, it's more of a "bitbeat" than a true Bytebeat.

3. **Optimization**:  
   `digitalWrite` can be slow. You can speed it up with direct port manipulation:
   ```c
   if (out == 0) {
       bitClear(PORTB, OUT1);
   } else {
       bitSet(PORTB, OUT1);
   }
 ```

  Really, if you know what your doing, you can avoid it.
  It has a lot of overhead built into it and its good to know why. You will write better
  programs and choose pins ( timers, pwm) better if you understand all the quirks that will
  make your program seem "buggy" because of all the unwritten "rules" behind those mega functions.

  This is not a biggie, so long as you have the CPU power to cash your checks, does it really matter if you optimize digitalOut? No. But we are playing with things to learn.

Anyways, using these steps, you can change the code to read another pot or swap between equations with easy because you have the tools to steal back cpu if it sounds slow.

In this case, I opted for more outputs and a different equation.  "running man?"

Since we are only outputing one bit, we can select different bits for different sounds.
Do bit 1 instead of 0

```c
    if ( bitRead(out, 1) > 0) {
      bitSet(PORTB, OUT3);
    } else {
      bitClear(PORTB, OUT3);
    }
```

You can even combine things like Xoring two bits.

```c
    // xor bits 1 and 2
    if ( bitRead(out, 1) ^  bitRead(out, 2) == 0) {
      bitSet(PORTB, OUT4);
    } else {
      bitClear(PORTB, OUT4);
    }
```
This will give you a different sound than just using one bit. You can even use the same bit for two outputs.

```c
    // xor bits 1 and 2
    if ( bitRead(out, 1) ^  bitRead(out, 2) == 0) {
      bitSet(PORTB, OUT4);
      bitSet(PORTB, OUT3);
    } else {
      bitClear(PORTB, OUT4);
      bitClear(PORTB, OUT3);
    }
```

Each pin will sound kinda similar but different at the same time.

Sum them up, or R2R ladder the output.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0650/images/ACS-85-0650.png)

## Equivalent

This sketch is a great starting point for exploring Bytebeat. It's simple, fun, and perfect for experimenting with sound generation on the ATTiny85.

## Related Sketches

- [Bytebeat (Bitbeat v1)](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0650)
- [Bytebeat (Bitbeat v2)](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0651)
- [Bytebeat (Bitbeat v2) clocked](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0652)
- [Bytebeat (Bitbeat 2)](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0653)

## Other Links

- <https://medium.com/@naive_sound/glitch-a-noisy-fusion-of-math-and-music-6a9b24e7f5b5>
- <https://noisio.de/>

## Examples
