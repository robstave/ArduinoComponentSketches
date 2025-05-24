# ACS-85-0321

Divide by 2, 3, 4, 5

## Overview

This sketch is essentially a counter. When used with audio, dividing by 2 gives you an octave lower.

The division values are hardcoded but can be customized in the software. You can set values other than 2-5 using the following defines:

```C
    #define DIVA 2
    #define DIVB 3
    #define DIVC 4
    #define DIVD 5
```

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0321/images/acs-85-0321.png)

## Equivalent

This is similar to [Ken Stone's divider](http://www.cgs.synth.net/modules/subosc.html).

However, this is a great example of where a microcontroller shines, potentially reducing your chip count depending on your needs.

If you're working with very high frequencies, a dedicated CMOS counter is the better choice. But for breadboard experiments, this approach is hard to beat. Dividing by odd numbers, in particular, can be tricky to implement with discrete circuitry.
