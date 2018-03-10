/**
 * ACS-85-0352
 * ATTiny85 sample and vibrato ramps
 *
 * Samples the squarewave on pin 7 and output
 * a squarewave that is above or below that note
 *
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = freq
 * External pin 3 (PB4) = width
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
 *  TODO
 *  The speed is based on looping through the pots, which takes time.
 *  This uses Timer 0,  Perhaps flip to timer 1 and use delay
 *
 * Rob Stave (Rob the fiddler) CCBY 2015
 */

//  ATTiny overview
//                     +-\/-+
//              Reset 1|    |8  VCC
// (pin3) tune 0  PB3 2|    |7  PB2 (pin2/int0) CLOCK
// (pin4) tune 1  PB4 3|    |6  PB1 (pin1) out 1
//                GND 4|    |5  PB0 (pin0) out 0
//                     ------

#define DETUNEHIGH1 40


//Note the speed here is really just how many times we
//loop through the busywork or reading the pots.
//If you do this on another platform, this will not work well or
//at least need adjustment
#define SPEEDHIGH 1
#define SPEEDLOW 30

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

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 99;                //set the compare value
  OCR1C = 99;
  TCCR1 = _BV(CTC1) | _BV(CS10); // Start timer, ctc mode, prescaler clk/1
  TIMSK |= (1 << OCIE1A); //interrupt on Compare Match A  /works with timer

  


  interrupts();             // enable all interrupts


  DDRB = B00000011; // set PORTB to outputs except PB2 for the clock

  attachInterrupt(int0, clockCounter, CHANGE);
  //  clockInt is our interrupt, clockCounter function is called when
  //  invoked on either clock edge

}

 

void clockCounter()      // called by interrupt
{
  diff =  (diff+(counter - lastCounter))>>1;
  lastCounter = counter;
}



ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
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

int vibrato = 0;
int loopCounter = 0;
int loopSpeed = 20;
int detune_value = 20;

 
void loop()
{
 
  if (loopCounter > 10 ) {
    loopCounter = 0;
    
    int sample = analogRead(A3);
    detune_value = map(sample, 0, 1023, 0 ,  DETUNEHIGH1);
    sample = analogRead(A2);
    loopSpeed = map(sample, 0, 1023, SPEEDHIGH,  SPEEDLOW);
  }

  loopCounter++;
  
  delay(loopSpeed);
  if (vibrato > detune_value) {
    vibrato = 0;
  }
  // loop d loop

  oscFreq1 = diff + vibrato;
  oscFreq2 = diff - vibrato;
  vibrato++;
}
