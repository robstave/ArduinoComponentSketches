# ACS-85-0711

Button Select - Binary output

## Overview

Similar to ACS-85-0710 except that the output states are binary.

up    | down   | State |  notes
--- | --- | --- | ---
 0   |  0  | 0 0 0  |  init
 1  |  0  |  1 0 0  |  press up button
 0  |  0  |  1 0 0  |  release
 1  |  0  |  0 1 0  |  press up button
 0  |  0  |  0 1 0  |  release
 1  |  0  |  1 1 0  |  press up button
 0  |  0  |  1 1 0  |  release and so on
 n  |  0  |  ...  |  until it limits
 n  |  0  |  1 1 1  |  until it limits
 0  |  1  |  0 1 0   |  press down
 0  |  0  |  0 1 0   |  release
 0  |  1  |  1 0 0  |  press down

Wrap can be configured with the following line of code

```c
    boolean doWrap = LOW;
```

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0711/images/ACS-85-0711.png)

## Equivalent

## Examples
