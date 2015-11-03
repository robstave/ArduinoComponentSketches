
/**
 * ACS-85-0550
 * ATTiny85  Tri note melody maker uper
 *
 * Plays notes randomly on pins 5, 6, 7
 * Sum them with resistors or gate em out.
 * 
 * Plays 3 tones at the same time with one of three pins
 * going to the next note round robin like.
 *
 * Input one controls the speed
 * 
 * I found that using byte counters really cuts down the clock cycles
 * in the interrupt.  However, the trade off is less resolution
 * in the counter.  I suppose that 255 different notes is nothing to complain
 * about.  Anything below 32 is a pretty high note, so I shift it out if needed.
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
 *
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 * Observations.
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) out 2
//      (pin4) none A2  PB4 3|    |6  PB1 (pin1) out 1
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------


//Not used...but these are rough guides of values you should be hitting
#define VCO1_HIGH 32
#define VCO1_LOW 255

/**
 * the notes we play are based on how long the counters loop.
 * The above defines are not really used, just to kinda give you a feel.
 * The real range is 
 * 2^n to 255 so 
 * 5 => 32 to 255
 * Lower this number to get higher notes
 */
#define HIGHEST_FACTOR 5


//counters for the frequencies
byte oscFreq1 = 200;
byte  oscCounter1 = 0;
byte  oscFreq2 = 210;
byte  oscCounter2 = 0;
byte  oscFreq3 = 220;
byte  oscCounter3 = 0;


int cycle = 0;
int loopSpeed = 3000;
int prevSample1 = 0;


unsigned int lfsr  = 1;

boolean getNewFreq1 = LOW;
boolean getNewFreq2 = LOW;
boolean getNewFreq3 = LOW;

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  TCCR1 = _BV(CTC1) | _BV(CS10) ; // Start timer, ctc mode
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 79;                //set the compare value
  OCR1C = 79;

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
  oscCounter1++;

  if (oscCounter2 > oscFreq2) {
    oscCounter2 = 0;
    PORTB ^= (_BV(PB1));
  }
  oscCounter2++;

  if (oscCounter3 > oscFreq3) {
    oscCounter3 = 0;
    PORTB ^= (_BV(PB2));
  }
  oscCounter3++;

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


/**
 * Gets a counter value to use.  If its > 32, tweek it a bit.
 */
byte getValue () {

  clockCounter();
  byte c1 = lfsr & B11111111;
  if (c1 < 32) {
    c1 = c1 << 5;
  }
  return c1;
}


void loop() {

  //Read freq
  int osc1_t = analogRead(A3);
  loopSpeed = map(osc1_t, 0, 1023, 1000, 33);


  int millsSample = millis();

  //Sample every xms to get new freqs
  if ((millsSample - prevSample1) > loopSpeed ) {
    prevSample1 = millsSample;

    /**
     * This is simple get a new freq every third time.  It will round robin.
     * You can add more cycles to hold notes or get double different sounding
     * things
     */
    if (cycle == 0) {
      getNewFreq1 = HIGH;
    }
    if (cycle == 1) {
      getNewFreq2 = HIGH;
    }

    if (cycle == 2) {
      getNewFreq3 = HIGH;
    }
    cycle++;
    if (cycle > 2) {
      cycle = 0;
    }
  }

  //fetch new freq if getNew flag is high
  if (getNewFreq1 == HIGH) {
    oscFreq1 = getValue ();
    getNewFreq1 = LOW;
  }

  if (getNewFreq2 == HIGH) {
    oscFreq2 = getValue ();
    getNewFreq2 = LOW;
  }

  if (getNewFreq3 == HIGH) {
    oscFreq3 = getValue ();
    getNewFreq3 = LOW;
  }

}
