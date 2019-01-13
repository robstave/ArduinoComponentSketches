
/**
   ACS-85-0550
   ATTiny85  Tri note melody maker upper

   Plays notes randomly on pins 5, 6, 7
   Sum them with resistors or gate em out.

   Plays 3 tones at the same time with one of three pins
   going to the next note round robin like.

   input controls the speed
   bool bananas makes it go crazy

   I found that using byte counters really cuts down the clock cycles
   in the interrupt.  However, the trade off is less resolution
   in the counter.  I suppose that 255 different notes is nothing to complain
   about.  Anything below 32 is a pretty high note, so I shift it out if needed.

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 speed
   External pin 3 (PB4) = bananas
   External pin 4       = GND
   External pin 5 (PB0) = output 0 output
   External pin 6 (PB1) = Output 1 output
   External pin 7 (PB2) = Output 2 output
   External pin 8       = Vcc

   V 1.0  -  First Version
   V 1.1  -  Tweeks

   Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
   Observations.

   Rob Stave (Rob the fiddler) ccby 2015
*/

//  ATTiny overview
//                        +-\/-+
//                 Reset 1|    |8  VCC
// (pin3)      in A3 PB3 2|    |7  PB2 (pin2) out 2
// (pin4) bananas A2 PB4 3|    |6  PB1 (pin1) out 1
//                   GND 4|    |5  PB0 (pin0) out 0
//                        ------


//Not used...but these are rough guides of values you should be hitting
#define VCO1_HIGH 32
#define VCO1_LOW 255

/**
   the notes we play are based on how long the counters loop.
   The above defines are not really used, just to kinda give you a feel.
   The real range is
   2^n to 255 so
   5 => 32 to 255
   Lower this number to get higher notes
*/
#define HIGHEST_FACTOR 5


//counters for the frequencies
volatile byte oscFreq1 = 200;
volatile byte oscCounter1 = 0;
volatile byte oscFreq2 = 210;
volatile byte oscCounter2 = 0;
volatile byte oscFreq3 = 220;
volatile byte oscCounter3 = 0;



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

boolean checkPin4() {
  // Check pin 0
  return ((PINB & 0b00010000) != 0);
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
   Gets a counter value to use.  If its less than 32, tweek it a bit.
*/
byte getValue () {

  clockCounter();
  byte c1 = lfsr & B11111111;
  if (c1 < 32) {
    c1 = c1 << 5;
  }
  return c1;
}

void checkGoBananas(byte cycle) {
  if (checkPin4() == true) {
    //go bananas
    if (cycle == 0) {
      oscFreq1++;
    }
    if (cycle == 1) {
      oscFreq2++;
    }

    if (cycle == 2) {
      oscFreq3++;
    }
  }
}

void loop() {
  int prevSample1 = 0;
  byte  cycle = 0;
  int loopSpeed = 3000;


  while (true) {
    clockCounter();
    //Read freq
    int osc1_t = analogRead(A3);
    loopSpeed = map(osc1_t, 0, 1023, 1000, 33);
    int millsSample = millis();

    checkGoBananas(cycle);

    //Sample every xms to get new freqs
    if ((millsSample - prevSample1) > loopSpeed ) {
      prevSample1 = millsSample;

      /**
         Each time we hit this loop, we change the value of one of
         the frequencies.
      */
      if (cycle == 0) {
        oscFreq1 = getValue ();
      }
      if (cycle == 1) {
        oscFreq2 = getValue ();
      }

      if (cycle == 2) {
        oscFreq3 = getValue ();
      }
      cycle++;
      if (cycle > 2) {
        cycle = 0;
      }




    }
  }

}
