# ACS-85-0106

Cowbell - XOR Version

## Overview

This sketch sums two square waves to create a cowbell sound (Roland-style). It’s the same as ACS-85-0105, except the two values are XOR’d. It’s pretty similar but worth A/B testing by ear.

You can disable the tuning and stick with the classic frequencies or add a switch to toggle that. The code as-is allows for tuning.

It’s not going to be perfect. If you replace the low-pass filter with a bandpass filter, you can achieve a closer sound. You’ll need to Google that, though.

For reference:  
[TR-808 Cowbell Schematic](https://www.researchgate.net/figure/TR-808-cowbell-schematic-blocks-marked-adapted-from-1_fig1_267629988)

## Output

The output is a square wave XOR’d for a unique sound.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0106/images/ACS-85-0106.png)

## Related

- [ACS-85-0105 Cowbell](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0105/)
- [ACS-85-0106 XOR Cowbell](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0106/)
- [ACS-85-0107 3XOR Cowbell/Other](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0107/)
- [ACS-85-0108 Cowbell-Like Things?](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0108/)

## Cat

![Cat](https://lh3.googleusercontent.com/pw/AP1GczM2G_Fnkfeos2xG2Xi9dLvd8-G1MysF5wGvRiIFktZKmhFWpcXGKXVTZzF4j_VgXfAcsAQezjJjJ-Pql0j-vmwquyjzuoRP7IzeKY1Lu2dFlNeqdLA=w2400)
