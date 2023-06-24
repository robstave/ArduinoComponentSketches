/**
   ACS-85-0110
   ATTiny85  High Hat Sample player
   
   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0  Sample Select
   External pin 3 (PB4) = input 1  not used
   External pin 4       = GND
   External pin 5 (PB0) = Trigger
   External pin 6 (PB1) = PWM
   External pin 7 (PB2) = Clock
   External pin 8       = Vcc
   V 1.0  -  First Version
   
   This is mostly Jan Ostmans Sample code dsp-Host Drum8. 
   In this case, all the samples are pretty
   short.

   It is set up for 8 samples, but does not use up all the slots.

   Sample select also tunes the sample.  There is no CV for that.
   ...but you could do that if you want.  Where the sample is selected
   is where things get different with this take on the code.

   Rob Stave (Rob the fiddler) 2023 GNU and Jan Ostman 2016 GNU
*/

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//(pin3) sample sel A3  PB3 2|    |7  PB2 (pin2) Clock
//      (pin4) none A2  PB4 3|    |6  PB1 (pin1) PWM
//                      GND 4|    |5  PB0 (pin0) trigger
//        


// Copyright 2016 DSP Synthesizers Sweden.
//
// Author: Jan Ostman
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.


//  So, getting these samples are kinda a chore.
//  I got some samples from music radar.
//  I edited them to mono and clipped in Audacity.
//  Then I used https://www.online-convert.com/ to convert it to 8 bit 8k sampling rate.
//  Somehow, audacity does this, but It never worked for me. This site does it well.
//
//  Then I used this website to create the sample code.
//
//  https://synthworks.eu/attiny85-drum-creator/
// Attiny85 has only 8k to work with.  So I have only 5 samples



#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


//--------- Ringbuf parameters ----------
uint8_t Ringbuffer[256];
uint8_t RingWrite = 0;
uint8_t RingRead = 0;
volatile uint8_t RingCount = 0;
//-----------------------------------------


const uint8_t drum1[] PROGMEM = {
 0x49, 0x4E, 0x46, 0x4F, 
0x49, 0x53, 0x46, 0x54, 0x0D, 0x00, 0x00, 0x00, 0x4C, 0x61, 0x76, 0x66, 0x36, 0x30, 0x2E, 0x35, 
0x2E, 0x31, 0x30, 0x30, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61, 0xDC, 0x05, 0x00, 0x00, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x7F, 0x81, 0x7E, 0x83, 0x7E, 0x84, 0x2B, 0xA0, 0x81, 0x4C, 0xA7, 0x50, 
0xA7, 0x7D, 0x56, 0xC4, 0x42, 0x91, 0x96, 0x3D, 0xC9, 0x4D, 0x83, 0x96, 0x60, 0x8B, 0x81, 0x56, 
0x93, 0x99, 0x44, 0xB4, 0x5E, 0x84, 0x78, 0x64, 0x9A, 0x47, 0xB8, 0x42, 0x6F, 0x9F, 0x44, 0x9D, 
0x5C, 0x93, 0x7E, 0x5C, 0x9D, 0x7A, 0x5D, 0xB7, 0x5F, 0x70, 0xB4, 0x47, 0x93, 0x66, 0x82, 0x90, 
0x55, 0xA8, 0x71, 0x7B, 0x7C, 0x8D, 0x69, 0x46, 0xBA, 0x54, 0x8A, 0x72, 0x7E, 0x8D, 0x60, 0x8E, 
0x6B, 0x6E, 0xB1, 0x5E, 0x72, 0x75, 0x80, 0x9C, 0x4F, 0xAE, 0x55, 0x8F, 0x80, 0x65, 0x84, 0x70, 
0xB0, 0x39, 0x9F, 0x8E, 0x44, 0xB6, 0x5C, 0x85, 0x7F, 0x64, 0x8C, 0x64, 0xAC, 0x68, 0xA3, 0x43, 
0x7E, 0xA0, 0x4C, 0x9E, 0x65, 0x86, 0x75, 0x6A, 0x8F, 0x60, 0x70, 0xA5, 0x4A, 0x91, 0x62, 0x82, 
0x7B, 0x61, 0xB6, 0x46, 0x9E, 0x75, 0x6E, 0xA8, 0x5E, 0xAA, 0x69, 0x66, 0x9B, 0x52, 0x90, 0x9B, 
0x5C, 0x8B, 0x88, 0x57, 0x90, 0x5A, 0x9B, 0x55, 0x79, 0x80, 0x64, 0x91, 0x5B, 0x97, 0x4D, 0x8B, 
0x8A, 0x58, 0x9A, 0x64, 0x9B, 0x5B, 0x87, 0x81, 0x76, 0x8F, 0x69, 0xAF, 0x33, 0x9F, 0x74, 0x72, 
0x95, 0x74, 0x95, 0x3A, 0xBB, 0x66, 0x83, 0x6D, 0xA0, 0x8F, 0x3E, 0xB3, 0x7D, 0x62, 0x9B, 0x94, 
0x59, 0xB8, 0x61, 0x85, 0x83, 0x6A, 0xAF, 0x5E, 0x80, 0x91, 0x68, 0x8A, 0xA0, 0x6A, 0x73, 0x62, 
0x8A, 0xA2, 0x4C, 0xAD, 0x7E, 0x5D, 0xA8, 0x78, 0x74, 0x9E, 0x89, 0x6A, 0x93, 0x5F, 0x90, 0x86, 
0x64, 0xA9, 0x62, 0x7D, 0x9A, 0x68, 0x7D, 0xA8, 0x68, 0x89, 0x80, 0x63, 0xB2, 0x72, 0x78, 0xAF, 
0x52, 0x8F, 0x9D, 0x69, 0x7D, 0x88, 0x70, 0x78, 0x64, 0x8D, 0x62, 0x89, 0x8E, 0x5B, 0x85, 0x6A, 
0x95, 0x5E, 0x78, 0x7F, 0x85, 0x84, 0x60, 0xAE, 0x64, 0x82, 0x95, 0x57, 0x96, 0x89, 0x64, 0x8D, 
0x91, 0x64, 0x97, 0x6C, 0x79, 0xAC, 0x5C, 0x9B, 0x8F, 0x55, 0xA5, 0x8C, 0x5F, 0x97, 0xA0, 0x4D, 
0xA7, 0x8B, 0x59, 0xB5, 0x7E, 0x7C, 0x93, 0x72, 0x77, 0xAB, 0x64, 0x88, 0xA3, 0x53, 0x8B, 0xA5, 
0x5E, 0x75, 0xAE, 0x4D, 0x93, 0x90, 0x52, 0xAC, 0x72, 0x7E, 0x97, 0x64, 0x88, 0xAB, 0x54, 0x9A, 
0xA2, 0x46, 0xAC, 0x94, 0x5F, 0x9B, 0xAA, 0x64, 0x85, 0x74, 0x6C, 0xA2, 0x64, 0x8A, 0x87, 0x58, 
0x96, 0x8B, 0x67, 0x93, 0x8C, 0x65, 0x94, 0x61, 0x91, 0x89, 0x6A, 0xA0, 0x61, 0x7C, 0x99, 0x7A, 
0x6D, 0xA0, 0x67, 0x78, 0x9D, 0x54, 0x9E, 0x89, 0x6A, 0xA5, 0x6C, 0x70, 0xA5, 0x77, 0x66, 0xB2, 
0x60, 0x74, 0x9E, 0x66, 0x7F, 0xA2, 0x71, 0x74, 0x92, 0x5C, 0x8C, 0x9E, 0x6A, 0x8B, 0x87, 0x74, 
0x73, 0x8F, 0x55, 0x91, 0x7D, 0x59, 0x9C, 0x78, 0x6C, 0x9A, 0x87, 0x6E, 0x79, 0x61, 0x85, 0x8F, 
0x58, 0x9B, 0x73, 0x5C, 0x9E, 0x83, 0x67, 0x90, 0x8B, 0x58, 0xA4, 0x74, 0x68, 0xA4, 0x6C, 0x86, 
0x7A, 0x6C, 0x99, 0x7E, 0x63, 0xA6, 0x6A, 0x75, 0x9D, 0x5C, 0x90, 0x89, 0x72, 0x90, 0x5F, 0x85, 
0x98, 0x5F, 0x87, 0xA0, 0x52, 0x92, 0x8E, 0x65, 0x8C, 0x9F, 0x68, 0x81, 0x93, 0x5E, 0x8C, 0x8D, 
0x78, 0x8C, 0x82, 0x76, 0x78, 0xA4, 0x77, 0x75, 0x99, 0x7F, 0x67, 0x8D, 0x8C, 0x55, 0xAE, 0x87, 
0x5B, 0x90, 0x8F, 0x5F, 0x8F, 0xAE, 0x59, 0x7F, 0x94, 0x80, 0x60, 0xAE, 0x8A, 0x52, 0xA2, 0x95, 
0x66, 0x83, 0xA4, 0x5F, 0x80, 0x9B, 0x59, 0x9B, 0x87, 0x73, 0x8D, 0x6C, 0x7D, 0x93, 0x67, 0x7E, 
0x96, 0x5C, 0x90, 0x81, 0x65, 0x9A, 0x7B, 0x7E, 0x76, 0x72, 0x90, 0x7E, 0x65, 0x9E, 0x72, 0x6A, 
0x9A, 0x67, 0x7D, 0x96, 0x76, 0x84, 0x79, 0x74, 0x89, 0x9E, 0x6F, 0x83, 0x92, 0x83, 0x6A, 0x92, 
0x98, 0x69, 0x89, 0x8F, 0x84, 0x6B, 0x91, 0x95, 0x72, 0x86, 0x91, 0x7E, 0x69, 0x98, 0x98, 0x75, 
0x7F, 0x8C, 0x73, 0x78, 0x9A, 0x77, 0x83, 0x91, 0x82, 0x75, 0x8B, 0x91, 0x6C, 0x98, 0x8F, 0x6C, 
0x80, 0xA3, 0x7F, 0x6D, 0xA2, 0x87, 0x70, 0x80, 0x9F, 0x6B, 0x86, 0x83, 0x61, 0x85, 0x85, 0x69, 
0x82, 0x88, 0x6E, 0x7E, 0x6D, 0x87, 0x80, 0x71, 0x90, 0x6C, 0x75, 0x8E, 0x79, 0x72, 0x91, 0x75, 
0x77, 0x7C, 0x6E, 0x9C, 0x8D, 0x79, 0x82, 0x8A, 0x7C, 0x6F, 0x94, 0x87, 0x79, 0x81, 0x89, 0x72, 
0x76, 0x9F, 0x8D, 0x79, 0x75, 0x9F, 0x7F, 0x68, 0x95, 0x9D, 0x80, 0x63, 0x9D, 0x93, 0x65, 0x88, 
0xAE, 0x7A, 0x60, 0x9E, 0x8F, 0x6F, 0x76, 0xAE, 0x7B, 0x65, 0x97, 0x98, 0x77, 0x6D, 0xA7, 0x71, 
0x70, 0x96, 0x99, 0x69, 0x80, 0xA9, 0x60, 0x7D, 0x98, 0x85, 0x6B, 0x93, 0x92, 0x60, 0x85, 0x8E, 
0x76, 0x7A, 0x94, 0x71, 0x7C, 0x78, 0x77, 0x8D, 0x6F, 0x8A, 0x7A, 0x6C, 0x8A, 0x84, 0x6C, 0x94, 
0xA0, 0x60, 0x82, 0xA0, 0x83, 0x6A, 0x95, 0xA5, 0x67, 0x79, 0x8E, 0x9D, 0x72, 0x78, 0x96, 0x89, 
0x7E, 0x70, 0x95, 0x8B, 0x81, 0x7A, 0x95, 0x89, 0x6B, 0x87, 0x99, 0x95, 0x64, 0x80, 0x97, 0x77, 
0x6D, 0x9E, 0x93, 0x5D, 0x8B, 0x9B, 0x82, 0x6D, 0x99, 0x9B, 0x65, 0x81, 0x99, 0x90, 0x6E, 0x88, 
0x9E, 0x7B, 0x77, 0x85, 0x86, 0x74, 0x89, 0x79, 0x6D, 0x8A, 0x85, 0x6F, 0x84, 0x83, 0x6C, 0x8B, 
0x71, 0x7D, 0x84, 0x77, 0x89, 0x68, 0x7C, 0x89, 0x75, 0x76, 0x8F, 0x72, 0x7B, 0x7F, 0x71, 0x96, 
0x8B, 0x81, 0x75, 0x90, 0x87, 0x6D, 0x84, 0x97, 0x8F, 0x66, 0x85, 0x9A, 0x83, 0x6E, 0x9A, 0xA1, 
0x69, 0x77, 0x8F, 0x96, 0x81, 0x76, 0x8C, 0x8D, 0x8A, 0x6C, 0x88, 0x93, 0x8C, 0x6F, 0x78, 0x9D, 
0x85, 0x6D, 0x83, 0xA4, 0x78, 0x6E, 0x87, 0x93, 0x8F, 0x73, 0x84, 0x88, 0x8F, 0x74, 0x7E, 0x94, 
0x93, 0x79, 0x71, 0x9F, 0x88, 0x72, 0x83, 0x8F, 0x71, 0x7D, 0x88, 0x6E, 0x88, 0x7B, 0x7E, 0x83, 
0x6A, 0x82, 0x88, 0x6E, 0x83, 0x87, 0x6B, 0x89, 0x78, 0x77, 0x88, 0x77, 0x89, 0x87, 0x7A, 0x7B, 
0x8C, 0x8A, 0x7E, 0x80, 0x8B, 0x8B, 0x72, 0x88, 0x8F, 0x8D, 0x71, 0x7D, 0x98, 0x80, 0x74, 0x8D, 
0x9C, 0x70, 0x71, 0x91, 0x93, 0x81, 0x7A, 0x89, 0x84, 0x84, 0x75, 0x82, 0x92, 0x8B, 0x77, 0x77, 
0x9C, 0x86, 0x70, 0x85, 0x9A, 0x7F, 0x6F, 0x8A, 0x8F, 0x79, 0x75, 0x98, 0x85, 0x71, 0x82, 0x8D, 
0x8B, 0x75, 0x86, 0x88, 0x84, 0x7A, 0x7E, 0x91, 0x87, 0x7A, 0x7C, 0x7D, 0x7A, 0x89, 0x88, 0x7D, 
0x82, 0x85, 0x81, 0x76, 0x89, 0x8D, 0x80, 0x7B, 0x7C, 0x7A, 0x81, 0x8D, 0x80, 0x81, 0x84, 0x88, 
0x7A, 0x7E, 0x8D, 0x8D, 0x7E, 0x75, 0x93, 0x8A, 0x6F, 0x81, 0x9A, 0x8A, 0x6F, 0x7D, 0x8B, 0x90, 
0x80, 0x7D, 0x82, 0x88, 0x83, 0x74, 0x88, 0x8F, 0x8F, 0x73, 0x7A, 0x8E, 0x8F, 0x78, 0x77, 0x8F, 
0x86, 0x86, 0x78, 0x7C, 0x8E, 0x86, 0x7A, 0x79, 0x90, 0x7E, 0x72, 0x8A, 0x91, 0x77, 0x75, 0x8C, 
0x8A, 0x79, 0x7B, 0x98, 0x7F, 0x73, 0x86, 0x8C, 0x82, 0x79, 0x8B, 0x87, 0x79, 0x7E, 0x8A, 0x85, 
0x7C, 0x86, 0x81, 0x81, 0x7C, 0x83, 0x89, 0x80, 0x7D, 0x85, 0x83, 0x78, 0x87, 0x88, 0x85, 0x77, 
0x81, 0x8D, 0x76, 0x7B, 0x8D, 0x89, 0x72, 0x7D, 0x8B, 0x84, 0x75, 0x83, 0x95, 0x80, 0x7B, 0x7C, 
0x83, 0x8C, 0x8F, 0x79, 0x73, 0x88, 0x8E, 0x80, 0x74, 0x88, 0x8B, 0x80, 0x7E, 0x7D, 0x83, 0x91, 
0x89, 0x73, 0x7E, 0x89, 0x7E, 0x76, 0x8A, 0x8A, 0x72, 0x7E, 0x8B, 0x83, 0x75, 0x85, 0x8D, 0x77, 
0x7A, 0x84, 0x8A, 0x7C, 0x7E, 0x86, 0x81, 0x81, 0x7C, 0x85, 0x88, 0x80, 0x7B, 0x85, 0x80, 0x78, 
0x85, 0x86, 0x82, 0x77, 0x86, 0x88, 0x76, 0x80, 0x8E, 0x81, 0x76, 0x84, 0x89, 0x7B, 0x7C, 0x8F, 
0x82, 0x73, 0x85, 0x89, 0x7E, 0x7B, 0x87, 0x8C, 0x81, 0x7C, 0x7E, 0x83, 0x8A, 0x83, 0x75, 0x83, 
0x8C, 0x82, 0x79, 0x82, 0x8F, 0x82, 0x7C, 0x7E, 0x81, 0x89, 0x8A, 0x7B, 0x78, 0x8A, 0x88, 0x7D, 
0x7B, 0x88, 0x8B, 0x7E, 0x7D, 0x81, 0x85, 0x82, 0x80, 0x7F, 0x87, 0x87, 0x7D, 0x7D, 0x85, 0x89, 
0x78, 0x7D, 0x87, 0x86, 0x79, 0x81, 0x8B, 0x7C, 0x7C, 0x82, 0x87, 0x80, 0x7E, 0x85, 0x80, 0x7F, 
0x7F, 0x84, 0x82, 0x7F, 0x81, 0x81, 0x7F, 0x7D, 0x84, 0x85, 0x7F, 0x7E, 0x85, 0x80, 0x7C, 0x83, 
0x89, 0x87, 0x7A, 0x7E, 0x87, 0x84, 0x7B, 0x82, 0x87, 0x83, 0x82, 0x7C, 0x7F, 0x88, 0x89, 0x7F, 
0x7A, 0x84, 0x89, 0x84, 0x7D, 0x83, 0x82, 0x87, 0x82, 0x7D, 0x7F, 0x86, 0x8A, 0x7D, 0x7C, 0x82, 
0x87, 0x82, 0x80, 0x80, 0x82, 0x88, 0x80, 0x7D, 0x80, 0x8A, 0x85, 0x7B, 0x80, 0x82, 0x85, 0x7D, 
0x80, 0x83, 0x80, 0x7F, 0x7E, 0x83, 0x84, 0x7F, 0x7D, 0x84, 0x81, 0x7C, 0x81, 0x86, 0x81, 0x7B, 
0x83, 0x83, 0x7D, 0x80, 0x88, 0x7F, 0x7C, 0x82, 0x84, 0x7E, 0x7E, 0x89, 0x7F, 0x7B, 0x84, 0x84, 
0x7E, 0x7F, 0x84, 0x85, 0x80, 0x7F, 0x7F, 0x82, 0x87, 0x83, 0x7A, 0x80, 0x86, 0x85, 0x7F, 0x81, 
0x82, 0x82, 0x84, 0x7F, 0x7F, 0x81, 0x87, 0x81, 0x7C, 0x81, 0x85, 0x82, 0x80, 0x81, 0x7F, 0x83, 
0x85, 0x7F, 0x7E, 0x82, 0x87, 0x80, 0x7F, 0x80, 0x82, 0x84, 0x83, 0x7F, 0x7E, 0x84, 0x82, 0x7D, 
0x81, 0x85, 0x80, 0x7C, 0x82, 0x83, 0x80, 0x7E, 0x84, 0x82, 0x7D, 0x80, 0x83, 0x81, 0x7F, 0x82, 
0x82, 0x7F, 0x7F, 0x82, 0x81, 0x80, 0x82, 0x80, 0x80, 0x80, 0x80, 0x82, 0x80, 0x80, 0x81, 0x84, 
0x81, 0x7F, 0x80, 0x85, 0x83, 0x7F, 0x80, 0x81, 0x83, 0x83, 0x82, 0x7E, 0x80, 0x84, 0x81, 0x7F, 
0x80, 0x83, 0x83, 0x80, 0x80, 0x7F, 0x82, 0x84, 0x82, 0x7E, 0x80, 0x83, 0x82, 0x81, 0x81, 0x80, 
0x81, 0x81, 0x7F, 0x80, 0x82, 0x83, 0x7F, 0x7F, 0x82, 0x83, 0x80, 0x7F, 0x83, 0x81, 0x80, 0x80, 
0x81, 0x82, 0x82, 0x80, 0x7F, 0x82, 0x82, 0x7F, 0x80, 0x83, 0x81, 0x7F, 0x81, 0x82, 0x80, 0x80, 
0x82, 0x81, 0x7F, 0x81, 0x82, 0x80, 0x80, 0x82, 0x81, 0x80, 0x81, 0x82, 0x81, 0x80, 0x81, 0x81, 
0x81, 0x80, 0x80, 0x81, 0x82, 0x81, 0x80, 0x80, 0x82, 0x82, 0x81, 0x81, 0x80, 0x81, 0x82, 0x81, 
0x80, 0x80, 0x81, 0x81, 0x81, 0x81, 0x80, 0x81, 0x82, 0x81, 0x80, 0x80, 0x81, 0x81, 0x81, 0x80, 
0x81, 0x81, 0x81, 0x80, 0x80, 0x81, 0x81, 0x81, 0x80, 0x80, 0x81, 0x81, 0x81, 0x81, 0x80, 0x81, 
0x81, 0x80, 0x80, 0x81, 0x81, 0x80, 0x80, 0x81, 0x81, 0x80, 0x81, 0x81, 0x80, 0x80, 0x81, 0x81, 
0x80, 0x81, 0x81, 0x80, 0x80, 0x81, 0x81, 0x80, 0x81, 0x81, 0x80, 0x80, 0x81, 0x81, 0x80, 0x81, 
0x80, 0x81, 0x80, 0x80, 0x80, 0x81, 0x81, 0x80, 0x80, 0x81, 0x81, 0x81, 0x80, 0x80, 0x80, 0x81, 
0x81, 0x80, 0x80, 0x80, 0x81, 0x80, 0x80, 0x80, 0x80, 0x81
};

const uint8_t drum2[] PROGMEM = {
  0x49, 0x4E, 0x46, 0x4F, 
0x49, 0x53, 0x46, 0x54, 0x0D, 0x00, 0x00, 0x00, 0x4C, 0x61, 0x76, 0x66, 0x36, 0x30, 0x2E, 0x35, 
0x2E, 0x31, 0x30, 0x30, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61, 0x93, 0x04, 0x00, 0x00, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x80, 0x7F, 0x75, 0x7F, 0x81, 0x8A, 0x82, 0x80, 0x86, 0x7C, 0x83, 0x86, 
0x75, 0x80, 0x82, 0x7B, 0x86, 0x80, 0x81, 0x86, 0x50, 0x73, 0x8C, 0x71, 0xB1, 0x8F, 0x99, 0x7F, 
0x82, 0x91, 0x56, 0x68, 0x66, 0x88, 0x80, 0x6D, 0x7E, 0xA1, 0x7B, 0xA0, 0x8C, 0x80, 0x8F, 0x67, 
0x86, 0x75, 0x6D, 0x66, 0x79, 0x80, 0x9F, 0x6F, 0xA1, 0x7E, 0x72, 0xA2, 0x62, 0xA2, 0x82, 0x4B, 
0xA8, 0x59, 0x72, 0xA6, 0x6A, 0x7F, 0x83, 0x5D, 0x92, 0x85, 0x6B, 0xA1, 0x60, 0x98, 0x95, 0x85, 
0xA1, 0x69, 0x92, 0x83, 0x6C, 0x9B, 0x6B, 0x73, 0x9D, 0x6B, 0x76, 0x92, 0x5B, 0x99, 0x77, 0x83, 
0x88, 0x66, 0x9B, 0x7E, 0x79, 0x8E, 0x6D, 0x7A, 0x86, 0x60, 0x94, 0x7D, 0x6C, 0xB6, 0x5B, 0x86, 
0x96, 0x6A, 0x9D, 0x6D, 0x83, 0x8B, 0x6B, 0x90, 0x81, 0x6A, 0x9E, 0x5F, 0x6A, 0xAF, 0x5C, 0x97, 
0x8E, 0x70, 0x94, 0x7D, 0x87, 0x8E, 0x65, 0x7B, 0x7C, 0x78, 0xA1, 0x6A, 0x80, 0x94, 0x60, 0x99, 
0x72, 0x80, 0x92, 0x6F, 0x96, 0x7F, 0x6C, 0x99, 0x71, 0x7E, 0x8C, 0x66, 0x97, 0x85, 0x73, 0x91, 
0x6C, 0x75, 0x86, 0x76, 0x88, 0x74, 0x85, 0x8F, 0x62, 0x97, 0x8B, 0x66, 0xA7, 0x6E, 0x7C, 0x94, 
0x72, 0x9D, 0x7E, 0x72, 0x97, 0x70, 0x75, 0x97, 0x63, 0x8A, 0x80, 0x6F, 0x94, 0x69, 0x90, 0x79, 
0x6C, 0x99, 0x65, 0x8A, 0x8C, 0x6E, 0x9D, 0x78, 0x71, 0x93, 0x67, 0x87, 0x8D, 0x68, 0x98, 0x77, 
0x78, 0x94, 0x70, 0x98, 0x76, 0x71, 0xA1, 0x71, 0x92, 0x95, 0x60, 0x94, 0x7B, 0x71, 0x97, 0x60, 
0x8F, 0x84, 0x74, 0x9B, 0x73, 0x7D, 0x87, 0x6B, 0x85, 0x77, 0x6D, 0x94, 0x72, 0x84, 0x8B, 0x6B, 
0x91, 0x77, 0x89, 0x98, 0x6C, 0x8F, 0x87, 0x6B, 0x97, 0x7A, 0x77, 0x8F, 0x73, 0x86, 0x85, 0x71, 
0x8B, 0x71, 0x81, 0x8A, 0x73, 0x89, 0x79, 0x80, 0x82, 0x6C, 0x89, 0x79, 0x73, 0x94, 0x7A, 0x81, 
0x97, 0x73, 0x85, 0x85, 0x6E, 0x94, 0x76, 0x80, 0x87, 0x75, 0x8B, 0x7A, 0x7F, 0x82, 0x77, 0x8A, 
0x82, 0x7B, 0x8C, 0x73, 0x7C, 0x91, 0x69, 0x87, 0x81, 0x74, 0x90, 0x7B, 0x7D, 0x85, 0x78, 0x85, 
0x7A, 0x85, 0x86, 0x78, 0x8A, 0x7B, 0x74, 0x8C, 0x79, 0x8D, 0x86, 0x76, 0x89, 0x7A, 0x89, 0x7E, 
0x6D, 0x88, 0x78, 0x72, 0x93, 0x73, 0x84, 0x8E, 0x69, 0x8A, 0x81, 0x7C, 0x8E, 0x6E, 0x86, 0x8B, 
0x75, 0x95, 0x7C, 0x7B, 0x89, 0x74, 0x87, 0x7E, 0x76, 0x8E, 0x72, 0x7D, 0x85, 0x6E, 0x84, 0x76, 
0x74, 0x88, 0x73, 0x8A, 0x82, 0x73, 0x95, 0x76, 0x82, 0x89, 0x71, 0x8B, 0x84, 0x82, 0x90, 0x76, 
0x88, 0x86, 0x6E, 0x87, 0x84, 0x70, 0x8A, 0x7F, 0x81, 0x80, 0x7E, 0x89, 0x74, 0x87, 0x7E, 0x72, 
0x89, 0x7F, 0x7D, 0x7F, 0x7A, 0x85, 0x7F, 0x73, 0x8D, 0x7C, 0x85, 0x89, 0x7A, 0x88, 0x83, 0x79, 
0x83, 0x73, 0x80, 0x84, 0x79, 0x86, 0x78, 0x83, 0x88, 0x73, 0x8B, 0x82, 0x76, 0x90, 0x7A, 0x82, 
0x85, 0x72, 0x84, 0x7A, 0x7D, 0x84, 0x80, 0x88, 0x84, 0x7F, 0x8F, 0x79, 0x81, 0x86, 0x6D, 0x8E, 
0x7A, 0x7A, 0x8A, 0x75, 0x82, 0x85, 0x74, 0x89, 0x7C, 0x77, 0x8A, 0x7B, 0x85, 0x7E, 0x7B, 0x88, 
0x73, 0x7E, 0x85, 0x75, 0x8E, 0x7D, 0x7E, 0x8D, 0x77, 0x89, 0x87, 0x74, 0x8B, 0x7C, 0x7B, 0x8F, 
0x78, 0x85, 0x83, 0x7C, 0x82, 0x77, 0x81, 0x7D, 0x72, 0x87, 0x7D, 0x7B, 0x90, 0x75, 0x81, 0x83, 
0x78, 0x88, 0x7C, 0x7C, 0x86, 0x7B, 0x88, 0x80, 0x7C, 0x89, 0x7E, 0x81, 0x87, 0x77, 0x89, 0x7D, 
0x7D, 0x89, 0x78, 0x89, 0x85, 0x75, 0x89, 0x79, 0x7E, 0x88, 0x76, 0x82, 0x7F, 0x7B, 0x89, 0x79, 
0x85, 0x85, 0x77, 0x8A, 0x7E, 0x79, 0x86, 0x76, 0x83, 0x7D, 0x75, 0x8D, 0x79, 0x81, 0x83, 0x79, 
0x8B, 0x83, 0x7F, 0x8D, 0x76, 0x87, 0x87, 0x79, 0x8A, 0x78, 0x7C, 0x89, 0x74, 0x83, 0x7F, 0x7E, 
0x8F, 0x76, 0x81, 0x86, 0x78, 0x89, 0x76, 0x7B, 0x84, 0x79, 0x8A, 0x83, 0x78, 0x8B, 0x79, 0x7F, 
0x86, 0x71, 0x8A, 0x80, 0x7D, 0x8F, 0x78, 0x83, 0x86, 0x75, 0x85, 0x7A, 0x7A, 0x85, 0x77, 0x8A, 
0x7F, 0x7A, 0x8B, 0x7E, 0x87, 0x86, 0x75, 0x84, 0x7B, 0x7B, 0x89, 0x76, 0x82, 0x85, 0x7B, 0x8B, 
0x7B, 0x83, 0x87, 0x79, 0x84, 0x7D, 0x7B, 0x87, 0x78, 0x80, 0x84, 0x76, 0x8A, 0x7D, 0x7C, 0x89, 
0x74, 0x83, 0x80, 0x7A, 0x8B, 0x7B, 0x7F, 0x89, 0x77, 0x88, 0x81, 0x7B, 0x8B, 0x79, 0x84, 0x86, 
0x79, 0x8A, 0x82, 0x7C, 0x89, 0x7A, 0x83, 0x86, 0x77, 0x85, 0x79, 0x7D, 0x83, 0x76, 0x82, 0x7D, 
0x7A, 0x88, 0x79, 0x84, 0x86, 0x79, 0x86, 0x7F, 0x7B, 0x89, 0x79, 0x84, 0x84, 0x78, 0x8A, 0x7E, 
0x7F, 0x88, 0x7B, 0x87, 0x80, 0x7A, 0x8B, 0x7C, 0x80, 0x85, 0x76, 0x84, 0x7B, 0x7A, 0x86, 0x77, 
0x84, 0x84, 0x7B, 0x8B, 0x7C, 0x7D, 0x86, 0x77, 0x84, 0x83, 0x77, 0x87, 0x7B, 0x81, 0x83, 0x77, 
0x86, 0x7D, 0x7C, 0x89, 0x78, 0x83, 0x85, 0x79, 0x87, 0x7F, 0x7F, 0x86, 0x77, 0x83, 0x7F, 0x7D, 
0x87, 0x7A, 0x7F, 0x83, 0x7A, 0x83, 0x7E, 0x7E, 0x84, 0x7C, 0x86, 0x83, 0x7D, 0x88, 0x7B, 0x80, 
0x84, 0x78, 0x85, 0x7E, 0x79, 0x86, 0x7B, 0x80, 0x83, 0x78, 0x86, 0x7D, 0x7E, 0x86, 0x79, 0x83, 
0x81, 0x79, 0x86, 0x7C, 0x7E, 0x82, 0x79, 0x85, 0x7E, 0x7C, 0x87, 0x7A, 0x81, 0x82, 0x7A, 0x86, 
0x7F, 0x7D, 0x84, 0x7A, 0x81, 0x80, 0x7B, 0x82, 0x7B, 0x80, 0x84, 0x7B, 0x86, 0x7F, 0x7C, 0x85, 
0x7A, 0x80, 0x84, 0x7A, 0x85, 0x7F, 0x7F, 0x85, 0x7A, 0x84, 0x80, 0x7C, 0x85, 0x7C, 0x80, 0x83, 
0x7C, 0x84, 0x81, 0x7D, 0x85, 0x7B, 0x81, 0x82, 0x79, 0x86, 0x7E, 0x80, 0x85, 0x7B, 0x83, 0x82, 
0x7B, 0x82, 0x7C, 0x80, 0x83, 0x7B, 0x84, 0x7D, 0x7D, 0x86, 0x7D, 0x83, 0x84, 0x7C, 0x87, 0x7D, 
0x7F, 0x84, 0x7B, 0x82, 0x80, 0x7B, 0x84, 0x7F, 0x81, 0x83, 0x7A, 0x84, 0x7E, 0x7E, 0x86, 0x7A, 
0x80, 0x81, 0x7C, 0x85, 0x7E, 0x7F, 0x84, 0x7C, 0x83, 0x81, 0x7E, 0x85, 0x7C, 0x82, 0x82, 0x7A, 
0x86, 0x7F, 0x7F, 0x85, 0x7B, 0x81, 0x81, 0x7A, 0x83, 0x7C, 0x7F, 0x83, 0x7C, 0x85, 0x80, 0x7D, 
0x86, 0x7B, 0x80, 0x83, 0x7B, 0x85, 0x80, 0x7F, 0x87, 0x7D, 0x83, 0x83, 0x7B, 0x84, 0x7F, 0x80, 
0x85, 0x7C, 0x85, 0x81, 0x7D, 0x83, 0x7A, 0x7F, 0x81, 0x79, 0x83, 0x7E, 0x7D, 0x86, 0x7C, 0x81, 
0x82, 0x7B, 0x86, 0x7D, 0x7E, 0x85, 0x7C, 0x84, 0x81, 0x7C, 0x85, 0x7E, 0x81, 0x84, 0x7D, 0x84, 
0x80, 0x7E, 0x85, 0x7B, 0x80, 0x81, 0x7B, 0x83, 0x7E, 0x7E, 0x83, 0x7A, 0x81, 0x7E, 0x7C, 0x83, 
0x7D, 0x80, 0x82, 0x7C, 0x83, 0x81, 0x7F, 0x83, 0x7C, 0x81, 0x83, 0x7D, 0x84, 0x7E, 0x7F, 0x84, 
0x7E, 0x83, 0x7F, 0x7E, 0x83, 0x7D, 0x80, 0x82, 0x7D, 0x83, 0x7E, 0x7D, 0x83, 0x7C, 0x81, 0x80, 
0x7D, 0x83, 0x7E, 0x80, 0x82, 0x7E, 0x82, 0x80, 0x7F, 0x83, 0x7E, 0x81, 0x81, 0x7C, 0x84, 0x7D, 
0x80, 0x83, 0x7D, 0x83, 0x81, 0x7E, 0x84, 0x7E, 0x7F, 0x82, 0x7C, 0x82, 0x80, 0x7E, 0x84, 0x7D, 
0x83, 0x81, 0x7C, 0x84, 0x7E, 0x80, 0x84, 0x7B, 0x83, 0x81, 0x7E, 0x83, 0x7D, 0x80, 0x82, 0x7D, 
0x83, 0x7F, 0x7F, 0x84, 0x7D, 0x82, 0x81, 0x7C, 0x84, 0x7D, 0x80, 0x82, 0x7C, 0x83, 0x80, 0x7D, 
0x84, 0x7D, 0x81, 0x82, 0x7D, 0x84, 0x7F, 0x80, 0x84, 0x7D, 0x83, 0x80, 0x7E, 0x83, 0x7D, 0x81, 
0x82, 0x7C, 0x85, 0x7F, 0x7F, 0x84, 0x7C, 0x82, 0x82, 0x7D, 0x83, 0x7E, 0x7F, 0x82, 0x7C, 0x83, 
0x81, 0x7D, 0x83, 0x7D, 0x81, 0x82, 0x7D, 0x83, 0x7F, 0x7F, 0x84, 0x7D, 0x82, 0x81, 0x7E, 0x84, 
0x7D, 0x7F, 0x82, 0x7C, 0x82, 0x80, 0x7D, 0x84, 0x7F, 0x81, 0x82, 0x7C, 0x82, 0x7E, 0x7E, 0x83, 
0x7C, 0x82, 0x82, 0x7E, 0x85, 0x7E, 0x80, 0x82, 0x7D, 0x83, 0x80, 0x7D, 0x84, 0x7E, 0x80, 0x82, 
0x7C, 0x82, 0x7F, 0x7D, 0x82, 0x7C, 0x81, 0x81, 0x7D, 0x83, 0x7D, 0x7F, 0x83, 0x7C, 0x83, 0x81, 
0x7D, 0x85, 0x7E, 0x80, 0x82, 0x7C, 0x83, 0x80, 0x7F, 0x83, 0x7D, 0x81, 0x82, 0x7D, 0x82, 0x7E, 
0x7F, 0x82, 0x7D, 0x82, 0x80, 0x7D, 0x83, 0x7D, 0x7F, 0x81, 0x7C, 0x83, 0x7F, 0x7F, 0x83, 0x7D, 
0x82, 0x80, 0x7D, 0x82, 0x7E, 0x80, 0x83, 0x7D, 0x83, 0x80, 0x7E, 0x83, 0x7D, 0x80, 0x81, 0x7E

};

const uint8_t drum3[] PROGMEM = {
 0x49, 0x4E, 0x46, 0x4F, 
0x49, 0x53, 0x46, 0x54, 0x0D, 0x00, 0x00, 0x00, 0x4C, 0x61, 0x76, 0x66, 0x36, 0x30, 0x2E, 0x35, 
0x2E, 0x31, 0x30, 0x30, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61, 0x7E, 0x03, 0x00, 0x00, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x81, 0x7F, 0x82, 0x7C, 0x85, 0x74, 0x8B, 0x7E, 0x84, 0x74, 0x84, 0x7F, 
0x72, 0x93, 0x51, 0x89, 0x5B, 0x69, 0x8B, 0x66, 0x4A, 0x81, 0x60, 0x6E, 0x4B, 0x72, 0x75, 0xBD, 
0x6D, 0xA4, 0x95, 0x91, 0x9C, 0x99, 0x8A, 0x71, 0x96, 0x8D, 0x55, 0x7A, 0x76, 0x88, 0x90, 0x6B, 
0xAB, 0x66, 0x93, 0xAB, 0x96, 0xA1, 0xC0, 0x97, 0x92, 0x7A, 0x8B, 0xA3, 0x6C, 0x6A, 0x7E, 0x6D, 
0x54, 0x68, 0x67, 0x9A, 0x73, 0x53, 0x9C, 0x4D, 0x6A, 0x7E, 0x5A, 0x8C, 0x4F, 0x6A, 0x7E, 0x54, 
0x68, 0x77, 0x8A, 0x67, 0x95, 0x65, 0xA0, 0x7E, 0x92, 0xB0, 0xA3, 0x78, 0x91, 0x9A, 0xA0, 0x83, 
0x65, 0xB7, 0x60, 0x85, 0x91, 0x68, 0x78, 0x63, 0x72, 0xA2, 0x9C, 0xA0, 0xC3, 0x9E, 0x9F, 0xA2, 
0xB1, 0x77, 0x84, 0xA5, 0x71, 0x91, 0x7E, 0x90, 0x74, 0x76, 0x97, 0xA0, 0x8D, 0x6B, 0xA3, 0x78, 
0x8C, 0x7A, 0x9E, 0x85, 0x7E, 0x6F, 0x79, 0x59, 0x6E, 0x90, 0x6E, 0xA1, 0x60, 0x7B, 0x86, 0x8C, 
0x75, 0x95, 0x80, 0x74, 0x93, 0x66, 0x78, 0x5D, 0x5A, 0x65, 0x4F, 0x72, 0x64, 0x4C, 0x74, 0x71, 
0x6E, 0x9A, 0x69, 0x70, 0x85, 0x4F, 0x97, 0x4B, 0x71, 0x7B, 0x72, 0x5F, 0x79, 0x60, 0x90, 0x87, 
0x6B, 0x86, 0x85, 0x7D, 0x78, 0x91, 0x75, 0x78, 0x75, 0x8D, 0x7C, 0x72, 0x73, 0x99, 0x99, 0x8B, 
0xA7, 0x9E, 0x8C, 0xCE, 0x80, 0x77, 0xC0, 0x81, 0x9C, 0x92, 0x88, 0xB1, 0x8C, 0x90, 0xA7, 0x7F, 
0x90, 0x99, 0x6F, 0x7C, 0x77, 0x76, 0x8C, 0x89, 0x9E, 0x95, 0x9E, 0x7D, 0xAE, 0x9E, 0x6D, 0xA7, 
0xA4, 0x61, 0x8E, 0x7C, 0x7A, 0x80, 0x57, 0x9B, 0x79, 0x58, 0x93, 0x7F, 0x68, 0x8B, 0x98, 0x6E, 
0x98, 0x78, 0xA5, 0x7C, 0x77, 0x81, 0x89, 0x6D, 0x8C, 0x5F, 0x63, 0x9D, 0x4E, 0x7D, 0x7F, 0x5C, 
0x71, 0x85, 0x5A, 0x9A, 0x63, 0x7B, 0x80, 0x70, 0x54, 0x93, 0x5C, 0x7A, 0x6D, 0x55, 0x8E, 0x7A, 
0x75, 0x7B, 0x7A, 0x54, 0x7F, 0x54, 0x80, 0x53, 0x7C, 0x7C, 0x7A, 0x6A, 0x8F, 0x83, 0x69, 0x9E, 
0x6A, 0x8B, 0x5E, 0x81, 0x6F, 0x80, 0x5E, 0x99, 0x58, 0x93, 0x67, 0x81, 0x6F, 0x87, 0x6B, 0x7E, 
0x92, 0x7D, 0x79, 0x90, 0x7D, 0x92, 0x83, 0x80, 0xB2, 0x6A, 0x9E, 0x88, 0x99, 0x71, 0x96, 0x8D, 
0x94, 0x77, 0x89, 0x88, 0x79, 0x8D, 0x82, 0x91, 0x92, 0x76, 0x85, 0xA1, 0x5B, 0x94, 0x7F, 0x7E, 
0x75, 0x89, 0x90, 0x7B, 0x7F, 0x83, 0x9B, 0x6C, 0x8E, 0x8D, 0x71, 0x8D, 0x71, 0x7B, 0x8F, 0x77, 
0x8E, 0x7E, 0x8F, 0x81, 0x95, 0x6C, 0x92, 0x8A, 0x81, 0x98, 0x7B, 0x8D, 0x8A, 0x93, 0x6F, 0xA2, 
0x76, 0x9D, 0x64, 0x94, 0x80, 0x8C, 0x70, 0x90, 0x85, 0x89, 0x7E, 0x7A, 0x92, 0x7F, 0x74, 0x74, 
0x95, 0x74, 0x78, 0x7B, 0x82, 0x7A, 0x7C, 0x6B, 0x8E, 0x6E, 0x80, 0x8F, 0x81, 0x84, 0x83, 0x9B, 
0x78, 0x8A, 0x80, 0x83, 0x7C, 0x80, 0x81, 0x76, 0x7B, 0x87, 0x6E, 0x8C, 0x85, 0x73, 0x96, 0x7A, 
0x8F, 0x7B, 0x82, 0x72, 0x8F, 0x65, 0x99, 0x73, 0x81, 0x96, 0x79, 0xA1, 0x7F, 0x98, 0x84, 0x93, 
0x7A, 0x88, 0x93, 0x76, 0x91, 0x7F, 0x86, 0x8A, 0x82, 0x71, 0x81, 0x74, 0x8C, 0x7B, 0x75, 0x8D, 
0x7E, 0x86, 0x67, 0x9F, 0x6F, 0x88, 0x71, 0x80, 0x92, 0x73, 0x8C, 0x79, 0x90, 0x7C, 0x8D, 0x70, 
0x93, 0x67, 0x77, 0x80, 0x74, 0x73, 0x7D, 0x72, 0x70, 0x83, 0x5F, 0x89, 0x68, 0x6C, 0x78, 0x6D, 
0x72, 0x7D, 0x6F, 0x7C, 0x7B, 0x6E, 0x82, 0x6C, 0x6F, 0x88, 0x57, 0x81, 0x6C, 0x75, 0x76, 0x81, 
0x7D, 0x81, 0x7C, 0x7B, 0x83, 0x6F, 0x91, 0x6B, 0x76, 0x7F, 0x6D, 0x76, 0x7A, 0x6E, 0x82, 0x7C, 
0x72, 0x8B, 0x7B, 0x77, 0x76, 0x82, 0x74, 0x7B, 0x74, 0x77, 0x73, 0x7B, 0x72, 0x7A, 0x78, 0x84, 
0x83, 0x80, 0x79, 0x85, 0x82, 0x7E, 0x7D, 0x73, 0x84, 0x76, 0x73, 0x88, 0x78, 0x80, 0x7F, 0x82, 
0x83, 0x88, 0x77, 0x85, 0x82, 0x6E, 0x8D, 0x78, 0x83, 0x8B, 0x7B, 0x88, 0x7F, 0x79, 0x85, 0x81, 
0x74, 0x8B, 0x6E, 0x86, 0x73, 0x87, 0x70, 0x84, 0x7B, 0x83, 0x7E, 0x7B, 0x82, 0x82, 0x8C, 0x70, 
0x9D, 0x73, 0x8B, 0x85, 0x80, 0x8F, 0x85, 0x90, 0x7E, 0x90, 0x81, 0x94, 0x83, 0x90, 0x91, 0x87, 
0x95, 0x8D, 0x8B, 0x8A, 0x8E, 0x80, 0x90, 0x83, 0x80, 0x93, 0x7B, 0x83, 0x8F, 0x8C, 0x80, 0x96, 
0x85, 0x95, 0x8A, 0x8D, 0x9B, 0x89, 0x8C, 0x93, 0x87, 0x84, 0x8D, 0x89, 0x8C, 0x8D, 0x86, 0x98, 
0x84, 0x91, 0x8F, 0x8C, 0x8C, 0x8F, 0x8B, 0x86, 0x83, 0x87, 0x7D, 0x82, 0x7D, 0x80, 0x83, 0x86, 
0x86, 0x85, 0x85, 0x84, 0x8B, 0x8C, 0x81, 0x88, 0x7A, 0x88, 0x7E, 0x82, 0x76, 0x86, 0x7F, 0x7A, 
0x8C, 0x78, 0x8E, 0x7F, 0x92, 0x7D, 0x83, 0x8A, 0x78, 0x7B, 0x7E, 0x83, 0x72, 0x7B, 0x78, 0x79, 
0x73, 0x79, 0x77, 0x75, 0x7C, 0x74, 0x79, 0x72, 0x82, 0x74, 0x78, 0x74, 0x77, 0x79, 0x71, 0x69, 
0x85, 0x6B, 0x6F, 0x83, 0x6F, 0x85, 0x73, 0x7E, 0x85, 0x7B, 0x75, 0x81, 0x75, 0x83, 0x76, 0x7C, 
0x85, 0x77, 0x85, 0x7F, 0x7F, 0x84, 0x7A, 0x7A, 0x83, 0x73, 0x79, 0x7A, 0x74, 0x79, 0x71, 0x7F, 
0x75, 0x83, 0x79, 0x7F, 0x79, 0x7E, 0x78, 0x77, 0x7E, 0x75, 0x7F, 0x7E, 0x75, 0x80, 0x79, 0x7E, 
0x7F, 0x78, 0x85, 0x7D, 0x7C, 0x87, 0x7C, 0x82, 0x83, 0x83, 0x84, 0x87, 0x82, 0x7D, 0x90, 0x76, 
0x8A, 0x81, 0x81, 0x7B, 0x80, 0x7F, 0x78, 0x82, 0x73, 0x86, 0x7A, 0x88, 0x78, 0x88, 0x83, 0x79, 
0x91, 0x7A, 0x83, 0x7E, 0x83, 0x7D, 0x8A, 0x81, 0x7F, 0x88, 0x79, 0x84, 0x7D, 0x84, 0x79, 0x81, 
0x77, 0x7F, 0x7A, 0x7C, 0x7F, 0x7A, 0x7A, 0x80, 0x82, 0x7C, 0x7E, 0x83, 0x81, 0x7F, 0x7B, 0x8A, 
0x7F, 0x7F, 0x7F, 0x84, 0x77, 0x86, 0x79, 0x7C, 0x84, 0x78, 0x87, 0x7C, 0x84, 0x80, 0x8A, 0x7B, 
0x8B, 0x7E, 0x85, 0x81, 0x77, 0x85, 0x73, 0x7B, 0x72, 0x78, 0x74, 0x77, 0x77, 0x78, 0x80, 0x7E, 
0x82, 0x7D, 0x83, 0x7F, 0x7F, 0x7B, 0x80, 0x7E, 0x7B, 0x7F, 0x79, 0x7F, 0x76, 0x83, 0x7C, 0x7C, 
0x7C, 0x7F, 0x7D, 0x75, 0x80, 0x79, 0x7A, 0x86, 0x79, 0x82, 0x81, 0x82, 0x85, 0x8C, 0x80, 0x8A, 
0x8A, 0x86, 0x8A, 0x85, 0x88, 0x87, 0x84, 0x84, 0x8B, 0x84, 0x8A, 0x89, 0x8B, 0x8D, 0x84, 0x8D, 
0x85, 0x8A, 0x85, 0x84, 0x86, 0x7F, 0x82, 0x81, 0x89, 0x78, 0x88, 0x83
};

const uint8_t drum4[] PROGMEM = {
  0x49, 0x4E, 0x46, 0x4F, 
0x49, 0x53, 0x46, 0x54, 0x0D, 0x00, 0x00, 0x00, 0x4C, 0x61, 0x76, 0x66, 0x36, 0x30, 0x2E, 0x35, 
0x2E, 0x31, 0x30, 0x30, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61, 0x0D, 0x02, 0x00, 0x00, 0x8C, 0x6E, 
0x80, 0x97, 0x90, 0x87, 0x85, 0x92, 0x89, 0x7F, 0x7B, 0x7E, 0x87, 0x89, 0x85, 0x7D, 0x83, 0x8B, 
0x89, 0x85, 0x83, 0x84, 0x89, 0x86, 0x7F, 0x80, 0x7C, 0x82, 0x7C, 0x7F, 0x87, 0x80, 0x7F, 0x7C, 
0x7E, 0x81, 0x7A, 0x83, 0x7D, 0x7B, 0x82, 0x7F, 0x7C, 0x80, 0x7E, 0x83, 0x7D, 0x81, 0x7E, 0x7B, 
0x82, 0x7C, 0x7F, 0x7F, 0x7F, 0x7B, 0x80, 0x7F, 0x7F, 0x7F, 0x80, 0x7C, 0x7E, 0x80, 0x7C, 0x81, 
0x7D, 0x7D, 0x7E, 0x80, 0x7C, 0x80, 0x7D, 0x7F, 0x7D, 0x7E, 0x7F, 0x7E, 0x7C, 0x7F, 0x7E, 0x7D, 
0x80, 0x7F, 0x7A, 0x83, 0x7C, 0x7D, 0x80, 0x7D, 0x7E, 0x7F, 0x7E, 0x7E, 0x7D, 0x80, 0x7E, 0x7D, 
0x7F, 0x80, 0x7C, 0x80, 0x7E, 0x7D, 0x80, 0x7D, 0x7E, 0x7F, 0x7D, 0x7E, 0x7E, 0x7F, 0x7E, 0x7E, 
0x7F, 0x7E, 0x7E, 0x80, 0x7D, 0x7E, 0x7E, 0x7E, 0x7F, 0x7D, 0x7F, 0x7D, 0x7E, 0x7F, 0x7D, 0x7E, 
0x7F, 0x7E, 0x7E, 0x7E, 0x7E, 0x7D, 0x80, 0x7D, 0x7E, 0x7F, 0x7E, 0x7E, 0x7F, 0x7D, 0x7F, 0x7E, 
0x7E, 0x7F, 0x7E, 0x7E, 0x7F, 0x7D, 0x7F, 0x7E, 0x7F, 0x7F, 0x7E, 0x7E, 0x7F, 0x7E, 0x7F, 0x7F, 
0x7F, 0x7E, 0x81, 0x7D, 0x7F, 0x80, 0x7E, 0x7F, 0x7F, 0x7F, 0x80, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
0x80, 0x7F, 0x7F, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x81, 0x7F, 
0x80, 0x80, 0x7F, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x7F, 0x80, 0x80, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x7F, 0x81, 0x80, 0x81, 0x81, 0x80, 0x80, 0x80, 0x81, 0x80, 
0x81, 0x81, 0x80, 0x81, 0x80, 0x80, 0x81, 0x80, 0x81, 0x80, 0x81, 0x80, 0x80, 0x81, 0x81, 0x80, 
0x80, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x7F, 0x80, 0x80, 0x80, 0x81, 0x80, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x7F, 0x7F, 0x80, 0x80, 0x7F, 
0x80, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x7F, 0x80, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
0x7F, 0x7E, 0x7F, 0x7F, 0x7E, 0x7E, 0x7E, 0x7E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7E, 0x7F, 
0x7E, 0x7E, 0x7E, 0x7E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 
0x7E, 0x7E, 0x7F, 0x7E, 0x7E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7F, 
0x7F, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x80, 0x7F, 
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x7F, 0x80, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x7F, 0x80, 0x7F, 0x7F, 0x80, 0x7F, 0x7F, 0x80, 0x7F, 0x7F, 0x80, 0x7F, 0x7F, 
0x7F, 0x80, 0x7F, 0x80, 0x7F

};

const uint8_t drum5[] PROGMEM = {
 0x49, 0x4E, 0x46, 0x4F, 
0x49, 0x53, 0x46, 0x54, 0x0D, 0x00, 0x00, 0x00, 0x4C, 0x61, 0x76, 0x66, 0x36, 0x30, 0x2E, 0x35, 
0x2E, 0x31, 0x30, 0x30, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61, 0xC4, 0x02, 0x00, 0x00, 0x80, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x81, 0x7E, 0x80, 0x81, 0x7D, 0x82, 0x7E, 0x83, 0x7C, 0x85, 0x82, 0x7D, 
0x82, 0x7E, 0x79, 0x7C, 0x7D, 0x7C, 0x7E, 0x80, 0x7F, 0x83, 0x7F, 0x84, 0x82, 0x83, 0x80, 0x7D, 
0x80, 0x7A, 0x7B, 0x79, 0x78, 0x7D, 0x7E, 0x83, 0x87, 0x8C, 0x86, 0x8B, 0x88, 0x89, 0x85, 0x86, 
0x7F, 0x7F, 0x7D, 0x7C, 0x7A, 0x78, 0x7E, 0x78, 0x7B, 0x7B, 0x7E, 0x7A, 0x7A, 0x7B, 0x7C, 0x81, 
0x77, 0x81, 0x80, 0x7F, 0x88, 0x80, 0x84, 0x85, 0x8A, 0x83, 0x89, 0x82, 0x81, 0x84, 0x7E, 0x80, 
0x79, 0x80, 0x7D, 0x7D, 0x7F, 0x7C, 0x81, 0x7D, 0x84, 0x7D, 0x83, 0x82, 0x7E, 0x80, 0x81, 0x7E, 
0x80, 0x7F, 0x7F, 0x7F, 0x7B, 0x83, 0x83, 0x7E, 0x81, 0x7E, 0x79, 0x7D, 0x7B, 0x78, 0x81, 0x80, 
0x81, 0x7F, 0x83, 0x81, 0x86, 0x87, 0x8B, 0x8A, 0x86, 0x85, 0x7F, 0x81, 0x83, 0x7C, 0x83, 0x82, 
0x86, 0x82, 0x7B, 0x7F, 0x7D, 0x86, 0x83, 0x88, 0x7C, 0x86, 0x83, 0x7F, 0x82, 0x84, 0x88, 0x87, 
0x88, 0x86, 0x83, 0x81, 0x84, 0x81, 0x80, 0x7D, 0x7C, 0x7D, 0x80, 0x7D, 0x7E, 0x7C, 0x82, 0x7D, 
0x81, 0x7E, 0x7A, 0x7E, 0x7D, 0x87, 0x7C, 0x82, 0x80, 0x83, 0x83, 0x82, 0x7E, 0x7E, 0x7F, 0x81, 
0x7D, 0x76, 0x79, 0x7B, 0x7A, 0x79, 0x7C, 0x78, 0x81, 0x81, 0x84, 0x85, 0x80, 0x81, 0x82, 0x83, 
0x7F, 0x7F, 0x7A, 0x7D, 0x7E, 0x7D, 0x7E, 0x7D, 0x7D, 0x80, 0x80, 0x81, 0x81, 0x82, 0x7C, 0x7E, 
0x7E, 0x7D, 0x7E, 0x7F, 0x7F, 0x81, 0x7F, 0x7F, 0x80, 0x82, 0x7E, 0x85, 0x83, 0x80, 0x80, 0x7F, 
0x7F, 0x83, 0x80, 0x7E, 0x81, 0x86, 0x83, 0x85, 0x78, 0x7C, 0x7F, 0x7F, 0x80, 0x7E, 0x7F, 0x7E, 
0x81, 0x81, 0x80, 0x83, 0x85, 0x83, 0x80, 0x87, 0x7F, 0x83, 0x80, 0x81, 0x83, 0x84, 0x83, 0x82, 
0x81, 0x80, 0x83, 0x7D, 0x81, 0x83, 0x81, 0x7E, 0x80, 0x82, 0x7E, 0x81, 0x7E, 0x80, 0x82, 0x84, 
0x80, 0x81, 0x83, 0x82, 0x80, 0x80, 0x7E, 0x7F, 0x80, 0x7C, 0x81, 0x7F, 0x80, 0x83, 0x82, 0x7F, 
0x81, 0x80, 0x7F, 0x7F, 0x7F, 0x7B, 0x7E, 0x81, 0x7E, 0x7A, 0x7B, 0x7E, 0x81, 0x7F, 0x80, 0x80, 
0x81, 0x85, 0x84, 0x82, 0x7E, 0x80, 0x83, 0x83, 0x81, 0x7F, 0x82, 0x82, 0x82, 0x7D, 0x7D, 0x7D, 
0x80, 0x7E, 0x7C, 0x7C, 0x7E, 0x80, 0x7F, 0x7E, 0x7F, 0x82, 0x80, 0x81, 0x7E, 0x7F, 0x81, 0x82, 
0x80, 0x80, 0x81, 0x7E, 0x82, 0x7F, 0x7E, 0x7E, 0x7F, 0x80, 0x7E, 0x7F, 0x80, 0x80, 0x80, 0x81, 
0x83, 0x80, 0x81, 0x80, 0x80, 0x82, 0x80, 0x82, 0x80, 0x7F, 0x81, 0x80, 0x7F, 0x80, 0x81, 0x81, 
0x81, 0x82, 0x84, 0x82, 0x84, 0x80, 0x81, 0x80, 0x83, 0x81, 0x7F, 0x7F, 0x80, 0x81, 0x81, 0x82, 
0x80, 0x83, 0x80, 0x81, 0x7F, 0x81, 0x7E, 0x7E, 0x80, 0x7B, 0x7F, 0x7E, 0x7D, 0x7C, 0x7F, 0x7D, 
0x81, 0x81, 0x80, 0x7F, 0x81, 0x83, 0x81, 0x82, 0x81, 0x84, 0x81, 0x82, 0x81, 0x81, 0x81, 0x7F, 
0x7E, 0x7E, 0x7D, 0x7F, 0x7E, 0x7C, 0x7E, 0x7C, 0x7F, 0x80, 0x7F, 0x80, 0x81, 0x80, 0x82, 0x80, 
0x80, 0x81, 0x81, 0x80, 0x80, 0x7F, 0x7E, 0x80, 0x7E, 0x80, 0x7F, 0x7D, 0x80, 0x81, 0x7E, 0x7D, 
0x7E, 0x80, 0x81, 0x80, 0x80, 0x80, 0x80, 0x7F, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x7E, 0x7E, 0x7F, 
0x7E, 0x7E, 0x80, 0x81, 0x80, 0x81, 0x7F, 0x7F, 0x80, 0x7F, 0x7E, 0x7E, 0x81, 0x7E, 0x7F, 0x7D, 
0x7F, 0x7F, 0x80, 0x80, 0x82, 0x80, 0x82, 0x82, 0x81, 0x82, 0x82, 0x81, 0x82, 0x82, 0x82, 0x81, 
0x80, 0x80, 0x7F, 0x7F, 0x80, 0x7E, 0x7F, 0x7E, 0x80, 0x80, 0x81, 0x81, 0x81, 0x82, 0x82, 0x83, 
0x82, 0x82, 0x83, 0x82, 0x81, 0x82, 0x81, 0x82, 0x81, 0x80, 0x81, 0x80, 0x82, 0x80, 0x80, 0x80, 
0x80, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x81, 0x81, 0x80, 0x81, 0x81, 0x80, 0x81, 0x81, 
0x81, 0x80, 0x81, 0x81, 0x81, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x7F, 0x80, 0x80, 0x81, 
0x81, 0x80, 0x80, 0x80, 0x82, 0x81, 0x82, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x7E, 0x7E, 
0x7F, 0x7E, 0x7F, 0x7F, 0x7E, 0x7F, 0x7F, 0x7F, 0x80, 0x80, 0x80, 0x81, 0x81, 0x81, 0x82, 0x80, 
0x81, 0x81, 0x7F, 0x7F, 0x7E, 0x7D, 0x7D, 0x7D, 0x7D, 0x7E, 0x7D, 0x7E, 0x7E, 0x7F, 0x7F, 0x80, 
0x80, 0x7F, 0x80, 0x80, 0x80, 0x80, 0x81, 0x80, 0x80, 0x80, 0x80, 0x7F, 0x7F, 0x7E, 0x7E, 0x7E, 
0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x80, 0x80, 0x7F, 0x7F, 0x80, 0x80, 0x7F, 
0x80, 0x80, 0x80, 0x80, 0x7F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x7F, 0x7F, 0x7F, 0x7F, 0x80, 0x7F, 
0x7F, 0x7F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x81, 0x81, 0x80, 0x81, 0x81, 0x81, 0x81, 0x80, 
0x80, 0x80, 0x80, 0x80, 0x7F, 0x7F, 0x80, 0x7F, 0x7F, 0x7F, 0x7F, 0x80, 0x80, 0x80, 0x81, 0x81, 
0x80, 0x81, 0x81, 0x81, 0x81, 0x80, 0x80, 0x80, 0x7F, 0x80, 0x7F, 0x7F, 0x7F, 0x7F, 0x80, 0x80, 
0x80, 0x81
};

const uint8_t drum6[] PROGMEM = {};

const uint8_t drum7[] PROGMEM = {};

const uint8_t drum8[] PROGMEM = {

};


void setup() {
  OSCCAL = 255;
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1 << PCKE | 1 << PLLE;

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                                     // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10;  // PWM A, clear on match, 1:1 prescale
  //GTCCR = 1<<PWM1B | 2<<COM1B0;           // PWM B, clear on match
  OCR1A = 128;  //OCR1B = 128;               // 50% duty at start

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);  // Enable PWM output pin
  //pinMode(2, INPUT_PULLUP);
  pinMode(2, INPUT);  //rstave - change to pulldown

  //Set up Timer/Counter0 for 20kHz interrupt to output samples.

  TCCR0A = 3 << WGM00;              // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00;  // 1/8 prescale
  TIMSK = 1 << OCIE0A;              // Enable compare match, disable overflow
  OCR0A = 49;                       // Divide by 400

  uint16_t dummy = analogRead(0);
}

void loop() {
  uint16_t samplecnt1, samplecnt2, samplecnt3, samplecnt4, samplecnt5, samplecnt6, samplecnt7, samplecnt8;
  samplecnt1 = samplecnt2 = samplecnt3 = samplecnt4 = samplecnt5 = samplecnt6 = samplecnt7 = samplecnt8 = 0;

  uint16_t samplepnt1, samplepnt2, samplepnt3, samplepnt4, samplepnt5, samplepnt6, samplepnt7, samplepnt8;
  uint8_t dread2 = 0;
  uint8_t odread2 = 0;
  uint16_t valRead = 44;
  uint8_t Seldrum = 0;
  uint8_t MUX = 2;

  sbi(ADCSRA, ADSC);  //start next conversation
  while (1) {
    if (RingCount < 32) {  //if space in ringbuffer
      int16_t total = 0;
      if (samplecnt1) {
        total += (pgm_read_byte_near(drum1 + samplepnt1++) - 128);
        samplecnt1--;
      }
      if (samplecnt2) {
        total += (pgm_read_byte_near(drum2 + samplepnt2++) - 128);
        samplecnt2--;
      }
      if (samplecnt3) {
        total += (pgm_read_byte_near(drum3 + samplepnt3++) - 128);
        samplecnt3--;
      }
      if (samplecnt4) {
        total += (pgm_read_byte_near(drum4 + samplepnt4++) - 128);
        samplecnt4--;
      }
      if (samplecnt5) {
        total += (pgm_read_byte_near(drum5 + samplepnt5++) - 128);
        samplecnt5--;
      }
      if (samplecnt6) {
        total += (pgm_read_byte_near(drum6 + samplepnt6++) - 128);
        samplecnt6--;
      }
      if (samplecnt7) {
        total += (pgm_read_byte_near(drum7 + samplepnt7++) - 128);
        samplecnt7--;
      }
      if (samplecnt8) {
        total += (pgm_read_byte_near(drum8 + samplepnt8++) - 128);
        samplecnt8--;
      }

      // total >>= 1;
      total += 128;
      if (total > 255) total = 255;
      cli();
      uint16_t playing = samplecnt1 + samplecnt2 + samplecnt3 + samplecnt4 + samplecnt5 + samplecnt6 + samplecnt7 + samplecnt8;
      digitalWrite(0, playing);

      Ringbuffer[RingWrite] = total;
      RingWrite++;
      RingCount++;
      sei();
      dread2 = digitalRead(2);
      if (dread2 != odread2) {
        odread2 = dread2;
        if (odread2) {



          if (valRead < 200) {
            OCR0A = 88 + ((127 - (valRead >> 3)));
            Seldrum = 0;
          } else if (valRead < 400) {
            OCR0A = 88 + ((127 - ((valRead - 200) >> 3)));
            Seldrum = 1;
          } else if (valRead < 600) {
            OCR0A = 88 + ((127 - ((valRead - 400) >> 3)));
            Seldrum = 2;
          } else if (valRead < 800) {
            OCR0A = 88 + ((127 - ((valRead - 600) >> 3)));
            Seldrum = 3;
          } else {
            OCR0A = 88 + ((127 - ((valRead - 800) >> 3)));
            Seldrum = 4;
          }


          if (Seldrum == 0) {
            samplepnt1 = 0;
            samplecnt1 = sizeof(drum1);
          }
          if (Seldrum == 1) {
            samplepnt2 = 0;
            samplecnt2 = sizeof(drum2);
          }
          if (Seldrum == 2) {
            samplepnt3 = 0;
            samplecnt3 = sizeof(drum3);
          }
          if (Seldrum == 3) {
            samplepnt4 = 0;
            samplecnt4 = sizeof(drum4);
          }
          if (Seldrum == 4) {
            samplepnt5 = 0;
            samplecnt5 = sizeof(drum5);
          }
          if (Seldrum == 5) {
            samplepnt6 = 0;
            samplecnt6 = sizeof(drum6);
          }
          if (Seldrum == 6) {
            samplepnt7 = 0;
            samplecnt7 = sizeof(drum7);
          }
          if (Seldrum == 7) {
            samplepnt8 = 0;
            samplecnt8 = sizeof(drum8);
          }
        }
      }

      if (!(ADCSRA & 64)) {
        if (MUX == 3) {
          // OCR0A = 49 + ((127 - ((ADCL + (ADCH << 8)) >> 3)));
          // currently not used.
        }
        if (MUX == 2) {
          //Seldrum = ((ADCL + (ADCH << 8)) >> 7);
          valRead = ((ADCL + (ADCH << 8)));
        }
        MUX++;
        if (MUX == 4) MUX = 2;
        ADMUX = MUX;        //Select MUX
        sbi(ADCSRA, ADSC);  //start next conversation
      }
    }
  }
}

ISR(TIMER0_COMPA_vect) {
  //-------------------  Ringbuffer handler -------------------------

  if (RingCount) {                     //If entry in FIFO..
    OCR1A = Ringbuffer[(RingRead++)];  //Output 8-bit DAC
    RingCount--;
  }

  //-----------------------------------------------------------------
}
