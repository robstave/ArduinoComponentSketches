# ACS-85-0110

 ATTiny85 High hats

## Overview

This is a retool of Jan Ostmans ATTINY85 sample player dsp-Host Drum8..
Its a bit different though.  There are 5 samples of various
High Hat Sounds.  Gonna say they are kinda short. Maybe you can
crank down the sample rate for smaller sizes

In this case, there are 5 samples.  The Sample select also tunes the
sample.  I did not make tuning a separate CV...but go for it.
It just did not work well in the place I was going to use it.

An Idea for the mod ( open cv pin) if you want is an accent or mute.

So, getting these samples are kinda a chore.

I edited them to mono and clipped in Audacity.
Then I used <https://www.online-convert.com/> to convert it to 8 bit 8k sampling rate.
Somehow, audacity does this, but It never worked for me. This site does it well.
  
Then I used this website to create the sample code.
  <https://synthworks.eu/attiny85-drum-creator/>
  
  Attiny85 has only 8k to work with.  So I have only 5 samples

## References

<https://synthworks.eu/attiny85-drum-creator/>

 <https://modwiggler.com/forum/viewtopic.php?t=160118&start=150>

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0110/images/ACS-85-0110.png)
