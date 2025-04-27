# ACS-85-0004 - Two Square Wave VCOs with Selectable Range (LF to HF)

## Overview

This sketch is a bit more practical and straightforward. It features two voltage-controlled oscillators (VCOs) with an analog pin to select from six frequency ranges: High Frequency 1, High Frequency 2, Medium Frequency 1, Medium Frequency 2, and Low Frequency 1 and 2.

| Range | Low Frequency | High Frequency |
|-------|---------------|----------------|
| HF1   | 250 Hz        | 4 kHz          |
| HF2   | 166 Hz        | 666 Hz         |
| MF1   | 62.5 Hz       | 222 Hz         |
| MF2   | 38.5 Hz       | 133 Hz         |
| LF1   | 1 Hz          | 16 Hz          |
| LF2   | 0.2 Hz        | 4 Hz           |

You can tweak these values, but keep in mind that the HF and MF counters use integers. For low frequencies, the code uses an additional loop. Changing the integer to a long didn’t work well, as it caused issues above 1.5 kHz. This is likely due to the extra overhead of handling longs in the interrupt handler on an 8-bit machine.

To keep things simple and efficient, the counter code for the LFO branch was duplicated rather than adding complexity (and clock cycles) with a shared handler. Nesting another loop turned out to be faster.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004/images/ACS-85-0004.png)

## Equivalent Circuit

![Equivalent Circuit](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004/images/ACS-85-0004-overview.png)

This setup is equivalent to two 40106 oscillators with selectable RC constants.

## Examples

## Improvements

Looking back, the range control could be simplified to directly set values in OCR1A. This would eliminate a loop and make the code much cleaner and more efficient.

