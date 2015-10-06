
/**
 * ACS-85-0307
 * ATTiny85  or/and Array.
 *
 * 3 inputs and 2 outputs.
 * 
 * First output is or of a and b
 * second is and with that output
 * 
 * a b c  out1 out2
 * 0 0 0  0    0
 * 0 1 0  1    0
 * 1 0 0  1    0
 * 1 1 0  1    0
 * 0 0 1  0    0  
 * 0 1 1  1    1
 * 1 0 1  1    1
 * 1 1 1  1    1 
 *
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = (a | b)
 * External pin 3 (PB4) = ( ( a | b ) & c ) 
 * External pin 4       = GND
 * External pin 5 (PB0) = a
 * External pin 6 (PB1) = b
 * External pin 7 (PB2) = c
 * External pin 8       = Vcc
 *
 * V 1.0  -  First Version
 * 
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * Rob Stave (Rob the fiddler) 2015
 */


//  ATTiny overview
//                           +-\/-+
//                   Reset  1|    |8  VCC
//         (pin3) PB4 HF_0  2|    |7  LFO_1 PB2 (pin2)
//         (pin4) PB3 HF_1  3|    |6  LFO_2 PB1 (pin1)
//                     GND  4|    |5  HF Ramp PB0 (pin0)

void setup() {
  DDRB = B00011000; // set PORTB outputs
  PORTB = B00000111; //enable all pull ups
}
 
void loop() {
  //Read Port B
  byte inputPortB = PINB;

  //Caculate values
  boolean outputBit1 = (bitRead(inputPortB, 0) | bitRead(inputPortB, 1)) ;
  boolean outputBit2 = (outputBit1 & bitRead(inputPortB, 2));

  //Write back to port b
  byte output = B00000000;
  bitWrite(output, 3, outputBit1);
  bitWrite(output, 4, outputBit2);  
  PORTB = output;
}
