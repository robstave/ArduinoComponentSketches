/**
   ACS-85-0604
   ATTiny85  3 Channel random triggers

   Simple Drum triggers

  This is really like 603 but just the blank pattern and random triggers filling in

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq
   External pin 3 (PB4) = input 1 randomness
   External pin 4       = GND
   External pin 5 (PB0) = output 0
   External pin 6 (PB1) = output 1
   External pin 7 (PB2) = output 2
   External pin 8       = Vcc

   V 1.0  -  First Version
   Any further extensions to this will be treated as a sequencer.

   Rob Stave (Rob the fiddler) ccby 2019
*/



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) freq A3  PB3 2|    |7  PB2 (pin2) output 2
//         (pin4) rand  PB4 3|    |6  PB1 (pin1) output 1
//                      GND 4|    |5  PB0 (pin0) output 0
//                           ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.


//  Max value of the randomness.  A value that is the 30% to 50% of 255 is good
#define MAX_RANDOM 100

// Max and min counter values.  To allow a faster tempo, decrease HIGH
// to allow a slower tempo, increase (longer counter ) low.
#define VCO1_L1_HIGH 70
#define VCO1_L1_LOW 1400


// pick this number to be less than HIGH.  SO like if HIGH = 200, pick 20 or so.
// This is the length of the TRIGGER.  Really, you only need to mess with this if you are
// messing with the timers. So if THIS range was 3000 to 200 and you changed the timers to clock
// slower, you might end up adjusting this range to 1000 - 66  and then use a trigger like 10.
//  0 means no trigger...just a GATE value
#define VCO1_TRIGGER  20

 

//counters for the frequencies

volatile int oscCounter1 = 0;
volatile int oscFreq1 = 0;

volatile int patternCount = 0;
 
volatile byte randomness = 0;


volatile unsigned int lfsr  = 1;

 
// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 80;                //set the compare value
  OCR1C = 80;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1)  | _BV(CS11) | _BV(CS12); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts

}

byte getPatternValue () {
  byte result = B00000000;
 
  // if there is some randomness going...flip based on last 3 bits of lfsr.  This might end up no flipping at all.
  if (doFlip()) {
    result = result ^ ( lfsr & B00000001);
  }
  if (doFlip()) {
    result = result ^ ( lfsr & B00000010);
  }

  if (doFlip()) {
    result = result ^ ( lfsr & B00000100);
  }
  return result;
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  if (oscCounter1 >= oscFreq1) {
    oscCounter1 = 0;
    PORTB = getPatternValue();
    
  }

  // If you want a GATE instead of a trigger...just delete this block
  
  if ( VCO1_TRIGGER > 0 && oscCounter1 == VCO1_TRIGGER ) {
    // unset trigger
    PORTB = B00000000;
  }
  

  oscCounter1++;
  return;
}
 

void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;

  // check if in a null state
  if (lfsr == 0){
    lfsr = 1;
  }
}

// clock a few times
void clockCounter()      // called by interrupt
{
  clockLfsr ();
  clockLfsr ();
}


/**
 Check if we need to flip a bit or two based on lfsr
*/
boolean doFlip () {

  clockCounter();
  byte c1 = lfsr & B11111111;
  if (c1 < randomness) {
    return true;
  }
  return false;
}




void loop() {
  clockLfsr ();
  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_L1_LOW,  VCO1_L1_HIGH);
 
  clockLfsr ();

  randomness =  map(analogRead(A2), 0, 1023, 0,  MAX_RANDOM); // pick max to be like 30% to 50% of 256
}
