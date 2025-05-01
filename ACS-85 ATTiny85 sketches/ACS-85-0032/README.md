# ACS-85-0032

ATTiny85 Clocked LFO

## Overview

This is a clocked Low-Frequency Oscillator (LFO) that uses a clock signal to set the tempo. 

Despite its simplicity, it works pretty...ok. However, it lacks proper debouncing (actually, none at all), so you might want to add a low-pass filter (LPF) on the clock pin (e.g., 1kΩ/100nF or similar).

The sketch uses a counter that functions similarly to `millis()`. Instead of milliseconds, it leverages both Timer 0 and Timer 1, incrementing a counter to create a linear and usable timing mechanism.

Each clock trigger resets the wavetable index and calculates the delta, which determines the time until the next wavetable index.

### Waveforms

The LFO supports seven waveforms:

- Ramp Up
- Ramp Down
- Triangle
- Sine
- Square
- Random (4 notes per clock)
- Steps (random values alternating with 0s, e.g., `0, 33, 0, 29, 0, 143, 0, 200`)

There’s also a "Goose it" button that doubles or triples the frequency for a quick tempo boost.

Feel free to tweak and experiment!

## Pinout

![Pinout](https://github.com/robstave/ArduinoComponentSketches/blob/master/ACS-85%20ATTiny85%20sketches/ACS-85-0032/images/ACS-85-0032.png)

You’ll need a low-pass filter on the PWM pins for smoother output.

## Notes

Consider adding an LPF on the clock pin to handle debouncing for you.
