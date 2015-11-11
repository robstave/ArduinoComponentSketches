
/**
 * ACS-85-0600
 * ATTiny85  Simple One pin Sequencer
 *
 * Is it an LFO or a sequencer?
 * Patterns are programmed as arrays in the code and an analog pot can be used to
 * select scenes.  The included scenes are not super interesting, just a starting point.
 *
 * The patterns can be programmed by
 * changing the values in the arrays.
 *
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 freq
 * External pin 3 (PB4) = input 1 scene select
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 output
 * External pin 6 (PB1) = clock
 * External pin 7 (PB2) = nc
 * External pin 8       = Vcc
 *
 * V 1.0  -  First Version
 * Any further extensions to this will be treated as a sequencer.
 *
 * Rob Stave (Rob the fiddler) ccby 2015
 */



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) nc
//      (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) clock
//                      GND 4|    |5  PB0 (pin0) out 0
//                           ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.


#define VCO1_L1_HIGH 400
#define VCO1_L1_LOW 8000

//Use this to tweek the feel of the pot.  If you are using an audio pot versus a linear, set this
//value to 0
#define MAP_VALUES_AS_LINEAR 0

#if MAP_VALUES_AS_LINEAR == 1
  #define STATE0 10
  #define STATE1 20
  #define STATE2 30
  #define STATE3 40
#else
  #define STATE0 5
  #define STATE1 10
  #define STATE2 20
  #define STATE3 40
#endif


//counters for the frequencies

volatile int oscCounter1 = 0;
volatile int oscFreq1 = 0;


int patternCount = 0;
volatile int pattern = 0;


#define PATTERN_0_SIZE 8
boolean  pattern_0[PATTERN_0_SIZE] = {HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW};

#define PATTERN_1_SIZE 8
boolean  pattern_1[PATTERN_1_SIZE] = { HIGH, HIGH, LOW, LOW, HIGH, LOW, HIGH, LOW};

#define PATTERN_2_SIZE 6
boolean  pattern_2[PATTERN_2_SIZE] = { HIGH, HIGH, LOW, HIGH, HIGH, LOW};

#define PATTERN_3_SIZE 5
boolean  pattern_3[PATTERN_3_SIZE] = { HIGH, LOW, LOW, HIGH,  LOW};


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer1
  noInterrupts();           // disable all interrupts

  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  //GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 99;                //set the compare value
  OCR1C = 99;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A

  TCCR1 = _BV(CTC1)  | _BV(CS11) | _BV(CS12); // Start timer, ctc mode, prescaler clk/2

  interrupts();             // enable all interrupts

}

boolean getPatternValue () {
  boolean result = LOW;

  if (pattern == 0) {
    if (patternCount >= PATTERN_0_SIZE) {
      patternCount = 0;
     }
      result = pattern_0[patternCount];
   
  }

  
  if (pattern == 1) {
    if (patternCount >= PATTERN_1_SIZE) {
      patternCount = 0;
    }
    result = pattern_1[patternCount];
  }

  if (pattern == 2) {
    if (patternCount >= PATTERN_2_SIZE) {
      patternCount = 2;
    }
    result = pattern_2[patternCount];
  }

  if (pattern == 3) {
    if (patternCount >= PATTERN_3_SIZE) {
      patternCount = 0;
    }
    result = pattern_3[patternCount];
  }

  patternCount++;
  return result;
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  if (oscCounter1 >= oscFreq1) {
    oscCounter1 = 0;

    PORTB ^= (_BV(PB1));
    boolean v = getPatternValue();
    if (v == HIGH) {
      PORTB |= _BV(PB0);
    } else {
      PORTB &= ~_BV(PB0);
    }
  }

  oscCounter1++;
  return;
}

int mapPattern (int sample) {

  if (sample <= STATE0) {
    return 0;
  } else if (sample <= STATE1) {
    return 1;
  } else if (sample <= STATE2) {
    return 2;
  } else if (sample <= STATE3) {
    return 3;
  }
  return 0;
}

void loop() {

  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_L1_LOW,  VCO1_L1_HIGH);

  int pattern_t = analogRead(A2);
  pattern =  mapPattern(map(pattern_t, 0, 1023, 0,  40));
}
