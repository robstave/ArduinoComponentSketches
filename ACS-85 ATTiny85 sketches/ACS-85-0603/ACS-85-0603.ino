/**
   ACS-85-0603
   ATTiny85  2 Channel drum patterns with randomness

   Simple Drum triggers

   The patterns can be programmed by
   changing the values in the arrays.
   There is a tempo and a pattern select as well as a randomness value that randomly inverses what you would expect.
   The last pattern is blank, so that can be used with the random to create fairly unique beats.


   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq
   External pin 3 (PB4) = input 1 scene select
   External pin 4       = GND
   External pin 5 (PB0) = output 0
   External pin 6 (PB1) = output 1
   External pin 7 (PB2) = input 2 randomness
   External pin 8       = Vcc

   V 1.0  -  First Version
   Any further extensions to this will be treated as a sequencer.

   Rob Stave (Rob the fiddler) ccby 2015
*/



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) freq A3  PB3 2|    |7  PB2 (pin2) output 2
//     (pin4) scene A2  PB4 3|    |6  PB1 (pin1) output 1
//                      GND 4|    |5  PB0 (pin0) input 2 rand
//                           ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.


//  Max value of the randomness.  A value that is the 30% to 50% of 255 is good
#define MAX_RANDOM 88

// Max and min counter values.  To allow a faster tempo, decrease HIGH
// to allow a slower tempo, increase (longer counter ) low.
#define VCO1_L1_HIGH 70
#define VCO1_L1_LOW 500


// pick this number to be less than HIGH.  SO like if HIGH = 200, pick 20 or so.
// This is the length of the TRIGGER.  Really, you only need to mess with this if you are
// messing with the timers. So if THIS range was 3000 to 200 and you changed the timers to clock
// slower, you might end up adjusting this range to 1000 - 66  and then use a trigger like 10.
//  0 means no trigger...just a GATE value
#define VCO1_TRIGGER  20


//Use this to tweek the feel of the pot.  If you are using an audio pot versus a linear, set this
//value to 0.  Really, ya gotta play with it...or make sure you stick to LINEAR pots when doing this.
#define MAP_VALUES_AS_LINEAR 1
#define MAX_STATE_VALUE 80

#if MAP_VALUES_AS_LINEAR == 1
#define STATE0 10
#define STATE1 20
#define STATE2 30
#define STATE3 40
#define STATE4 50
#define STATE5 60
#define STATE6 70
#define STATE7 80
#else
#define STATE0 5
#define STATE1 10
#define STATE2 15
#define STATE3 25
#define STATE4 35
#define STATE5 40
#define STATE6 60
#define STATE7 80
#endif


//counters for the frequencies

volatile int oscCounter1 = 0;
volatile int oscFreq1 = 0;

volatile int patternCount = 0;
volatile int pattern = 0;
volatile byte randomness = 0;


volatile unsigned int lfsr  = 1;

#define PATTERN_0_SIZE 16
byte  pattern_0[PATTERN_0_SIZE] = {3, 0, 0, 0,
                                   1, 0, 0, 0,
                                   3, 0, 0, 0,
                                   1, 0, 0, 0
                                  };

#define PATTERN_1_SIZE 16
byte  pattern_1[PATTERN_1_SIZE] = {2, 0, 0, 0,
                                   1, 0, 0, 0,
                                   2, 0, 0, 0,
                                   1, 0, 0, 0
                                  };

#define PATTERN_2_SIZE 16
byte  pattern_2[PATTERN_2_SIZE] = {2, 0, 0, 0,
                                   1, 0, 0, 0,
                                   2, 0, 0, 0,
                                   1, 0, 1, 1
                                  };

#define PATTERN_3_SIZE 16
byte  pattern_3[PATTERN_3_SIZE] = {3, 0, 1, 0,
                                   1, 0, 1, 0,
                                   3, 0, 1, 0,
                                   1, 0, 1, 0
                                  };

#define PATTERN_4_SIZE 16
byte  pattern_4[PATTERN_4_SIZE] = {2, 0, 1, 0,
                                   0, 0, 2, 0,
                                   2, 0, 1, 0,
                                   0, 0, 1, 0
                                  };

#define PATTERN_5_SIZE 16
byte  pattern_5[PATTERN_5_SIZE] = {2, 0, 1, 2,
                                   0, 2, 1, 0,
                                   2, 0, 1, 2,
                                   0, 2, 1, 0
                                  };



#define PATTERN_6_SIZE 16
byte  pattern_6[PATTERN_6_SIZE] = {2, 0, 0, 1,
                                   2, 0, 1, 0,
                                   2, 0, 0, 1,
                                   2, 0, 3, 0
                                  };

#define PATTERN_7_SIZE 2
byte  pattern_7[PATTERN_7_SIZE] = {0, 0,  };


// consider https://drumbit.app/ to build em :)

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 33;                //set the compare value
  OCR1C = 33;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1)  | _BV(CS11) | _BV(CS12); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts

}

byte getPatternValue () {
  byte result = B00000000;


  switch (pattern) {
    case 0:
      if (patternCount >= PATTERN_0_SIZE) {
        patternCount = 0;
      }
      result = pattern_0[patternCount];
      break;
    case 1:
      if (patternCount >= PATTERN_1_SIZE) {
        patternCount = 0;
      }
      result = pattern_1[patternCount];
      break;

    case 2:
      if (patternCount >= PATTERN_2_SIZE) {
        patternCount = 0;
      }
      result = pattern_2[patternCount];
      break;

    case 3:
      if (patternCount >= PATTERN_3_SIZE) {
        patternCount = 0;
      }
      result = pattern_3[patternCount];
      break;

    case 4:
      if (patternCount >= PATTERN_4_SIZE) {
        patternCount = 0;
      }
      result = pattern_4[patternCount];
      break;

    case 5:
      if (patternCount >= PATTERN_5_SIZE) {
        patternCount = 0;
      }
      result = pattern_5[patternCount];
      break;

    case 6:
      if (patternCount >= PATTERN_6_SIZE) {
        patternCount = 0;
      }
      result = pattern_6[patternCount];
      break;

    case 7:
      if (patternCount >= PATTERN_7_SIZE) {
        patternCount = 0;
      }
      result = pattern_7[patternCount];
      break;



    default:
      // statements
      break;
  }

 
  patternCount++;


  // get the last two bits
  byte output = result & B00000011;

  // if there is some randomness going...flip based on last two bits of lfsr.  This might end up no flipping at all.
  if (doFlip()) {
    output = output ^ ( lfsr & B00000011);
  }
  return output;
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  if (oscCounter1 >= oscFreq1) {
    oscCounter1 = 0;
    PORTB = getPatternValue();
    clockLfsr ();
  }
  if ( VCO1_TRIGGER > 0 && oscCounter1 == VCO1_TRIGGER ) {
    PORTB = B00000000;
  }

  oscCounter1++;
  return;
}

// We could just map the pot to discrete steps, but with this step, you can accomidate audio pots if thats what you are stuck with.
// if you are building this, I suggest just using linear pots and skipping this step.
int mapPattern (int sample) {

  if (sample <= STATE0) {
    return 0;
  } else if (sample <= STATE1) {
    return 1;
  } else if (sample <= STATE2) {
    return 2;
  } else if (sample <= STATE3) {
    return 3;
  } else if (sample <= STATE4) {
    return 4;
  } else if (sample <= STATE5) {
    return 5;
  } else if (sample <= STATE6) {
    return 6;
  } else if (sample <= STATE7) {
    return 7;
  }
  return 0;
}

void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
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

  //clockCounter();
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

  int pattern_t = analogRead(A2);
  pattern =  mapPattern(map(pattern_t, 0, 1023, 0,  MAX_STATE_VALUE));
  clockLfsr ();

  randomness =  map(analogRead(A1), 0, 1023, 0,  MAX_RANDOM); // pick max to be like 30% to 50% of 256
}
