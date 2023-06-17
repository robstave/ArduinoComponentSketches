/**
   ACS-85-0630
   ATTiny85  Random Value LFO
   LFO that outputs a value randomly.
   Kinda like turning machine...but simpler
   Uses LFSR for values. Clock the LFSR in the loop ( or anywhere else that you would like )
   and when we are ready, mask out
   lower bits into OCR0A.
   Rob Stave (Rob the fiddler) ccby 2023
*/

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) Size A3   PB3 2|    |7  PB2 (pin2) clock
//     (pin4) rand A2   PB4 3|    |6  PB1 (pin1) PWM LFO out
//                      GND 4|    |5  PB0 (pin0) trigger Square out
//                           ------

const int clockInt = 0;  // digital pin 2 is now interrupt 0
unsigned int lfsr = 1;

// Length of TRIGGER ( its a loop counter, so just tweak for longer/shorter)
#define TRIG_MAX 500
unsigned int triggerCounter = 0;

// Size of the sequence from 3 to 8.  9 means always random
byte sequenceSize = 3;
// index
byte sequenceCount = 0;
#define ALWAYS_RANDOM 9


byte notes[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };


void clockLfsr() {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  //set output bits

  noInterrupts();  // disable all interrupts

  attachInterrupt(clockInt, clockCounter, RISING);
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1 << PCKE | 1 << PLLE;

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                                     // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10;  // PWM A, clear on match, 1:1 prescale
  pinMode(1, OUTPUT);                            // Enable PWM output pin
  interrupts();                                  // enable all interrupts
}

void clockCounter()  // called by interrupt
{

  setRandomValue();
  triggerCounter = 500;
  bitSet(PORTB, 0);
}

/**
   Set the OCR0A value with the lower bits of the LFSR.
   Notes is an array of sequencer values.  If the value is zero, then it means it 
   needs to be replaced.  A value can be set to zero if the cv ( rand ) is high (ish) or
   of the size of the count is changed ( in which case there is a wholesale reset)
*/
void setRandomValue() {

  //just in case
  if (sequenceCount >= 8) {
    sequenceCount = 0;
  }

  if (notes[sequenceCount] == 0) {
    byte reg = lfsr & B11111111;
    notes[sequenceCount] = reg;
  }

  OCR1A = notes[sequenceCount];

  sequenceCount = sequenceCount + 1;
  if (sequenceCount >= sequenceSize || sequenceCount >= 7) {
    sequenceCount = 0;
  }
}

void resetAllNotesInArray() {
  notes[0] = 0;
  notes[1] = 0;
  notes[2] = 0;
  notes[3] = 0;
  notes[4] = 0;
  notes[5] = 0;
  notes[6] = 0;
  notes[7] = 0;
}

// check for trigger toggle.
void checkTrigger() {

  if (triggerCounter > 0) {
    triggerCounter--;
  }

  if (triggerCounter == 0) {
    bitClear(PORTB, 0);  //done
  }
}

void loop() {

  byte newSize = 1;
  int last_size_read = 0;

  while (true) {

    clockLfsr();

    // check if we need to toggle the sequence.  This is just a cv thing
    int toggleRandom = analogRead(A2);

    if (toggleRandom > 150) {
      byte nextNote = sequenceCount + 1;
      if ((nextNote >= sequenceSize) || (nextNote > 7)) {
        nextNote = 0;
      }
      notes[nextNote] = 0;
    }

    int size_read = analogRead(A3);

    //add some hysteresis so its not so touchy
    if (abs(last_size_read - size_read) > 10) {
      last_size_read = size_read;
    }

    if (last_size_read > 800) {
      newSize = ALWAYS_RANDOM;
    } else if (last_size_read > 600) {
      newSize = 8;
    } else if (last_size_read > 400) {
      newSize = 5;
    } else if (last_size_read > 200) {
      newSize = 4;
    } else {
      newSize = 3;
    }

    // newSize = map(size_read, 0, 1023, 3, 9); this works in a way too

    // if the size has changed, reset the values in the sequencer
    if (sequenceSize != newSize || newSize == ALWAYS_RANDOM) {
      sequenceSize = newSize;
      resetAllNotesInArray();
    }

    checkTrigger();
  }
}