/**
   ACS-85-0608
   ATTiny85 clocked 4 Channel random triggers
   Simple Drum triggers
   
   This is really like 607 but clocked
   External pin 1       = Reset (not used)
   External pin 2 (PB3) = output 2 clock
   External pin 3 (PB4) = output 3
   External pin 4       = GND
   External pin 5 (PB0) = output 0
   External pin 6 (PB1) = output 1
   External pin 7 (PB2) = clock input
   External pin 8       = Vcc
   V 1.0  -  First Version
   Any further extensions to this will be treated as a sequencer.
   Rob Stave (Rob the fiddler) ccby 2019
*/



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) output 2  PB3 2|    |7  PB2 (pin2) clock input
//     (pin4) output 3  PB4 3|    |6  PB1 (pin1) output 1
//                      GND 4|    |5  PB0 (pin0) output 0
//                           ------




#define MAX_RANDOM 130


volatile unsigned long startTrigger = 0;
volatile byte randomness = 0;
volatile unsigned int lfsr  = 31;
int maxTriggerTime = 200;   // max time of the trigger

const int clockInt = 0;  // digital pin 2 is now interrupt 0

// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00011011;  //set output bits
  attachInterrupt(clockInt, clockCounter, RISING);

}

byte getPatternValue () {
  byte result = B00000000;

  // if there is some randomness going...flip based on last  bits of lfsr.  This might end up no flipping at all.
  if (doFlip()) {
    result = result | ( lfsr & B00000001);
  }
  if (doFlip()) {
    result = result | ( lfsr & B00000010);
  }

  if (doFlip()) {
    result = result | ( lfsr & B00001000);
  }

  if (doFlip()) {
    result = result | ( lfsr & B00010000);
  }


  return result;
}

void clockCounter()      // called by interrupt
{
  PORTB = getPatternValue();

  startTrigger = millis();
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

  clockLfsr();
  byte c1 = lfsr & B11111111;
  if (c1 < randomness) {
    return true;
  }
  return false;
}



unsigned long triggerTime;

void loop() {
  randomness =  MAX_RANDOM; // pick max to be like 30% to 50% of 256

  unsigned long currentMillis;
  while (true) {
    clockLfsr ();
    currentMillis = millis();

    if (currentMillis - startTrigger >   maxTriggerTime) {
      PORTB =  B00000000;
      startTrigger =  millis();
    }
  }

}
