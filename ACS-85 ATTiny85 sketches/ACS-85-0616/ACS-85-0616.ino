
/**
   ACS-85-0616
   ATTiny85 probability with delay

   A chance sketch with the trigger on Pin 7.
   On rising, the output of Pin 5 and 6 trigger for a fixed Length with a 
   50% and 25% probability.  
   The Probability and trigger length values are fixed.

   If PB4 is High, the the gate triggers after a delay ( the lenght remains the same).
   If Pin 3 is low ( not plugged in, you may need to tie that down) 
   then it just acts like a regular probability gate.
   If the pin is high, then an added delay happens before the pin is set to high.

   A3 determines the length of the delay. 

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = input delay
   External pin 3 (PB4) = input Do Delay
   External pin 4       = GND
   External pin 5 (PB0) = output A 50%
   External pin 6 (PB1) = output B 25%
   External pin 7 (PB2) = Input Trigger
   External pin 8       = Vcc
   V 1.0  -  First Version

   Rob Stave (Rob the fiddler) ccby 2024
*/



//  ATTiny overview
//                           +-\/-+
//                    Reset 1|    |8  VCC
//     (pin3) Length 2  PB3 2|    |7  PB2 (pin2) Trigger input
//     (pin4) DoDelay 3 PB4 3|    |6  PB1 (pin1) output B 50%
//                      GND 4|    |5  PB0 (pin0) output A 25%
//                           ------

// Configuration values

#define MINDELAY 0      // min time of the Delay
#define MAXDELAY 1000   // max time of the Delay

#define GATELENGTH 200  // Length of Gate

// In this case, the probability is fixed at 50% and 25, but you can adjust as required.
// This number is just compared to the low 8 bits of the LFSR.
#define RAND1 128  
#define RAND2 64 // triggers if x (8bit) < 64

// variables

volatile unsigned int lfsr = 31;
volatile unsigned long startGateTime = 0;
volatile boolean doGate1 = false;
volatile boolean doGate2 = false;
volatile boolean gateRunning = false;


const int clockInt = 0;  // digital pin 2 is now interrupt 0

// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000011;  //set output bits
  attachInterrupt(clockInt, clockCounter, RISING);
}

/**
  Check if we need to flip a bit based on the value of the lfsr.
  We are just comparing the byte against the LFSR.  
  Note, it will never be all zeros
*/
boolean doFlip(byte p) {
  byte c1 = lfsr & B11111111;
  if (c1 < p) {
    return true;
  }
  return false;
}

// Checks probabilities and sets gateRunning if either pin needs to be set high.
void calculatePinProbability() {
  clockLfsr();  // one more clocking of the lfsr
  if (doFlip(RAND1)) {
    doGate1 = true;
    gateRunning = true;
  }

  if (doFlip(RAND2)) {
    doGate2 = true;
    gateRunning = true;
  }
}

void clockCounter()  // called by interrupt
{
  clockLfsr();
  calculatePinProbability();
  startGateTime = millis();
}


// Boilerplate 32 bit lfsr I use everywhere.
void clockLfsr() {

  //calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}


void loop() {

  unsigned long delayTime;  // length of delay
  unsigned long currentMillis;

  while (true) {
    clockLfsr();

    delayTime = 0;

    if (digitalRead(PB4)) {
      delayTime = map(analogRead(A3), 0, 1023, MINDELAY, MAXDELAY);
    }

    currentMillis = millis();

    // gateRunning is actually set in the clock interrupt
    if (gateRunning) {

      if ((currentMillis - startGateTime) >= delayTime) {
        if (doGate1) {
          PORTB = PORTB | B00000001;  // set pb0 high
          doGate1 = false;
        }
        if (doGate2) {
          PORTB = PORTB | B00000010;  // set pb1 high
          doGate2 = false;
        }
      }

      if ((currentMillis - startGateTime) > (GATELENGTH + delayTime)) {
        PORTB = PORTB & B11111100;  // clear both bits
        gateRunning = false;
      }
    }
  }
}