
/**
 * ACS-85-0522
 * ATTiny85  LFSR Varable Unclocked Low Frequency
 * 
 * Note.  This is in the high frequency ranges.
 * To get a lower frequency, swap lines in the setup. 
 * to add CS12 bump the prescaler
 *
 * Can be used to produce random numbers and noise.
 * The LFSR actually clocks 4x for each clock as we
 * are masking the bottom 4 bits from the integer
 * So you can use it to get a pseudo random 4 bit
 * pattern (or less)
 *
 * https://en.wikipedia.org/wiki/Linear_feedback_shift_register
 *
 * Similar to ACS-0520, but the clock is internal and
 * adjustable
 *
 * External pin 1 = Reset
 * External pin 2 = LFSR bit 2
 * External pin 3 = LFSR bit 3
 * External pin 4 = GND
 * External pin 5 = LFSR bit 0
 * External pin 6 = LFSR bit 1
 * External pin 7 = Analog input
 * External pin 8 = VCC
 *
 * V 1.0  -  First Version
 *
 * Rob Stave (Rob the fiddler) 2015 CCBY
 */



//  ATTiny overview
//                               +-\/-+
//                       Reset  1|    |8  VCC
//          LFSR 2 (pin3) PB3   2|    |7   PB2 (pin2) Analog speed
//          LFSR 3 (pin4) PB4   3|    |6   PB1 (pin1) LFSR 1
//                         GND  4|    |5   PB0 (pin0) LFSR 0

//Byte used as the LFSR
unsigned int lfsr  = 1;

#define VAR1 A1         //Analog read
#define VCO_LO 450   //A bigger number means a longer count = lower freq
#define VCO_HIGH 20

int prev = 0;

//counters for the frequencies
int oscFreq = 55; //I just picked a number, it gets written over real quick
int oscCounter = 0;


void setup() {
  DDRB = B00011011;  

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 100;                //set the compare value
  OCR1C = 100;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

 
  //NOTE: to slow this down for a lower frequency...uncomment the below
  //line and comment the above.
   TCCR1 = _BV(CTC1) | _BV(CS11)| _BV(CS12); // Start timer, ctc mode, prescaler clk/8

  interrupts();             // enable all interrupts

}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  //HF code is the same.  Just smaller counters
  if (oscCounter > oscFreq) {
    oscCounter = 0;
    clockLfsr ();
    clockLfsr ();
    clockLfsr ();
    clockLfsr ();
  }
  oscCounter++;

}
void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}
void loop() {


  //read the value from the pin
  int osc1_t = analogRead(VAR1);
  oscFreq = map(osc1_t, 0, 1023, VCO_HIGH,  VCO_LO);

  //Mask off the state
  byte reg = lfsr & B00001111;


  byte output = B00000000;
  bitWrite(output, 0,  bitRead(reg, 0));
  bitWrite(output, 1, bitRead(reg, 1));
  bitWrite(output, 3,  bitRead(reg, 2));
  bitWrite(output, 4, bitRead(reg, 3));

  PORTB = output;


}
