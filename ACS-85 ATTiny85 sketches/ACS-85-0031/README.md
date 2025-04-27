# ACS-85-0031 - ATTiny85 3-Phase Ramp LFO Clocked

## Overview

This sketch generates three phase-shifted ramp waveforms using PWM on the ATTiny85. It’s a great starting point for drone or Lunetta-style projects. However, there are some limitations to note:

- Timer0 provides higher resolution PWM for PB0 and PB1 (fast PWM at ~30kHz).
- Timer1 provides lower resolution PWM for PB4 (slower PWM at ~500Hz).

While this setup works, the difference in resolution between Timer0 and Timer1 may affect your application. For visual feedback (e.g., LEDs), it’s fine, but for audio or precise control, you might want to consider alternatives.

### Suggestions for Improvement

- Use only PB0 and PB1 for high-resolution PWM.
- Experiment with different phase relationships, such as half-speed, double-speed, or reverse.
- Implement a wavetable approach with a single counter. The counter clocked in the interrupt doesn’t need to be directly tied to the analog output.
- Utilize the spare pin for a sync feature.

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0031/images/ACS-85-0031.png)

### Notes

You’ll need a low-pass filter on the PWM pins to smooth the output.

## Additional Resources

- [Arduino Forum Discussion on PWM Frequency](https://forum.arduino.cc/t/pwm-frequency-change-attiny85/170321)
- [ATTiny85 Fast PWM on Timer1](https://forum.arduino.cc/t/attiny85-fast-pwm-on-timer-1/993958/14)

## Bonus: Cat

![Cat](https://lh3.googleusercontent.com/pw/AP1GczOC1K8SlLH3o_JWJbeBLbpNGRXPGJOU2n_DlStPM9GIqVDu2-h75HmkeozqkKQSol8QVhl8gmYhv1EUyc8cFQQXYUesFKexASVdpav7LZkyZ5N6aDQ=w2400)
