/**
   ACS-85-0660
   ATTiny85  3 tones  with patterns
   Simple Drum triggers
   The patterns can be programmed by
   changing the values in the arrays.
   There is a tempo and a pattern select.

   ITs a simple squarewave that drops in frequency.
   You could add 3 more accumulators and make them cowbells :)

   

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input 0 freq
   External pin 3 (PB4) = input 1 scene select
   External pin 4       = GND
   External pin 5 (PB0) = output 2
   External pin 6 (PB1) = output 1
   External pin 7 (PB2) = output 0
   External pin 8       = Vcc
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2023
*/

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//      (pin3) freq A3  PB3 2|    |7  PB2 (pin2) output 0
//     (pin4) scene A2  PB4 3|    |6  PB1 (pin1) output 1
//                      GND 4|    |5  PB0 (pin0) output 2
//                           ------

//Ranges for the pot.  Technically a small nuumber means a
//shorter timer so low or high...whatever you want to call it.



// Max and min counter values.  To allow a faster tempo, decrease HIGH
// to allow a slower tempo, increase (longer counter ) low.
#define VCO1_L1_HIGH 300
#define VCO1_L1_LOW 2000



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

volatile byte note = 0;

#define PATTERN_0_SIZE 16
byte pattern_0[PATTERN_0_SIZE] = { 1, 0, 0, 0,
                                   3, 0, 0, 0,
                                   1, 0, 0, 0,
                                   7, 0, 0, 0 };

#define PATTERN_1_SIZE 16
byte pattern_1[PATTERN_1_SIZE] = { 2, 0, 4, 0,
                                   1, 0, 4, 0,
                                   2, 0, 4, 0,
                                   1, 0, 4, 0 };

#define PATTERN_2_SIZE 16
byte pattern_2[PATTERN_2_SIZE] = { 2, 0, 0, 0,
                                   1, 0, 0, 0,
                                   2, 0, 0, 0,
                                   1, 0, 1, 1 };

#define PATTERN_3_SIZE 16
byte pattern_3[PATTERN_3_SIZE] = { 3, 0, 1, 0,
                                   1, 0, 1, 0,
                                   3, 0, 1, 0,
                                   1, 0, 1, 0 };

#define PATTERN_4_SIZE 16
byte pattern_4[PATTERN_4_SIZE] = { 2, 0, 1, 0,
                                   0, 0, 2, 0,
                                   2, 0, 1, 0,
                                   0, 0, 1, 0 };

#define PATTERN_5_SIZE 16
byte pattern_5[PATTERN_5_SIZE] = { 2, 0, 1, 2,
                                   0, 2, 1, 0,
                                   2, 0, 1, 2,
                                   0, 2, 1, 0 };

#define PATTERN_6_SIZE 16
byte pattern_6[PATTERN_6_SIZE] = { 2, 0, 0, 1,
                                   2, 0, 1, 0,
                                   2, 0, 0, 1,
                                   2, 0, 3, 0 };

#define PATTERN_7_SIZE 16
byte pattern_7[PATTERN_7_SIZE] = { 1, 0, 2, 1,
                                   2, 4, 1, 0,
                                   2, 1, 4, 1,
                                   2, 0, 3, 5 };

// consider https://drumbit.app/ to build em :)

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  //set output bits

  // initialize timer1
  noInterrupts();  // disable all interrupts

  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;              // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00;  // 1/8 prescale
  TIMSK = 1 << OCIE0A;              // Enable compare match, disable overflow
  OCR0A = 49;                       // Divide by 400

  interrupts();  // enable all interrupts
}


#define ClassicNote1 2721
#define ClassicNote2 1890
#define ClassicNote3 1490


// dds
volatile unsigned int Acc1 = 0;
volatile unsigned int Note1 = ClassicNote1;
volatile unsigned int Acc2 = 0;
volatile unsigned int Note2 = ClassicNote2;
volatile unsigned int Acc3 = 0;
volatile unsigned int Note3 = ClassicNote3;

ISR(TIMER0_COMPA_vect) {

  //notReallyMillis++;
  Acc1 = Acc1 + Note1;
  Acc2 = Acc2 + Note2;
  Acc3 = Acc3 + Note3;
  bool x1 = (Acc1 >> 8) & 0x80;
  bool x2 = (Acc2 >> 8) & 0x80;
  bool x3 = (Acc3 >> 8) & 0x80;

  if (x1 & ((note & B00000001) > 0)) {
    bitSet(PORTB, 0);
  } else {
    bitClear(PORTB, 0);
  }

  if (x2 & ((note & B00000010) > 0)) {
    bitSet(PORTB, 1);
  } else {
    bitClear(PORTB, 1);
  }

  if (x3 & ((note & B00000100) > 0)) {
    bitSet(PORTB, 2);
  } else {
    bitClear(PORTB, 2);
  }
}



byte getPatternValue() {
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
  byte output = result & B00000111;

  return output;
}



// We could just map the pot to discrete steps, but with this step, you can accomidate audio pots if thats what you are stuck with.
// if you are building this, I suggest just using linear pots and skipping this step.
int mapPattern(int sample) {

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


volatile byte stepSize = 0;

// BAsically takes a Note ( the delta in the DDS) and
// gradually steps down the frequency.  The result is a
// squarewave note that starts at a set freq and drops down
// until it hits a threshold and just stops.
//
// The next note will reset the counters.
void dropFreq() {

  if (stepSize < 15) {
    stepSize = stepSize + 1;
  }

  if (Note1 > 250) {
    Note1 = Note1 - stepSize;
  } else {
    Note1 = 0;
    Acc1 = 0;
  }

  if (Note2 > 250) {
    Note2 = Note2 - stepSize;
  } else {
    Note2 = 0;
    Acc2 = 0;
  }

  if (Note3 > 250) {
    Note3 = Note3 - stepSize;
  } else {
    Note3 = 0;
    Acc3 = 0;
  }
}

void loop() {

  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_L1_LOW, VCO1_L1_HIGH);

  int pattern_t = analogRead(A2);
  pattern = mapPattern(map(pattern_t, 0, 1023, 0, MAX_STATE_VALUE));



  dropFreq();
  if (oscCounter1 >= oscFreq1) {
    oscCounter1 = 0;
    note = getPatternValue();

    Note1 = ClassicNote1;
    Note2 = ClassicNote2;
    Note3 = ClassicNote3;
    stepSize = 1;
  }

  oscCounter1++;
}