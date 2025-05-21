# ACS-85-0711

Button Select - Binary Output

## Overview

Similar to ACS-85-0710, except that the output states are binary.

| Up | Down | State  | Notes                     |
|----|------|--------|---------------------------|
| 0  | 0    | 0 0 0  | Init                      |
| 1  | 0    | 1 0 0  | Press up button           |
| 0  | 0    | 1 0 0  | Release                   |
| 1  | 0    | 0 1 0  | Press up button           |
| 0  | 0    | 0 1 0  | Release                   |
| 1  | 0    | 1 1 0  | Press up button           |
| 0  | 0    | 1 1 0  | Release and so on         |
| n  | 0    | ...    | Until it limits           |
| n  | 0    | 1 1 1  | Until it limits           |
| 0  | 1    | 0 1 0  | Press down                |
| 0  | 0    | 0 1 0  | Release                   |
| 0  | 1    | 1 0 0  | Press down                |

Wrap can be configured with the following line of code:

```c
boolean doWrap = LOW;
```

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0711/images/ACS-85-0711.png)

## Equivalent

## Examples
