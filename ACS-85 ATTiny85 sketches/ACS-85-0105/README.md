# ACS-85-0105

Cowbell

## Overview

This sketch sums two square waves to create a cowbell sound (Roland-style).

You can disable the tuning and stick with the classic frequencies or add a switch to toggle that. The code allows for tuning.

It’s not going to be perfect. If you replace the low-pass filter (used to convert PWM to analog) with a bandpass filter, you might achieve a closer sound. You’ll need to Google that, though.

For reference:  
[TR-808 Cowbell Schematic](https://www.researchgate.net/figure/TR-808-cowbell-schematic-blocks-marked-adapted-from-1_fig1_267629988)

## Frequencies

From [TR-808 Cowbell Schematic](https://www.researchgate.net/figure/TR-808-cowbell-schematic-blocks-marked-adapted-from-1_fig1_267629988):

The cowbell circuit produces a sound with two tones, nominally at 540 Hz (C +55 cents) and 800 Hz (G5 +35 cents). These form an out-of-tune perfect fifth (6.80 semitones). The tuning of the tones is factory-set using two internal trimpots (TM1 and TM2).

The numbers in this sketch were tuned with a frequency counter. Since the ATTiny doesn’t use a crystal oscillator unless you add one, your results may vary. Consider tweaking the tuning to your liking with a frequency counter on a breadboard first.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0105/images/ACS-85-0105.png)

## Related

- [ACS-85-0105 Cowbell](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0105/)
- [ACS-85-0106 XOR Cowbell](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0106/)
- [ACS-85-0107 3XOR Cowbell/Other](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0107/)
- [ACS-85-0108 Cowbell-Like Things?](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0108/)

## Cat

![Cat](https://lh3.googleusercontent.com/pw/AP1GczM5xaGZF0dWRaGIyzrn856gDX2DSc9os8stYuQPmqHZednzTYtedj5F79Z7Utf399k3cQz1BDS7I__gGgPSWmUAmCZg88heY8dxVgOEy9E2WaBU4zo=w2400)
