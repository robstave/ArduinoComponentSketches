/**
   ACS-84-0608
   ATTiny84 clocked 4 Channel random triggers
   (requires an external clock)
   Simple Drum triggers that randomly trigger

   The clock ticks the LFSR ( Linear Feedback Shift Register).
   At each tick, we check if the lower 8 bits are
   greater than the probabilty value (Randomness)

   Permutations of the code could be: 
    - make some outputs a little more likely than others.

   The length of the trigger is defined by the other pot. PA1.
   
   There is another output that is pretty much a
   clock to trigger output.  It's intent was to drive
   a Pocket Oscillator.

  This is really like ACS-085-0607 but clocked
   External pin 1       = Vcc
   External pin 2  (PB0) = not used
   External pin 3  (PB1) = not used
   External pin 4  (PB)  = Reset (not used)
   External pin 5  (PB0) = not used
   External pin 6  (PA7) = not used
   External pin 7  (PA6) = output clock as trigger
   External pin 8  (PA5) = out 1
   External pin 9  (PA4) = out 2
   External pin 10 (PA3) = out 3
   External pin 11 (PA2) = out 4
   External pin 12 (PA1) = Trigger Length
   External pin 13 (PA0) = Random
   External pin 14       = GND
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2020
*/



//  ATTiny overview
//                        +-\/-+
//               VCC set 1|    |8  GND
//                   PB0 2|    |7  PA0 Rand (probabilty) A0
//                   PB1 3|    |6  PA1 Width A1
//                 reset 4|    |5  PA2 output 0
//    clock     INT0 PB2 5|    |5  PA3 output 1
//                   PA7 6|    |5  PA4 output 2
//   output trigger  PA6 7|    |5  PA5 output 3
//                           ------




#define MAX_RANDOM 220  // no bigger than 245 or so  Hard Stop at 255
#define MIN_RANDOM 10   // make smaller for super low probability

#define TRIGGER_MIN 15    // min trigger length in ms
#define TRIGGER_MAX 2000  // max trigger length in ms

// if the above is not working..double check that you are running at 8Mz

volatile unsigned long startTrigger = 0;
volatile byte randomness = 0;
volatile unsigned int lfsr  = 31;

const int clockInt = 0;  // digital pin 2 is interrupt 0

void setup() {
  DDRA = B01111100;  //set output bits
  attachInterrupt(clockInt, clockCounter, RISING);
}


/*
   Build up a pattern to apply to PORTA
*/
byte getPatternValue () {
  byte result = B00000000;

  // Pin 2
  if (doFlip()) {
    result = result | ( lfsr & B00000100);
  }

  // Pin 3
  if (doFlip()) {
    result = result | ( lfsr & B00001000);
  }

  // Pin 4
  if (doFlip()) {
    result = result | ( lfsr & B00010000);
  }

  // Pin 5
  if (doFlip()) {
    result = result | ( lfsr & B00100000);
  }

  // flip the trigger bit no matter what..pin6
  result = result |  B01000000;
  return result;
}

void clockCounter()      // called by interrupt
{
  PORTA = getPatternValue();
  startTrigger = millis(); // Countdown to trigger length
}


void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

/**
  Check if we need to flip a bit or two based on lfsr
*/
boolean doFlip () {
  // Get the next batch of bits. Im getting 7...but 8 would fine.  If you only clock once,
  // than a high number is still kinda high shifted once
  clockLfsr();
  clockLfsr();
  clockLfsr();
  clockLfsr();
  clockLfsr();
  clockLfsr();
  clockLfsr();


  byte c1 = lfsr & B11111111;
  if (c1 < randomness) {
    return true;
  }
  return false;
}



void loop() {
  randomness =  MAX_RANDOM;

  unsigned long maxTriggerTime;
  unsigned int readRandomCount = 0;
  unsigned int readTriggerCount = 5;


  unsigned long currentMillis;
  while (true) {
    clockLfsr (); // clock for good measure

    // no need to read values every loop
    readRandomCount++;

    if (readRandomCount > 10) {
      readRandomCount = 0;
      randomness = map(analogRead(A0), 0, 1023, MIN_RANDOM,  MAX_RANDOM);
    }

    readTriggerCount++;
    if (readTriggerCount > 10) {
      readTriggerCount = 0;
      maxTriggerTime = map(analogRead(A1), 0, 1023, 15,  2000);
    }

    currentMillis = millis();

    // Clear everything if trigger time passes
    if (currentMillis - startTrigger >   maxTriggerTime) {
      PORTA =  B00000000;
      startTrigger =  millis();
    }
  }

}
