

/**
 * ACS-85-0351 
 * ATTiny85 sample and detune
 *
 * Samples the squarewave on pin 7 and output
 * a squarewave that is above or below that note on two pins
 *
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = tune0
 * External pin 3 (PB4) = tune0
 * External pin 4       = GND
 * External pin 5 (PB0) = out 0
 * External pin 6 (PB1) = out 1
 * External pin 7 (PB2) = Clock
 * External pin 8       = Vcc

 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * TODO tighten the CTC time a bit for better resolution.
 *
 * Rob Stave (Rob the fiddler) CCBY 2015
 */


//  ATTiny overview
//                          +-\/-+
//                   Reset 1|    |8  VCC
//      (pin3) tune 0  PB3 2|    |7  PB2 (pin2/int0) CLOCK
//      (pin4) tune 1  PB4 3|    |6  PB1 (pin1) out 1
//                     GND 4|    |5  PB0 (pin0) out 0
//                          ------


#define DETUNELOW1 -20
#define DETUNEHIGH1 20

#define DETUNELOW2 -40
#define DETUNEHIGH2 40


const int int0 = 0;  // interrupt 0


volatile int oscFreq1 = 200;
volatile int  oscCounter1 = 0;

volatile int oscFreq2 = 200;
volatile int  oscCounter2 = 0;
 
unsigned long counter = 0;
unsigned long lastCounter = 0;

unsigned long diff = 0;


void setup()
{

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode Table 11.5

  OCR0A = 133; //CTC Compare value

  TCCR0B |= (1 << CS00); // Prescaler =8 Table 11.6

  // TCCR0A |=(1<<COM0A1); //Timer0 in toggle mode Table 11.2
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6
  interrupts();             // enable all interrupts


  DDRB = B00000011; // set PORTB to outputs except PB2 for the clock

  attachInterrupt(int0, clockCounter, CHANGE);
  //  clockInt is our interrupt, clockCounter function is called when
  //  invoked on either clock edge

}



/**
 * Get the difference save the previous count
 * 
 * Does a bit of a rolling average.  you could just do
 * diff = counter - lastCounter too
 */
void clockCounter()      // called by interrupt
{
  diff =  (diff +  (counter - lastCounter))>>1;
  lastCounter = counter;
}



ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{
  //clock main counter
  counter++;


  //Count up and toggle portB bits
  if (oscCounter1 > oscFreq1) {
    oscCounter1 = 0;
    PORTB ^= (_BV(PB0));
  }
  oscCounter1++;


  //Count up and toggle portB bits
  if (oscCounter2 > oscFreq2) {
    oscCounter2 = 0;
    PORTB ^= (_BV(PB1));
  }
  oscCounter2++;

}



void loop()
{

  int sample = analogRead(A3);
  int detune_1 = map(sample, 0, 1023, DETUNELOW1 ,  DETUNEHIGH1);
  sample = analogRead(A2);
  int detune_2 = map(sample, 0, 1023, DETUNELOW2,  DETUNEHIGH2);


  // loop d loop

  oscFreq1 = diff + detune_1;
  oscFreq2 = diff + detune_2;

}
