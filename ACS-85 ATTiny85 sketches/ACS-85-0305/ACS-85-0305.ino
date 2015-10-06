
/**
 * ACS-85-0305
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
 * External pin 5 = a
 * External pin 6 = b
 * External pin 7 = c
 *
 * V 1.0  -  First Version
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

  
  
    byte inputPortB = PINB;
  
  boolean outputBit1 = bitRead(inputPortB, 0) ^ bitRead(inputPortB, 1) ;
  boolean outputBit2 = outputBit1 ^ bitRead(inputPortB, 2);

  byte output = B00000000;
  bitWrite(output, 3, outputBit1);
  bitWrite(output, 4, outputBit2);
  

  PORTB = output;
  
 
}
