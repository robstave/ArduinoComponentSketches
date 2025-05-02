# ACS-85-0111

ATTiny85 Snares

## Overview

This is a retool of Jan Ostman’s ATTiny85 sample player DSP-Host Drum8.  
It’s a bit different though. There are five samples of various high-hat sounds.  
They are kind of short. Maybe you can crank down the sample rate for smaller sizes.

In this case, there are five samples. The sample select also tunes the sample.  
I did not make tuning a separate CV, but go for it. It just did not work well in the place I was going to use it.

An idea for the mod (open CV pin) if you want is an accent or mute.

### Sample Preparation

Getting these samples is kind of a chore:  
1. Edit them to mono and clip them in Audacity.  
2. Use <https://www.online-convert.com/> to convert them to 8-bit, 8k sampling rate.  
   Somehow, Audacity does this, but it never worked for me. This site does it well.  
3. Use this website to create the sample code:  
   <https://synthworks.eu/attiny85-drum-creator/>

The ATTiny85 has only 8k to work with, so I have only five samples.

## References

<https://synthworks.eu/attiny85-drum-creator/>  
<https://modwiggler.com/forum/viewtopic.php?t=160118&start=150>

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0111/images/ACS-85-0111.png)
