# ACS-85-0065

XOR-Based Phased Drone/VCO

## Overview

This sketch generates a phased drone sound using two pins. The result is a "phasey" sound that’s quite distinct.

The implementation is based on DDS and uses six accumulators:
- Three accumulators are XORed for **PB0**.
- The other three accumulators are XORed for **PB1**.

The three frequencies create a "beat" that the XOR operation highlights. Interestingly, it doesn’t sound like a ring modulator but has its own unique character.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0065/images/ACS-85-0065.png)

 
