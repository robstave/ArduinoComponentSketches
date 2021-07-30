# ACS-85-0650

Bytebeat ( Bitbeat v1)
 
## Overview

Bytebeat is a term for a self generating sequence of bytes using a small equation.

Based on a snippet I found here
https://www.youtube.com/watch?v=8HQUXlStqK8https://www.noizhardware.com/index.html
https://www.noizhardware.com/index.html

I'm not going to get the story of this right, so instead, I will give you a link.

http://nightmachines.tv/the-absolute-beginners-guide-to-coding-bytebeats.html
Here is a great little article that about sums it all up and references all the folks
that were a part of its online explosion like viznut in the early 2010s.
(I Will try and put some info in the wiki)

Back to the base snippet https://pastebin.com/mfnKNWVD

In that code you had the following

    uint8_t out;
    int16_t t = 0;
    uint8_t PARAM_A;
    uint16_t potA16;
 
    void setup(){
        pinMode(SNDOUT, OUTPUT);
        pinMode(POT_A, INPUT);}
 
    void loop(){
        potA16 = analogRead(POT_A);
        PARAM_A = (int)(potA16>>2); /* ditch bits to go from 10bit to 8bit */
 
        out = (t%127)*((t-65&t)>>(PARAM_A/14+7));
 
        digitalWrite(SNDOUT, out);
     
        t++;
    }

Dead simple and does a LOT for little bit of code.  If your looking for a fun little
program to put on an ATTINY, this is it.  Demoscene by thy name.

Each time it loops, it checks the pot, calculates and out byte and increments t.
Sweet!  The only thing is that it outputs via DigitalWrite which really means that its the
last bit as a squarewave output.  Eh...still beeps right.

So, in this sketch we will extend things a little and point out some improvements that
can be made. ( Although...again..this is just fine as it is)

_Issue 1)_  Audio in the Loop:
  For Audio processing, you really want an interrupt at a constant speed.  In this case, we are
  reading the pot in the loop.  My usually trick to save CPU is to put that in a loop so that
  you only read the pot like every 5th time.  If you do that, the pitch goes UP, because the 
  loop processing time is down.  Wacky huh.  The more processing you do in the loop, the slower
  it will get. 
  
  Ok, we now have a tuning parameter.

_Issue 2)_ its a digitalWrite, so we are only using the Signigileast significant bit.  Its 
  bitbeat.  This is why you do not need to LPF the output too.  Its Squarewave out.
  No matter, it makes sound. Roll with it!

_ISsue 3)_ DigitalWrite is kinda slow.  you can speed things up with:

    if ( out == 0) {
      bitClear(PORTB, OUT1);
    } else {
      bitSet(PORTB, OUT1);
    }

  Really, if you know what your doing, you can avoid it.
  It has a lot of overhead built into it and its good to know why. You will write better
  programs and choose pins ( timers, pwm) better if you understand all the quirks that will
  make your program seem "buggy" because of all the unwritten "rules" behind those mega functions.

  This is not a biggie, so long as you have the CPU to cash your checks, does it really matter
  if you optimize digitalOut....no.  But we are playing with things to learn.


Anyways, using these steps, you can change the code to read another pot or swap between equations with easy because you have the tools to steal back cpu if it sounds slow.



In this case, I opted for more outputs and a different equation.  "running man?"

Since we are only outputing one bit, we can select different bits for different sounds.
Do bit 1 instead of 0

    if ( bitRead(out, 1) > 0) {
      bitSet(PORTB, OUT3);
    } else {
      bitClear(PORTB, OUT3);
    }

You can even combine things like Xoring two bits.

    // xor bits 1 and 2
    if ( bitRead(out, 1) ^  bitRead(out, 2) == 0) {
      bitSet(PORTB, OUT4);
    } else {
      bitClear(PORTB, OUT4);
    }

Each pin will sound kinda similar but different at the same time.

Sum them up, or R2R ladder the output.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0650/images/acs-85-0650.png)

## Equivalent

 
 

## Examples:
 