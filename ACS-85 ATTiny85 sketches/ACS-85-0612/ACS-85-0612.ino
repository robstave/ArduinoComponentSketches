
/**
   ACS-85-0612
   ATTiny85 Random trigger clocked
   Like mutable instruments Branches perhaps

   Takes an input clock and works on the rising edge.
   Outputs a trigger or latched output based on the value of the probability.

   Uses LFSR
   Length of trigger is configurable in software.

   This is really like 607 but clocked
   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input chance/prob
   External pin 3 (PB4) = input toggle mode
   External pin 4       = GND
   External pin 5 (PB0) = output A
   External pin 6 (PB1) = output B
   External pin 7 (PB2) = Clock
   External pin 8       = Vcc
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2021
*/

//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) chance 2  PB3 2|    |7  PB2 (pin2) clock input
//     (pin4) toggle 3  PB4 3|    |6  PB1 (pin1) output B
//                      GND 4|    |5  PB0 (pin0) output A
//                           ------

// Adjustable parameters

int maxTriggerTime = 80;   // max time of the trigger, if in that mode
// max and min probability.  Really, there is no need to change this unless you want
// the max and min to be something like 25% to 75%....then change to 64 and 192
#define RANDLOW 1
#define RANDHIGH 255

// Pin definitions
#define GATE_PIN 4
#define clockInt  0  // digital pin 2 is now interrupt 0

// variables 
volatile bool doTrigger = true;

volatile unsigned long startTrigger = 0;
volatile byte randomness = 0;
volatile unsigned int lfsr  = 31;  // initialize to whatever, so long as its not zero
 
// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000011;  //set output bits
  attachInterrupt(clockInt, clockCounter, RISING);
}

/**
  Check if we need to flip a bit or two based on lfsr
  We are checking against the LFSR.  Note, it will never be all zeros
*/
boolean doFlip ( byte p) {

  clockLfsr();
  byte c1 = lfsr & B11111111;
  if (c1 < p) {
    return true;
  }
  return false;
}



// the meat of the deal.  Returns Port B output.
// If you were so inclined, there is nothing that says this has to
// be either or.  B00000011 would make both pins high.
byte getPatternValue () {
  byte result = B00000000;

  if (doFlip(randomness)) {
    result =   B00000001;
  } else {

    result =   B00000010;
  }
  return result;
}

void clockCounter()      // called by interrupt
{
  PORTB = getPatternValue();
  startTrigger = millis();
}


// Boilerplate 32 bit lfsr I use everywhere.
void clockLfsr () {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

void loop() {
  randomness =  33;  // initialize to whatever

  unsigned long currentMillis;

  while (true) {
    clockLfsr ();

    randomness = map(analogRead(A3), 0, 1023, RANDLOW, RANDHIGH);

    doTrigger = digitalRead(GATE_PIN) == HIGH;

    currentMillis = millis();

    if (doTrigger) {
      // To make the trigger, let it go for a little while then clear the port
      if (currentMillis - startTrigger >   maxTriggerTime) {
        PORTB =  B00000000;
      }

      startTrigger =  millis();
    }
  }

}
