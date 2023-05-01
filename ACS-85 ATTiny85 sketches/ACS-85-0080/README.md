# ACS-85-0080

Noise on 2 outputs


## Overview

There are two noise outputs based on a LFSR ( Linear Feedback Shift Register)

Can use for a noise for drums or whatever.  

There are two pins that are basically gates too...so you can use that for rhythmic things if you want.
 
## Notes

The Pitch will effect the general sound.  All it does is loop though a counter and ticks when done.  The limits are :

```

#define CHANNEL1_HIGH 50
#define CHANNEL1_LOW 180

#define CHANNEL2_HIGH 90
#define CHANNEL2_LOW 250

```

So if you want close sounding pins pick [{50,180}, {55,200}] maybe?

you can even make one sweep in the other direction by flipping one of the sets.  Play with the numbers!



## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0080/images/ACS-85-0080.png) 


## ---

![cat](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0080/images/catnoise.jpg)
