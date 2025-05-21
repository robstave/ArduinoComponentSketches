# ACS-85-0616

Probability Gate with Fixed Length and Delay

## Overview

This sketch implements a probability-based gate with a fixed trigger length and optional delay. Here's how it works:

- On a rising edge at Pin 7, Pins 5 and 6 trigger for a fixed length with probabilities of 50% and 25%, respectively.
- If PB4 is high, the gate triggers after a delay (length remains the same). A3 determines the delay duration.
- If Pin 3 is low (not connected or tied down), the gate behaves like a regular probability gate. If high, the delay is added before the gate activates.

This setup is great for adding randomness and timing variations to your circuits.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0616/images/ACS-85-0616.png)

## Notes

![Cat](https://lh3.googleusercontent.com/pw/AP1GczNrxXCGrzwhsksV_zmo2kYkiovtFc7uNljgMNTNafme_zlj16Z8Sgpq1_JSBkHPmOvkyarz-uaH1Qjma065DdcSaLx0vXZNjCBxoLw0jQFDQ5ueUok=w2400)
