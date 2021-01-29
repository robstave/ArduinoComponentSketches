
/**
   ACS-85-0620
   ATTiny85 Trigger to Gate

   Takes a trigger ( rising edge) and outputs a gate.
   Similar to Erika Synth PICO TG

   This is really like 607 but clocked
   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input Length
   External pin 3 (PB4) = input Prob
   External pin 4       = GND
   External pin 5 (PB0) = output A
   External pin 6 (PB1) = output B
   External pin 7 (PB2) = Input Trigger
   External pin 8       = Vcc
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2021
*/



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) Length 2  PB3 2|    |7  PB2 (pin2) Trigger input
//     (pin4) Prob 3    PB4 3|    |6  PB1 (pin1) output B
//                      GND 4|    |5  PB0 (pin0) output A
//                           ------

// Configuration values

#define MINGATE  80  // min time of the gate
#define MAXGATE  2000   // max time of the gate

// you can tweak this for narrower ranges of probability.
// for example  { 100, 150} would make the range of the probabilit between 40% and 60% vs 0 to 100%
// you prob dont want to change this
#define RANDLOW  1
#define RANDHIGH  254

// if you set this, the random gate will also have random length
const boolean doRandomLength = false;


// variables
volatile byte randomness = 0;

volatile unsigned int lfsr  = 31;

volatile unsigned long gateTime; // length of gate as sampled
volatile unsigned long gateATime; // length of gate
volatile unsigned long gateBTime; // length of gate

volatile unsigned long startGateTime = 0;

volatile boolean gateARunning = false;
volatile boolean gateBRunning = false;


const int clockInt = 0;  // digital pin 2 is now interrupt 0

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
  byte result = B00000001;
  gateARunning = true;

  if (doFlip(randomness)) {
    gateBRunning = true;
    result =  result |  B00000010;
  }
  return result;
}

void clockCounter()      // called by interrupt
{
  PORTB = getPatternValue();
  
  startGateTime = millis();

  gateATime = gateTime;

  if (doRandomLength) {
    byte c1 = lfsr & B11111111;
    gateBTime = map(c1, RANDLOW, RANDHIGH, MINGATE, MAXGATE);
  } else {
    gateBTime = gateTime;
  }
  
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

    gateTime = map(analogRead(A3), 0, 1023, MINGATE, MAXGATE);
    randomness = map(analogRead(A2), 0, 1023, RANDLOW, RANDHIGH);

    currentMillis = millis();


   if (gateARunning){
    if ((currentMillis - startGateTime) >   gateATime) {
      PORTB =  PORTB & B00000010;
      gateARunning = false;
    }
   }

    if (gateBRunning){
    if ((currentMillis - startGateTime) >   gateBTime) {
      PORTB =  PORTB & B00000001;
      gateBRunning = false;
    }
   }



  }

}
