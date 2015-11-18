
/**
 * ACS-85-0601
 * ATTiny85  3 Channel sequencer
 *
 * Simple Drum beats or 3 pin sequencer
 * 
 * The patterns can be programmed by
 * changing the values in the arrays.
 *
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = input 0 freq
 * External pin 3 (PB4) = input 1 scene select
 * External pin 4       = GND
 * External pin 5 (PB0) = output 0 
 * External pin 6 (PB1) = output 1 
 * External pin 7 (PB2) = output 2
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
//      (pin3) freq A3  PB3 2|    |7  PB2 (pin2) output 2
//     (pin4) scene A2  PB4 3|    |6  PB1 (pin1) output 1
//                      GND 4|    |5  PB0 (pin0) output 0 
//                           ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.


#define VCO1_L1_HIGH 200
#define VCO1_L1_LOW 3000

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


#define PATTERN_0_SIZE 16
byte  pattern_0[PATTERN_0_SIZE] = {3, 0, 1, 0, 7, 0, 1, 0, 3, 0, 1, 0, 7, 0, 1, 0};

#define PATTERN_1_SIZE 16
byte  pattern_1[PATTERN_1_SIZE] = {2, 1, 6, 1, 2, 1, 6, 1, 2, 1, 6, 1, 2, 1, 6, 1};

#define PATTERN_2_SIZE 16
byte  pattern_2[PATTERN_2_SIZE] = {3, 0, 7, 0, 7, 0, 3, 0, 3, 0, 1, 0, 1, 0, 7, 0};

#define PATTERN_3_SIZE 16
byte  pattern_3[PATTERN_3_SIZE] = {3, 0, 1, 0, 5, 0, 1, 0, 3, 0, 3, 0, 5, 0, 1, 0};


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

byte getPatternValue () {
  byte result = B00000000;

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

    
   byte output = result & B00000111;
  return output;
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  if (oscCounter1 >= oscFreq1) {
    oscCounter1 = 0;
    PORTB = getPatternValue();
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
