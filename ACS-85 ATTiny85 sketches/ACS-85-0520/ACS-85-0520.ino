
/**
 * ACS-85-0520
 * ATTiny85  LFSR Clocked
 *
 * Can be used to produce random numbers and noise.
 * The LFSR actually clocks 4x for each clock as we
 * are masking the bottom 4 bits from the integer
 * So you can use it to get a pseudo random 4 bit 
 * pattern (or less)
 * 
 * https://en.wikipedia.org/wiki/Linear_feedback_shift_register
 * 
 * if you clock it 1x or clock it 4x, the unique value
 * still has a period of 2x16-1
 * (note clocking it 3x will reduce the uniqueness to 2x15-1)
 * 
 * External pin 1 = Reset
 * External pin 2 = LFSR bit 2
 * External pin 3 = LFSR bit 3
 * External pin 4 = GND
 * External pin 5 = LFSR bit 0
 * External pin 6 = LFSR bit 1
 * External pin 7 = Clock
 * External pin 8 = VCC
 * 
 *
 * V 1.0  -  First Version
 *
 * Rob Stave (Rob the fiddler) 2015
 */


//  ATTiny overview
//                               +-\/-+
//                       Reset  1|    |8  VCC
//          LFSR 2 (pin3) PB3   2|    |7   PB2 (pin2) Clock
//          LFSR 3 (pin4) PB4   3|    |6   PB1 (pin1) LFSR 1
//                         GND  4|    |5   PB0 (pin0) LFSR 0

//Byte used as the LFSR
unsigned int lfsr  = 1;
const int clockInt = 0;  // digital pin 2 is now interrupt 0


void setup() {
  DDRB = B00011011; // set PORTB outputs
  attachInterrupt(clockInt, clockCounter, CHANGE);
}

void clockCounter()      // called by interrupt
{
  clockLfsr ();
  clockLfsr ();
  clockLfsr ();
  clockLfsr ();
  
}
void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}
void loop() {

    //Mask off the state
  byte reg = lfsr & B00001111;
 
  byte output = B00000000;
  bitWrite(output, 0,  bitRead(reg, 0));
  bitWrite(output, 1, bitRead(reg, 1));
  bitWrite(output, 3, bitRead(reg, 2));
  bitWrite(output, 4, bitRead(reg, 3));

  PORTB = output;

}
