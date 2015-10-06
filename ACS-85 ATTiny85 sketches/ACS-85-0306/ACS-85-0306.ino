
/**
 * ACS-85-0306
 * ATTiny85  Nands.
 *
 * Couple of nands.
 * 3 inputs and 2 outputs.
 * 
 * First output is nand of the first 2 inputs
 * Second is nand output and 
 *
 * Can be used for 1 Mhz or 8 Mhz internal.  Should not 
 * make that much of a difference.
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = !(a & b)
 * External pin 3 (PB4) = !( ! (a & b) & c ) 
 * External pin 4       = GND
 * External pin 5 (PB0) = a
 * External pin 6 (PB1) = b
 * External pin 7 (PB2) = c
 * External pin 8       = Vcc
 *
 * a b c  out1 out2
 * 0 0 0  1    1
 * 0 1 0  1    1
 * 1 0 0  1    1
 * 1 1 0  0    1
 * 0 0 1  1    0  
 * 0 1 1  1    0
 * 1 0 1  1    0
 * 1 1 1  0    1 
 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 * 
 * Rob Stave (Rob the fiddler) 2015
 */


//  ATTiny overview
//                           +-\/-+
//                Reset PB5 1|    |8  VCC
//             output 1 PB3 2|    |7 PB2  input c
//             output 2 PB4 3|    |6 PB1  input b
//                      GND 4|    |5 PB0  input a
//                            ----

void setup() {
  DDRB = B00011000; // set PORTB outputs
  PORTB = B00000111; //enable all pull ups
}
 
void loop() {

  //Read port B
  byte inputPortB = PINB;

  //Calculate outputs
  boolean outputBit1 = !(bitRead(inputPortB, 0) & bitRead(inputPortB, 1)) ;
  boolean outputBit2 = !(outputBit1 & bitRead(inputPortB, 2));

  //Write to port b
  byte output = B00000000;
  bitWrite(output, 3, outputBit1); //Output to Port B3
  bitWrite(output, 4, outputBit2); //Output to Port B4
  PORTB = output;
}


