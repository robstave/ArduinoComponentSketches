
/**
 * ACS-85-0510
 * ATTiny85  Shift Register
 * 
 * Short Shift register
 * These are used in lunettas outputed to r2r ladders sometimes.
 * One bit delay perhaps.  This Sketch will not give you any more
 * functionality that a CD4014, but its a good start for you 
 * to add on for other similar sketches
 * 
 * This version is just a simple register using a long
 * and outputing the 0, 4 and 8th bits.
 * 
 * ACS-85-0512 does this with a much larger register
 *
 * 
 * External pin 1 = Reset
 * External pin 2 = Q1
 * External pin 3 = Q2
 * External pin 4 = GND
 * External pin 5 = Q0
 * External pin 6 = shift in
 * External pin 7 = Clock
 * External pin 8 = VCC
 *
 * V 1.0  -  First Version
 *
 * Rob Stave (Rob the fiddler) 2015 CCBY
 */


//  ATTiny overview
//                               +-\/-+
//                       Reset  1|    |8  VCC
//              Q1 (pin3) PB3   2|    |7   PB2 (pin2) Clock
//              Q2 (pin4) PB4   3|    |6   PB1 (pin1) Shift In
//                         GND  4|    |5   PB0 (pin0) Q0



//Shift register taps
//This is a number from 0 to 15
// Example Q0 = 2 means that PB0 will represent the
// value of the third (zero based) flip flop in a size 16 register. 
#define Q0 0
#define Q1 8
#define Q2 12


//Byte used as the Shift Register
unsigned int shiftRegister  = 0;
const int clockInt = 0;  // digital pin 2 is now interrupt 0
const int inputPin = 1;

void setup() {
  DDRB = B00011001; // set PORTB outputs
  attachInterrupt(clockInt, clockCounter, RISING);
}


void clockCounter()      // called by interrupt
{
  clockShiftRegister ();
}
void clockShiftRegister () {
  shiftRegister = shiftRegister << 1;
  boolean inputBit = digitalRead(inputPin) ;
  shiftRegister |= inputBit;
}

void loop() {

  byte output = B00000000;
  bitWrite(output, 0,  bitRead(shiftRegister, Q0));
  bitWrite(output, 3, bitRead(shiftRegister, Q1));
  bitWrite(output, 4, bitRead(shiftRegister, Q2));  //You can go as high as 15 for an int
  
  PORTB = output;

}
