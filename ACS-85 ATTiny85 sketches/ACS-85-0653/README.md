# ACS-85-0653

Bytebeat (Bitbeat v2)

## Overview

Bytebeat refers to a self-generating sequence of bytes using a small equation.

This sketch is based on a snippet I found here:  
<https://www.youtube.com/watch?v=8HQUXlStqK8>  
<https://www.noizhardware.com/index.html>

For a great introduction to Bytebeat, check out this article:  
<http://nightmachines.tv/the-absolute-beginners-guide-to-coding-bytebeats.html>  
It covers the history and references key contributors like Viznut, who helped popularize Bytebeat in the early 2010s.

The original snippet can be found here:  
<https://pastebin.com/mfnKNWVD>

This sketch is similar to ACS-85-0650 but uses a different equation. It's important to listen and carefully pick bits, as the last bit in this case sounded awful.

## Issues

The sketch warbles a little, likely due to the equation short-circuiting its processing. For example:  
```c
x = (t > 127) & (evaluate stuff);
```
This will speed up and slow down as the first part of the equation is `0` half the time. The compiler is smart enough to branch out at that point since `0 & stuff = 0`. One possible improvement could be to process in a queue and pull from it periodically, rather than letting the loop drive the processing. However, this is more of an academic concern—it works well enough as is.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0653/images/acs-85-0653.png)

## Related Sketches

- [Bytebeat (Bitbeat v1)](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0650)  
- [Bytebeat (Bitbeat v2)](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0651)  
- [Bytebeat (Bitbeat v2) Clocked](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0652)  
- [Bytebeat (Bitbeat 2)](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0653)

## Other Links

- <https://medium.com/@naive_sound/glitch-a-noisy-fusion-of-math-and-music-6a9b24e7f5b5>  
- <https://noisio.de/>
