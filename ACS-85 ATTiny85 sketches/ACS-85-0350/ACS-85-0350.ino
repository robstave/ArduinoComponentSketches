/**
   ACS-85-0350
   ATTiny85 hamonizer

   Samples the squarewave on pin 7 and outputs harmonized signals

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = 3rd maj
   External pin 3 (PB4) = 4th up
   External pin 4       = GND
   External pin 5 (PB0) = min third
   External pin 6 (PB1) = 5th below
   External pin 7 (PB2) = Clock
   External pin 8       = Vcc

   V 1.0  -  First Version
   V 1.1  -  count down optimizations

   Note: This sketch has been written specifically for ATTINY85 and not Arduino uno


   Possible improvements:
   - store off the diff that the loop uses as a prev diff and skip
     math if nothing has changed

   - tighten interrupt.  You can decrease OCR0A to a point where
   it starts to crackle...and turn it back up a bit.  Since internal
   clocks are different, you would do this on a chip per chip basis.
   This would give you a little more resolution.

   Or get better resolution with ATTiny84 so you have pins for the Xtal
   and up your speed.


   Rob Stave (Rob the fiddler) CCBY 2015
*/


//  ATTiny overview
//                   +-\/-+
//            Reset 1|    |8  VCC
// (pin3) maj3  PB3 2|    |7  PB2 (pin2/int0) CLOCK
// (pin4) 4th   PB4 3|    |6  PB1 (pin1) 5th
//              GND 4|    |5  PB0 (pin0) min3
//                   ------

const int int0 = 0;  // interrupt 0


volatile int oscFreq1    = 200;
volatile int oscCounter1 = 0;
volatile int oscFreq2    = 200;
volatile int oscCounter2 = 0;
volatile int oscCounter3 = 0;
volatile int oscFreq3    = 200;
volatile int oscCounter4 = 0;

void setup()
{

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode Table 11.5
  OCR0A = 145; //CTC Compare value
  TCCR0B |= (1 << CS00); // Prescaler =8 Table 11.6

  // TCCR0A |=(1<<COM0A1); //Timer0 in toggle mode Table 11.2
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6


  interrupts();             // enable all interrupts


  DDRB = B00011011; // set PORTB to outputs except PB2 for the clock

  attachInterrupt(int0, clockCounter, CHANGE);
  //  clockInt is our interrupt, clockCounter function is called when
  //  invoked on either clock edge

}

volatile unsigned long counter = 0;
volatile unsigned long lastCounter = 0;
volatile unsigned long diff = 0;

/**
   Clock pin Interrupt.
   Sample the counter and determine the elapsed time.
*/
void clockCounter()      // called by interrupt
{
  diff =  (diff + (counter - lastCounter)) >> 1;
  lastCounter = counter;
}



ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{

  //Timer interrupt counter
  counter++;

  //OUTPUT ->
  //Count up and toggle portB bits
  if (oscCounter1 <= 0 ) {
    oscCounter1 = oscFreq1;

    //Toggle PB0
    PORTB ^= (_BV(PB0));
  }
  oscCounter1--;

  //OUTPUT ->
  //Count up and toggle portB bits
  if (oscCounter2 <= 0 ) {
    oscCounter2 = oscFreq2;

    //Toggle PB1
    PORTB ^= (_BV(PB1));
  }
  oscCounter2--;

  //OUTPUT ->
  //4th
  if (oscCounter3 <= 0 ) {
    oscCounter3 = oscFreq3;

    //Toggle PB3
    PORTB ^= (_BV(PB3));
  }
  oscCounter3--;

  //OUTPUT ->
  if (oscCounter4 <= 0  ) {
    oscCounter4 = oscFreq2 ;

    //Toggle PB4
    PORTB ^= (_BV(PB4));
  }


  //Note Im using an existing oscFreq2 but incrementing by 2, effectively
  //going an octave up
  oscCounter4 = oscCounter4 - 2;
}

void loop()
{
  // loop d loop

  //Consider faster operations. Shifting for 2,4.  Is x+x+x faster that 3*x ?

  //A minor third, in just intonation, corresponds to a pitch ratio of 6:5
  //flip the ratio as a higher freq is achieved by decreasing the counter
  int mThird = (5l * diff) / 6l;
  oscFreq1 = mThird+1;

  //this is fifth below
  int fifth = (3l * diff) / 2l;
  oscFreq2 = fifth+1;

  // A major third in just intonation corresponds to a pitch ratio of 5:4
  int mjThird = (4l * diff) / 5l;
  oscFreq3 = mjThird;

  //note, the fourth above is achieved by halving the count of the fifth
  //thats done in the loop

}
