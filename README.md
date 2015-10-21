# ArduinoComponentSketches
Collection of sketches for ATTiny to replace logic blocks in lunetta like circuits


Arduino Component Sketches
==============


## Objective:
Create a library of Arduino Sketches that can be loaded on ATTiny AVR microcontrollers to use in lo-fi synth/noise circuits.   Either replacing a block of logic or creating a new type of circuit block altogether.  The intent is not to create a synth on a chip, but to have a series of building blocks to create richer devices with an actual lower device count.  Rather than write the code to tailor the circuit, these are a prewritten library of sketches that can be used to plug into your breadboard or whatever.  Kinda like if you discovered a whole new series of CMOS chips like the 4000 series.

Of course, these are really arduino chips with the limitations that they have.  I am focusing on ATTiny85, 84 and 2313 for the most part.  Again, these are components, not full blown products.

The closest thing I have found is the Rad-fi System from Bleep Labs.

http://bleeplabs.com/store/the-rad-fi-system/

A collection of ATTiny micros with these sketches would be completely compatable with this system.


## Background:

[Background](https://github.com/robstave/ArduinoComponentSketches/wiki/Background)



## Requirements:
Ability to program ATTiny chips.  This only requires an arduino uno and a breadboard.
[Requirements](https://github.com/robstave/ArduinoComponentSketches/wiki/Requirements-and-Setup)

## System:
[Numbering System](https://github.com/robstave/ArduinoComponentSketches/wiki/Numbering-System)
 


### ATTiny85 sketches



Number    | Title   | Status   | Description  
--- | --- | --- | ---
[ACS-85-0001](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0001) | Oscillator | Ready| Fixed squarewave oscillator with High and Low Frequencies and a Ramp
[ACS-85-0002](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0002) | VCO | Ready| Two square wave vcos and a two tone out pin
[ACS-85-0003](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0003) | VCO | Ready| Two Tone VCO with speed
[ACS-85-0004](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004) | VCO | Ready| Two Tone VCO with Range (lfo to hf)
[ACS-85-0305](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0305) | Controller | Ready| Xor Gates (2)
[ACS-85-0306](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0306) | Gates | Ready| Nand Gates (2)
[ACS-85-0307](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0307) | Gates | Ready| Or/And gate
[ACS-85-0308](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0308) | Gates | Ready| Pick a Gate (and/or/nand/xor)
[ACS-85-0320](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0320) | Counter | Ready| Divide by 3,5,7,9
[ACS-85-0321](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0321) | Counter | Ready| Divide by 2,3,4,5
[ACS-85-0322](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0322) | Counter | Ready| Divide by N
[ACS-85-0500](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0500) | Component | Ready| Atari Punk Console
[ACS-85-0510](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0510) | Shift Register | Ready| Simple 16 bit Shift Register 
[ACS-85-0511](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0511) | Shift Register | Ready| Variable 32 bit Shift Register
[ACS-85-0512](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0512) | Shift Register | Ready| Large 128 bit Shift Register
[ACS-85-0520](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520) | Shift Register | Ready| Clocked 16 bit LFSR
[ACS-85-0521](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0521) | Shift Register | Ready| Variable speed 16 bit LFSR
[ACS-85-0521](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0522) | Shift Register | Ready| Variable low speed 16 bit LFSR
[ACS-85-0521](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0530) | Component | Ready| Drunk Walk
[ACS-85-0700](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0700) | Controller | Ready| Analog to Digital Decoder. Translate Analog to either 3 bits or single pins
  



### ATTiny84 sketches


Number    | Title   | Status   | Description  
--- | --- | --- | ---
[ACS-84-0001](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-84%20ATTiny84%20sketches/ACS-84-0001) | Oscillator | Ready | Fixed squarewave oscillator with High and Low Frequencies and a Ramp and Triangle
[ACS-84-0010](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-84%20ATTiny84%20sketches/ACS-84-0010) | Oscillator | Ready | Fixed squarewave oscillator with a grand assortment of beeps. 
[ACS-84-0305](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-84%20ATTiny84%20sketches/ACS-84-0305) | Gates | Ready |  Xor Array


