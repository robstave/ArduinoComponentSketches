
/**
 * ACS-85-0308
 * ATTiny85 Pick a gate
 *
 * Select and/or/nand/xor
 * 2 inputs and 1 outputs.
 * (and two selection bits)
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = select bit0
 * External pin 3 (PB4) = select bit1
 * External pin 4       = GND
 * External pin 5 (PB0) = input a  
 * External pin 6 (PB1) = input b
 * External pin 7 (PB2) = output
 * External pin 8       = Vcc
 * 
 * s0 s1  A B  out 
 * 0  0   0 0  0   AND
 * 0  0   0 1  0
 * 0  0   1 0  0
 * 0  0   1 1  1
 * 
 * 0  1   0 0  0  OR
 * 0  1   0 1  1
 * 0  1   1 0  1
 * 0  1   1 1  1
 * 
 * 1  0   0 0  1  NAND
 * 1  0   0 1  1
 * 1  0   1 0  1
 * 1  0   1 1  0
 * 
 * 1  1   0 0  0 XOR
 * 1  1   0 1  1
 * 1  1   1 0  1
 * 1  1   1 1  0
 *
 *
 * V 1.0  -  First Version
 * 
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * Rob Stave (Rob the fiddler) 2015
 */


//  ATTiny overview
//                      +-\/-+
//               Reset 1|    |8  VCC
// (pin3) select0  PB3 2|    |7  PB2 (pin2) OUT
// (pin4) select1  PB4 3|    |6  PB1 (pin1) IN A
//                 GND 4|    |5  PB0 (pin0) In B

void setup() {
  DDRB = B00000100; // set PORTB outputs
  PORTB = B00000000; //enable all pull downs
}
 
void loop() {

  byte inputPortB = PINB;

  //TODO...can hold off on these first reads

  //Get selection bits from value read from port b
  boolean bit2 =bitRead(inputPortB, 3);
  boolean bit1 =bitRead(inputPortB, 4);

  //Get A and B from value read from port b  
  boolean a = bitRead(inputPortB, 0);
  boolean b = bitRead(inputPortB, 1);
  
  boolean outputValue;
 
  if ((bit1==LOW)&&(bit2==LOW)) {
     outputValue  = a&b ;
  } 
  if ((bit1==LOW)&&(bit2==HIGH)) {
     outputValue  = a|b ;
  }
  if ((bit1==HIGH)&&(bit2==LOW)) {
     outputValue  = !(a&b) ;
  }
  if ((bit1==HIGH)&&(bit2==HIGH)) {
     outputValue  = a^b ;
  }
  
  //Output results to port b
  byte output = B00000000;
  bitWrite(output, 2, outputValue);
  PORTB = output;
  
}
