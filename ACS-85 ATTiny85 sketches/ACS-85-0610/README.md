# ACS-85-0610

Quadrature Counter/sequncer

## Overview

 Takes and input clock and outputs quadrature phase

 0: xx__xx__
 1: _xx__xx_
 2: __xx__xx
 3: x__xx__x

 Note, each pins is actually independent of each other, so you could do differnt patterns and even polyrhythms

 ```c
  #define PATTERN_0_SIZE 8
  boolean pattern_0[PATTERN_0_SIZE] = { HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW };

  #define PATTERN_1_SIZE 9
  boolean pattern_1[PATTERN_0_SIZE] = { HIGH, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW };
 
  #define PATTERN_2_SIZE 11
  boolean pattern_2[PATTERN_0_SIZE] = { LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, HIGH, HIGH };
 
  #define PATTERN_3_SIZE 3
  boolean pattern_3[PATTERN_0_SIZE] = { HIGH, LOW, LOW};
 ```

 ![output](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0610/images/610.png)


## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0610/images/acs-85-0610.png)

 
