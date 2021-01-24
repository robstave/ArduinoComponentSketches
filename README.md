# ArduinoComponentSketches

Collection of sketches for ATTiny that can be used to replace logic blocks in your circuits. Mostly, these are squarewave sketches so they can easily be used in lunetta-like circuits.

 
[Arduino Component Sketches](http://robstave.github.io/acs/)
 
 
## Objective

Create a library of Arduino sketches that can be loaded on ATTiny AVR micro controllers to use in lo-fi synth/noise circuits.

They can either replace a block of logic or create a new type of circuit altogether.  The intent is not to create a synth on a chip, but rather to have a series of building blocks to create richer devices with an actual lower device count.  Rather than write the code to tailor the circuit, these are a prewritten library of sketches that can be used to plug into your breadboard or whatever.  Kinda like if you discovered a whole new series of CMOS chips like the 4000 series.

Of course, these are really 8-bit microcontroller chips with limitations such as speed, quantization and code quirks.  

I am focusing on ATTiny85, 84 and 2313 for the most part.  Again, these are components, not full blown synth on a chip things.  But you can get a lot of sound from a minimal part count.

There are other plenty of projects out there with similar concepts. I draw inspiration from everything and hope that in some little way, someone can draw inspiration from my ramblings.

## Similar works

[MiniMoSynth](http://minimosynth.com/) A Formfactor and open source/open hardware design to make ATTiny Blocks.  Certainly the closest to what I have here.  I do not really have a
consistant strategy nor a consistant input like these little
blocks.  Check em out.

[Rad-fi System from Bleep Labs](http://bleeplabs.com/store/the-rad-fi-system/) Uses Arduinos as building blocks for Synths on a breadboard. A collection of ATTiny micros with these sketches would be completely compatible with this system.

[Bastl Kastle](http://www.bastl-instruments.com/instruments/kastle/kastle-synth/) uses 2 ATTiny85s that can be programmed with other features.

[Electric Druid](http://electricdruid.net/tap-tempo-lfo-taplfo-v2d/) has some AVRs and others as building blocks.  For example, an LFO.

## Background

[Background](https://github.com/robstave/ArduinoComponentSketches/wiki/Background)

## Requirements

Ability to program ATTiny chips.  This only requires an arduino uno, a breadboard and a handful of ATTiny85s to get started.

[Requirements](https://github.com/robstave/ArduinoComponentSketches/wiki/Requirements-and-Setup)

In some cases, a few extra parts might be needed like a small PWM filter or R2R ladder. These will be kept at a minimum unless the focus of the sketch IS an interface chip ( think optical encoder, Digital Pot controller and so on)

## System

I have a bit of numbering system so I can halfway keep these things straight.

[Numbering System](https://github.com/robstave/ArduinoComponentSketches/wiki/Numbering-System)

### ATTiny85 sketches
 

| Number  | Class | Description  |
| --- | --- | --- |
| **000-199 - Oscillators, Vco, LFO** |||
[ACS-85-0001](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0001) | Oscillator | Fixed squarewave oscillator with High and Low Frequencies and a Ramp
[ACS-85-0002](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0002) | VCO | Two square wave vcos and a two tone out pin
[ACS-85-0003](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0003) | VCO | Two Tone VCO with speed
[ACS-85-0004](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0004) | VCO | Two Freq VCO with Range (lfo to hf)
[ACS-85-0005](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0005) | VCO | Three Freq VCO detuned
[ACS-85-0006](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0006) | VCO | VCO with random period
[ACS-85-0007](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0007) | VCO | VCO with ramping period
[ACS-85-0008](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0008) | VCO | VCO with adjustable period/vibrato
[ACS-85-0010](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0010) | LFO PWM | Sine, tri, ramp up, ramp down
[ACS-85-0011](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0011) | LFO PWM | Random values (like psycho)
[ACS-85-0012](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0012) | LFO PWM | Random values 2 (like psycho)
[ACS-85-0015](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0015) | LFO | 2 Squarewave LFOs
[ACS-85-0020](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0020) | LFO | Adjustable Squarewave
[ACS-85-0030](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0030) | LFO | Four LFO with Pauses
[ACS-85-0040](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0040) | VCO | Squarewave VCO with Chiptune and PWM
[ACS-85-0041](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0041) | VCO | Squarewave VCO with PWM
[ACS-85-0042](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0042) | VCO | Squarewave VCO with Chiptune
[ACS-85-0050](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0050) | VCO PWM | Sinewave VCO PWM with distortion - in progress
[ACS-85-0060](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0060) | VCO  | Squarewave with LFO detuned frequency
[ACS-85-0061](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0061) | VCO  | Squarewave with LFO PWM frequency
[ACS-85-0100](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0100) | Sqaurewave | Debounced Sound generator
[ACS-85-0101](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0101) | Sqaurewave | Algoritmic Sound.  Blips and Bleeps.
| **200-299 	Envelope generators, VCA, Sample and Hold** ||
[ACS-85-0200](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0200) | VCA | Simple VCA - PWM - In progress
[ACS-85-0201](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0201) | VCA + AR | Simple VCA AR Envelope - PWM - In progress
[ACS-85-0210](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0210) | VCO with S&H | VCO with two tap delays
[ACS-85-0220](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0220) | Debounce gate | Debounced Gate with Repeat
| **300-499 	Modulation. Adders, Mixers, dividers, Multiplexers, Gates, Vibrato** ||
[ACS-85-0305](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0305) | Gates | Xor Gates (2)
[ACS-85-0306](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0306) | Gates | Nand Gates (2)
[ACS-85-0307](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0307) | Gates | Or/And gate
[ACS-85-0308](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0308) | Gates | Pick a Gate (and/or/nand/xor)
[ACS-85-0320](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0320) | Counter | Divide by 3,5,7,9
[ACS-85-0321](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0321) | Counter | Divide by 2,3,4,5
[ACS-85-0322](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0322) | Counter | Divide by N
[ACS-85-0350](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0350) | Modulation | Harmonizer
[ACS-85-0351](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0351) | Modulation | Detune
[ACS-85-0352](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0352) | Modulation | Vibrato (ramp)
[ACS-85-0353](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0353) | Modulation | Modulate LFSR
[ACS-85-0360](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0360) | Modulation | Selectable Counter-Divider
| **500-699 	Shift, Delay, Melody Makers, Atari Punk Console** ||
|  |  |  | 
[ACS-85-0500](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0500) | Component | Atari Punk Console
[ACS-85-0505](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0505) | Component | Nand synth
[ACS-85-0506](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0506) | Component | Nand DX 
[ACS-85-0510](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0510) | Shift Register | Simple 16 bit Shift Register 
[ACS-85-0511](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0511) | Shift Register | Variable 32 bit Shift Register
[ACS-85-0512](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0512) | Shift Register | Large 128 bit Shift Register
[ACS-85-0520](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0520) | Shift Register | Clocked 16 bit LFSR
[ACS-85-0521](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0521) | Shift Register | Variable speed 16 bit LFSR
[ACS-85-0522](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0522) | Shift Register | Variable low speed 16 bit LFSR
[ACS-85-0530](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0530) | Component | Drunk Walk
[ACS-85-0540](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0540) | Component | 4 Vco with LFO on/off
[ACS-85-0550](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0550) | Component | Melody Maker Upper 1
[ACS-85-0551](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0551) | Component | Melody Maker Upper 2
[ACS-85-0552](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0552) | Component | Melody Maker Upper 3
[ACS-85-0560](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0560) | Component | Freq sampler
[ACS-85-0570](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0570) |Shift Register | Analog Shift Register variable size
[ACS-85-0600](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0600) | LFO - Sequencer | Sequencer programmable LFO
[ACS-85-0601](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0601) | Sequencer | 1 bit 3 Pin Sequencer with analog scene select
[ACS-85-0603](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0603) | Sequencer | 2 Channel Drum Pattern Triggers
[ACS-85-0604](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0604) | Sequencer | 3 Channel Random Drum Triggers
[ACS-85-0605](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0605) | Sequencer | 1 bit waveform generator
[ACS-85-0607](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0607) | Sequencer | 4 Channel Random Drum Triggers
[ACS-85-0608](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0608) | Sequencer | 4 Channel Random Drum Triggers clocked
[ACS-85-0612](https://github.com/robstave/ArduinoComponentSketches/tree/master/
ACS-85%20ATTiny85%20sketches/ACS-85-0612) | Random | Clocked Coin Flip trigged-latch
[ACS-85-0613](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0613) | Random | Clocked Coin Flip gate
[ACS-85-0614](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0614) | Random | Clocked mulitple Coin Flip gate
| **700-899 	Interface, Control, Midi, Visualization** ||
[ACS-85-0700](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0700) | Controller | Analog to Digital Decoder. Translate Analog to either 3 bits or single pins
[ACS-85-0705](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0705) | Controller | Two pin Tap Tempo
[ACS-85-0710](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0710) | Controller | Button Select 3 pin
[ACS-85-0711](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-85%20ATTiny85%20sketches/ACS-85-0711) | Controller | Button Select 3 pin binary
| **900-999 	Other** ||


### ATTiny84 sketches


| Number    | Type   | Description  |
| --- | --- | --- |
| **000-199 - Oscillators, Vco, LFO** |||
[ACS-84-0001](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-84%20ATTiny84%20sketches/ACS-84-0001) | Oscillator | Fixed square wave oscillator with High and Low Frequencies and a Ramp and Triangle
[ACS-84-0010](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-84%20ATTiny84%20sketches/ACS-84-0010) | Oscillator | Fixed square wave oscillator with a grand assortment of beeps. 
| **300-499 	Modulation. Adders, Mixers, dividers, Multiplexers, Gates, Vibrato** |||
[ACS-84-0305](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-84%20ATTiny84%20sketches/ACS-84-0305) | Gates |  Xor Array
[ACS-84-0570](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-84%20ATTiny84%20sketches/ACS-84-0570) |Shift Register | Analog Shift Register 4 outputs
[ACS-84-0608](https://github.com/robstave/ArduinoComponentSketches/tree/master/ACS-84%20ATTiny84%20sketches/ACS-84-0608) | Sequncer | clocked 4 Channel random triggers


