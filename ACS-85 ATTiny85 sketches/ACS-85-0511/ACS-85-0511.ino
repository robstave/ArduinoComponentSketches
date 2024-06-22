
/**
 * ACS-85-0511
 * ATTiny85  Shift Register - Variable position shift register
 * 32 bit shift register, and an analog in to select the
 * output bit.
 *
 * External pin 1 = Reset
 * External pin 2 = Analog Read (selects bit 0 - 15)
 * External pin 3 = Out0
 * External pin 4 = GND
 * External pin 5 = Selected bit out
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
//                        Reset 1|    |8  VCC
//   Analog Read select A3 PB3  2|    |7   PB2 (pin2) Clock
//                   out0  PB4  3|    |6   PB1 (pin1) Shift In
//                         GND  4|    |5   PB0 (pin0) Select out

//Byte used as the Shift Register
unsigned long shiftRegister  = 0;
const int clockInt = 0;  // digital pin 2 is now interrupt 0
const int inputPin = 1;

#define ANALOG_READ A3        //Analog read

//There is an extra pin.  I choose to output the 0th bit,
//but you can pick a number up to 31 here
#define Q0 0

void setup() {
  DDRB = B00010001;  //could use this
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


  int osc1_t = analogRead(ANALOG_READ);
  int selectBit = map(osc1_t, 0, 1023, 0,  31);


  byte output = B00000000;
  bitWrite(output, 0,  bitRead(shiftRegister, selectBit));
  bitWrite(output, 4, bitRead(shiftRegister, Q0));

  PORTB = output;

}
