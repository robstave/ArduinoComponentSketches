# ACS-85-0650

Bytebeat ( Bitbeat v2)
 
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

Based on this snippet https://pastebin.com/mfnKNWVD

This is really not too different in style with ACS-0650.
I Used another equation and found that you REALLY should listen and pick bits.
The last bit in this case sounded awful.

Also, it warbles a little.  I think that is the equation short circuiting it processing.
For example

   x = ( t> 127) & ( evaluate stuff )

Will speed up and slow down as the first part of the equation is 0 half the time.  The
compiler is smart enough to brach out at that point as there is no need to finish the
equation  0 & stuff = zero.



   


 

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0651/images/acs-85-0651.png)

## Equivalent

 
 

## Examples:
 