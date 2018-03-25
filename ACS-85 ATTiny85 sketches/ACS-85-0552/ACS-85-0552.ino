
/**
 * ACS-85-0552
 * ATTiny85  Tri note melody maker upper - two
 *
 * Plays notes randomly on pins 5, 6, 7
 * Outputs are squarewave so no filtering needed.
 *
 * Suggested Serving: Sum them with resistors or gate em out.
 *
 * Plays 3 tones at the same time with one of three pins
 * going to the next note round robin like.
 *
 * One input for speed.
 *
 * I found that using byte counters really cuts down the clock cycles
 * in the interrupt.  However, the trade off is less resolution
 * in the counter.  The reason for this is that we are working on an
 * 8 bit chip.  Using a 2 btye varible takes extra time to load up 
 * the variable.  Make the variables you pas to your interrupts as 
 * small as possible.
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 speed
 * External pin 3 (PB4) = nc
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 output
 * External pin 6 (PB1) = Output 1 output
 * External pin 7 (PB2) = Output 2 output
 * External pin 8       = Vcc
 *
 *
 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 * but it really should not be too hard to adapt.

 TODO - Clean this up...why is there a LFSR in here. ITs not used?
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */


//  ATTiny85 overview
//                      +-\/-+
//               Reset 1|    |8  VCC
// (pin3) in 0 A3  PB3 2| AT |7  PB2 (pin2) out 2
//   (pin4) nc A2  PB4 3| 85 |6  PB1 (pin1) out 1
//                 GND 4|    |5  PB0 (pin0) out 0
//                      ------


//Not used...but these are rough guides of values you should be hitting
#define VCO1_HIGH 32
#define VCO1_LOW 255

//counters for the frequencies
volatile byte oscFreq1 = 200;
volatile byte oscCounter1 = 0;
volatile byte oscFreq2 = 210;
volatile byte oscCounter2 = 0;
volatile byte oscFreq3 = 220;
volatile byte oscCounter3 = 0;

volatile byte oscIncr1 = 1;
volatile byte oscIncr2 = 1;
volatile byte oscIncr3 = 1;
 
unsigned long prevSample1 = 0;

unsigned int lfsr  = 1;


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  TCCR1 = _BV(CTC1) | _BV(CS10) ; // Start timer, ctc mode
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 99;                //set the compare value
  OCR1C = 99;

  TIMSK |= (1 << OCIE1A); //interrupt on Compare Match A  /works with timer

  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  //Count up and toggle portB bits
  if (oscCounter1 > oscFreq1) {
    oscCounter1 = 0;
    PORTB ^= (_BV(PB0));
  }
  oscCounter1 = oscCounter1+ oscIncr1;

  if (oscCounter2 > oscFreq2) {
    oscCounter2 = 0;
    PORTB ^= (_BV(PB1));
  }
  oscCounter2 = oscCounter2 + oscIncr2;

  if (oscCounter3 > oscFreq3) {
    oscCounter3 = 0;
    PORTB ^= (_BV(PB2));
  }
  oscCounter3 = oscCounter3 + oscIncr3;

}


void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

void clockCounter()      // called by interrupt
{
  clockLfsr ();
  clockLfsr ();
  clockLfsr ();
  clockLfsr ();
}




byte getAction () {

  //clockCounter();
  byte a =  lfsr & B00000111;
  return a;
}


int cycle = 0;
byte note = 0;

byte getNote  () {
  note++;
  if (note > 4){
    note=0;
  }

 
  return note;
  
}

void loop() {

  if (lfsr == 0) {
    // should not happen..but just in case.
    lfsr = 1;
  }

  //Read freq
  int osc1_t = analogRead(A3);
  int  loopSpeed = map(osc1_t, 0, 1023, 500, 20);

  
  unsigned long millsSample = millis();

  //Sample every 'loopspeed' ms to get new freqs
  if ((millsSample - prevSample1) > loopSpeed ) {
    prevSample1 = millsSample;


    byte action = getAction();
    /**
     * This is simple get a new freq every third time.  It will round robin.
     * You can add more cycles to hold notes or get double different sounding
     * things
     */
    if (cycle == 0) {
      oscFreq1 = oscFreq1 + 5;
    }
    if (cycle == 1) {
      oscFreq2 = oscFreq2 - 7;
    }

    if (cycle == 2) {
      oscFreq3 = oscFreq3 + 3;
    }

    if (cycle == 3) {
      
      oscIncr1 = getNote();
    }
    if (cycle == 4) {
      oscIncr2 = getNote();
    }

    if (cycle == 5) {
      oscIncr3 = getNote();
    }

    

    cycle++;
    if (cycle > 6) {
      cycle = 0;
    }




  }



}
