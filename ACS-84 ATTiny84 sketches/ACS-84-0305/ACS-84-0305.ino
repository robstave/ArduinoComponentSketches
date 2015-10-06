
#include            <avr/io.h>                                                                                                                                                                                                                                                                                                                                                                         /*            attiny            register            names            */
#include            <avr/interrupt.h>

/**
 * ACS-84-0305
 * ATTiny84  Xor Array.
 *
 * Array of Xors.
 * 8 inputs and 3 outputs.
 * 
 * First output is xor of the first 2 inputs
 * Second is xor of first 3 inputs
 * Last is an Xor of all inputs
 *
 * External pin 2 = a^b
 * External pin 3 = a^b^c
 * External pin 5 = a^b..^h
 * External pin 6 = h
 * External pin 7 = g
 * External pin 8 = f
 * External pin 9 = e
 * External pin 10 = d
 * External pin 11 = c
 * External pin 12 = b
 * External pin 13 = a
 *
 * V 1.0  -  First Version
 *
 * Rob Stave (Rob the fiddler) 2015
 */
 

//  ATTiny overview
//                           +-\/-+
//                   Vcc  1|    |14  GND
//            A^B    PB0  2|    |13  PA0 Input A
//            A^B^C  PB1  3|    |12  PA1 Input B
//            Reset  PB3  4|    |11  PA2 Input C
//            A^all  PB2  5|    |10  PA3 Input D
//          Input H  PA7  6|    |9   PA4 Input E
//          Input G  PA6  7|    |8   PA5 Input F

//Counters in the interrupt to toggle pins.
// Increasing the number reduces the frequency.

// the setup function runs once when you press reset or power the board
void setup() {
  DDRA = B00000000; // set PORTA
  DDRB = B00000111; // set PORTB outputs
  PORTA = 0xFF; //enable all pull ups
}


void loop() {
  byte inputPortB = PINA;

  boolean outputBit1 = bitRead(inputPortB, 0) ^ bitRead(inputPortB, 1) ;
  boolean outputBit2 = outputBit1 ^ bitRead(inputPortB, 2);

  boolean outputBit3 = outputBit2 ^ bitRead(inputPortB, 3)
                       ^ bitRead(inputPortB, 4) ^ bitRead(inputPortB, 5)
                       ^ bitRead(inputPortB, 6) ^ bitRead(inputPortB, 7);

  byte output = B00000000;
  bitWrite(output, 0, outputBit1);
  bitWrite(output, 1, outputBit2);
  bitWrite(output, 2, outputBit3);

  PORTB = output;
}
