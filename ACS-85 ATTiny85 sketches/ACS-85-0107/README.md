# ACS-85-0107

Cowbell - 3Xor/other version

## Overview

The code xors three squarewaves to create a cowbell sound ( roland style). Its similar to ACS-85-0105 and ACS-85-0106. Its pretty similar...but worth A/Bing by ear.

You can, if you would like, disable the tuning and just stick with the classic frequencies or add a switch to toggle that. The code as is allows for tuning.

Its not going to be super. If you want to replace the lpf with a bandpass you can achieve a closer sound. Your going to have to google that though

<https://www.researchgate.net/figure/TR-808-cowbell-schematic-blocks-marked-adapted-from-1_fig1_267629988>

### Code

xor three waves. If its a 1, do the envelope, otherwise 0

```c
 // Xor

    if (a1 ^ a2 ^ a3) {
      OCR1A = amp;
    } else {
      OCR1A = 0;
    }
    return;

```

Sums up two aspects.  An Xor of two signals (a1, a2)
and an logical AND of two others ( a2, 3)
So you get a tone of sorts still.

```c
  byte sum = 0;
  if (a1 ^ a2) {
    sum = amp / 2;
  }

  if (a3 && a2)

  {
    sum = sum + amp / 2;
  }

  OCR1A = sum;
```

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0107/images/ACS-85-0107.png)

## Related

[ACS-85-0105 Cowbell](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0105/)

[ACS-85-0106 xor cowbell](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0106/)

[ACS-85-0107 3xor cowbell/other](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0107/)

[ACS-85-0108 cowbell-like things?](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0108/)
