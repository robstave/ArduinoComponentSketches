
#include            <avr/io.h>                                                                                                                                                                                                                                                                                                                                                                         /*            attiny            register            names            */
#include            <avr/interrupt.h>


//  ATTiny overview
//                           +-\/-+
//                   Vcc    1|    |14  GND
//                   PB0  2|    |13  PA0
//                   PB1  3|    |12  PA1
//               Reset PB3  4|    |11  PA2
//                   PB2  5|    |10  PA3
//                   PA7  6|    |9   PA4  HF
//                 PA6  7|    |8   PA5  HF

//Counters in the interrupt to toggle pins.
// Increasing the number reduces the frequency.
 
// the setup function runs once when you press reset or power the board
void setup() {

  DDRA = B00000000; // set PORTA
  DDRB = B00000111; // set PORTB outputs
PORTA = 0xFF; //enable all pull ups
  //DDRB = 0x3F;  could use this too
   /*
    pinMode(0, INPUT);
    pinMode(1, INPUT);
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
   */

}






void loop() {
  byte inputPortB = PINA;


  boolean outputBit1 = bitRead(inputPortB, 0) ^ bitRead(inputPortB, 1) ;
  boolean outputBit2 = outputBit1 ^ bitRead(inputPortB, 2);

  boolean outputBit3 = outputBit2 ^ bitRead(inputPortB, 3)
                       ^ bitRead(inputPortB, 4) ^ bitRead(inputPortB, 5)
                       ^ bitRead(inputPortB, 6) ^ bitRead(inputPortB, 7);
/*

boolean outputBit1 =  bitRead(inputPortB, 0);
boolean outputBit2 = bitRead(inputPortB, 1);
boolean outputBit3 = bitRead(inputPortB, 2);
*/
  byte output = B00000000;
  bitWrite(output, 0, outputBit1);
  bitWrite(output, 1, outputBit2);
  bitWrite(output, 2, outputBit3);
  
  PORTB = output;
}
