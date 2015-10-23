
/**
 * ACS-85-0530
 * ATTiny85  Drunk walk
 *
 * Pin 0 does a tone based on a kinda drunk walk algorithm that makes
 * the frequncy randomly move up or down.
 *
 * Speed: step speed of the walk
 * Skew: a value that skews the probability of the walk up or down.
 * doSkew:
 *   False: walk is random and goes up or down. If the boundry is hit, it starts back in in the middle
 *   True: adds a % probability to go up or down.
 *
 *
 * doStagger:
 *   False:  increments walk +/-1
 *   True:   increments walk +/- max 5
 *
 *   Random value is found with the lfsr.  the bottom 7 bits are used to find a number
 *   between 0 and 127.
 *   Generally, if the value is above or below 64, the walk goes up or down.
 *   Skew tweeks that boundry and stagger sets the step size
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 Speed
 * External pin 3 (PB4) = input doSkew
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 output
 * External pin 6 (PB1) = input doStagger
 * External pin 7 (PB2) = input 2 Skew
 * External pin 8       = Vcc
 *
 * V 1.0  -  First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) skew A3  PB3 2|    |7  PB2 (pin2) A1 Speed
//      (pin4) doSkew   PB4 3|    |6  PB1 (pin1) doStagger
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.

//High low and middle freq
#define FREQ_HIGH 7
#define FREQ_LOW  155
#define FREQ_START 88

//Step speed
#define SPEED_HIGH 500
#define SPEED_LOW 4000

//Compare bottom 7 bits to Center to determine up or down
//Add skew to..well...skew
#define SKEW_BOTTOM -30
#define SKEW_TOP 30
#define CENTER 64


//Byte used as the LFSR
unsigned int lfsr  = 1;

//Counter and freq for what you hear
int oscFreq1 = 50;
int oscCounter1 = 0;

int stagger = 1;

//Counter for the steps
int stepSpeed = 1000;
int stepCounter = 0;

//Counter for doSkew and doStagger
//Set to above 1000 to force an intial read
#define SETUP_LOOP_READ 301
int setupCounter = SETUP_LOOP_READ + 10;



// up or down skew  only applicable if doSkew is high
int skew = 0;
boolean doSkew = LOW;
boolean doStagger = LOW;

//step size
#define MAX_STAGGER 5

//Counter to throttle the read speed
int readSpeedCount = 0;



// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000001;

  // initialize timer1
  noInterrupts();           // disable all interrupts

  //Timer1 init
  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 100;                //set the compare value
  OCR1C = 100;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A
  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, ctc mode, prescaler clk/2

  //Timer0 init
  TCCR0A = 0;
  TCCR0B = 0;
  TCCR0A |= (1 << WGM01); //Start timer 1 in CTC mode Table 11.5
  OCR0A = 5; //CTC Compare value
  TCCR0B |= (1 << CS00) | (1 << CS01); // Prescaler =64 Table 11.6
  // TCCR0A |=(1<<COM0A1); //Timer0 in toggle mode Table 11.2
  TIMSK |= (1 << OCIE0A); //Enable CTC interrupt see 13.3.6
  interrupts();             // enable all interrupts
}

ISR(TIMER0_COMPA_vect)          // timer compare interrupt service routine
{

  //Count up and toggle portB bits
  if (stepCounter > stepSpeed) {
    stepCounter = 0;

    clockLfsr ();
    clockLfsr ();
    clockLfsr ();
    clockLfsr ();
    clockLfsr ();
    clockLfsr ();
    clockLfsr ();
    clockLfsr ();

    //Mask off the state
    byte reg = lfsr & B01111111;
    // this is a number from 0 to 128

    int compare = CENTER;

    //if we are skewing the number...add it
    if (doSkew == HIGH) {
      compare = CENTER + skew;
    }

    if (doStagger == HIGH) {
      stagger++;
      if (stagger > MAX_STAGGER) {
        stagger = 1;
      }
    }
    else {
      stagger = 1;
    }



    if (reg > compare) {
      oscFreq1 = oscFreq1 + stagger;
    } else {
      oscFreq1 = oscFreq1 - stagger ;
    }




    if (doSkew == HIGH) {
      if (oscFreq1 < FREQ_HIGH) {
        oscFreq1 = FREQ_LOW - 1;
        oscCounter1 = 0;
      }

      if (oscFreq1 > FREQ_LOW) {
        oscFreq1 = FREQ_HIGH + 1;
        oscCounter1 = 0;
      }
    } else {
      if ((oscFreq1 <= FREQ_HIGH) || (oscFreq1 >= FREQ_LOW)) {
        oscFreq1 = FREQ_START;
        oscCounter1 = 0;
      }
    }


  }
  stepCounter++;





}
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  //Count up and toggle portB bits
  if (oscCounter1 > oscFreq1) {
    oscCounter1 = 0;
    PORTB ^= (_BV(PB0));
  }
  oscCounter1++;


}

/**
 * Clock the LFSR one cycle.
 */
void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}



void loop() {

  //We do not read these values as often
  if (setupCounter > SETUP_LOOP_READ) {
    setupCounter = 0;
    doSkew = digitalRead(4);
    doStagger = digitalRead(1);
  }
  setupCounter++;

  //Read the speed for each note
  if (readSpeedCount > 20) {
    readSpeedCount = 0;
    int speed_t = analogRead(A3);
    stepSpeed = map(speed_t, 0, 1023, SPEED_HIGH,  SPEED_LOW);
  }
  readSpeedCount++;


  if (doSkew == HIGH) {
    int skew_t = analogRead(A1);
    skew = map(skew_t, 0, 1023, SKEW_BOTTOM,  SKEW_TOP);
  }

}
