
/**
 * ACS-85-0512
 * ATTiny85  Shift Register
 *
 * Long Shift register
 * These are used in lunettas outputed to r2r ladders sometimes.
 * One bit delay perhaps.
 *
 * ACS-85-0512 does this with a much larger register
 *
 *
 * V 1.0  -  First Version
 *
 * Rob Stave (Rob the fiddler) CCBY 2015
 */


//  ATTiny overview
//                               +-\/-+
//                       Reset  1|    |8  VCC
//   Shift out 95  (pin3) PB3   2|    |7   PB2 (pin2) Clock
//       Shift 127 (pin4) PB4   3|    |6   PB1 (pin1) Shift In
//                         GND  4|    |5   PB0 (pin0) Shift out 63

//Byte used as the LFSR
unsigned long shiftRegister1  = 0; //32 bits
unsigned long shiftRegister2  = 0;
unsigned long shiftRegister3  = 0;
unsigned long shiftRegister4  = 0;

const int clockInt = 0;  // digital pin 2 is now interrupt 0
const int inputPin = 1;

void setup() {
  DDRB = B00011001; // set PORTB outputs
  attachInterrupt(clockInt, clockCounter, RISING);
}


void clockCounter()      // called by interrupt
{
  clockShiftRegisters ();
}
void clockShiftRegisters () {

  boolean inbit = false;

  //Shift reg4 and push in last bit from reg 3
  shiftRegister4 = shiftRegister4 << 1;
  inbit = bitRead(shiftRegister3, 31);
  shiftRegister4 |= inbit;

  //Shift reg3 and push in last bit from reg 2
  shiftRegister3 = shiftRegister3 << 1;
  inbit = bitRead(shiftRegister2, 31);
  shiftRegister3 |= inbit;

  //Shift reg2 and push in last bit from reg 1
  shiftRegister2 = shiftRegister2 << 1;
  inbit = bitRead(shiftRegister1, 31);
  shiftRegister2 |= inbit;

   //Shift reg1 and push in input bit
  shiftRegister1 = shiftRegister1 << 1;
  boolean inputBit = digitalRead(inputPin) ;
  shiftRegister1 |= inputBit;


}

void loop() {

  //Adjust reads as needed for shift register taps

  byte output = B00000000;
  bitWrite(output, 0,  bitRead(shiftRegister2, 31));
  bitWrite(output, 3, bitRead(shiftRegister3, 31));
  bitWrite(output, 4, bitRead(shiftRegister4, 31));

  PORTB = output;

}
