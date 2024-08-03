# ACS-85-0105

Cowbell

## Overview

The code sums two squarewaves to create a cowbell sound ( roland style).

You can, if you would like, disable the tuning and just stick with the classic frequencies or add a switch to toggle that.  The code allows for tuning.

Its not going to be super.  If you want to replace the lpf (that you need to convert pwm to analog )  with a bandpass you might achieve a closer sound.  Your going to have to google that though

<https://www.researchgate.net/figure/TR-808-cowbell-schematic-blocks-marked-adapted-from-1_fig1_267629988>

## Frequencies

From <https://www.researchgate.net/figure/TR-808-cowbell-schematic-blocks-marked-adapted-from-1_fig1_267629988>

The cowbell circuit produces a sound with two tones, nominally at 540 Hz ( C +55 cents) and 800 Hz ( G 5 +35 cents). These form an out-of-tune perfect 5th (6.80 semitones). The tuning of the tones is factory-set using two internal trimpots (TM 1 and TM 2 ).

The numbers that this sketch were tuned with a frequency counter.  ATTINY does not use a xtal unless you add one...so your results may vary.  Consider just tweaking to your liking with a frequency counter on a breadboard first.
 

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0105/images/ACS-85-0105.png)

## Related

[ACS-85-0105 Cowbell](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0105/)

[ACS-85-0106 xor cowbell](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0106/)

[ACS-85-0107 3xor cowbell/other](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0107/)

[ACS-85-0108 cowbell-like things?](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0108/)




## Cat

![Pinout](https://lh3.googleusercontent.com/pw/AP1GczM5xaGZF0dWRaGIyzrn856gDX2DSc9os8stYuQPmqHZednzTYtedj5F79Z7Utf399k3cQz1BDS7I__gGgPSWmUAmCZg88heY8dxVgOEy9E2WaBU4zo=w2400)
