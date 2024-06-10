/**
   ACS-85-0650
   ATTiny85  Bytebeat  ( BitBeat)

   Bytebeat  simple 1 bit patterns
   Where the real bytebeat takes the values and uses the 8 bit values ( 0- 255), this
   just selectively uses single bits, evaluates if the value is > 127 or does some xor math.

   Ya..I realized after the fact that x>127 is just the last bit. Derp.
   Just play around.

   There is no speed on this

   based on
   https://www.reddit.com/r/diydsp/comments/ksgn2s/bytebeat_synth_on_a_single_attiny85/
   code used in jamuary02 - Bytebeat
     https://youtu.be/8HQUXlStqK8
     schematic at https://imgur.com/SekUTgN
     noizhardware.com
     enjoy :)

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 Parameter
   External pin 3 (PB4) = output 1 out
   External pin 4       = GND
   External pin 5 (PB0) = output 2
   External pin 6 (PB1) = output 3
   External pin 7 (PB2) = output 4
   External pin 8       = Vcc
   V 1.0  -  First Version
   Any further extensions to this will be treated as a sequencer.
   Rob Stave (Rob the fiddler) ccby 2021
*/



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) freq A3  PB3 2|    |7  PB2 (pin2) output 4
//         (pin4) out1  PB4 3|    |6  PB1 (pin1) output 3
//                      GND 4|    |5  PB0 (pin0) output 2
//


#include <avr/io.h>

#define OUT1 4 /* pin3 */
#define OUT2 0 /* pin5 */
#define OUT3 1 /* pin6 */
#define OUT4 2 /* pin7 */
#define POT_A 3 /* pin2 */

void setup() {
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(POT_A, INPUT);
}

void loop() {

  uint8_t readLoop = 0;

  int16_t t = 0;
  uint8_t out = 0;
  uint8_t PARAM_A;
  uint16_t var1;
  while (true) {

    readLoop++;
    if (readLoop > 4) {
      var1 = analogRead(POT_A);
      PARAM_A = map(var1, 0, 1023, 5, 15);
      readLoop = 0;
    }

    out = (128 & t * (4 | 7 & t >> PARAM_A) >> (1 & -t >> 11)) + (127 & t * (t >> 11 & t >> PARAM_A) * (3 & -t >> 9));

    // out 1 lsb of bytebeat
    if ( out == 0) {
      bitClear(PORTB, OUT1);
    } else {
      bitSet(PORTB, OUT1);
    }

    // toggle on 50%
    if ( out > 127) {
      bitSet(PORTB, OUT2);
    } else {
      bitClear(PORTB, OUT2);
    }


    if ( bitRead(out, 1) > 0) {
      bitSet(PORTB, OUT3);
    } else {
      bitClear(PORTB, OUT3);
    }

    // xor bits 1 and 2
    if ( bitRead(out, 1) ^  bitRead(out, 2) == 0) {
      bitSet(PORTB, OUT4);
    } else {
      bitClear(PORTB, OUT4);
    }

    t++;

  }

}
