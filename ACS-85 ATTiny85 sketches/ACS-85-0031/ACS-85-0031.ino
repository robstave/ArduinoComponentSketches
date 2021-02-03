/**
   ACS-85-0031
   ATTiny85 Three phase LFO PWM Clocked
   External pin 1       = Reset (not used)
   External pin 2 (PB3) = none
   External pin 3 (PB4) = OUTPUT c
   External pin 4       = GND
   External pin 5 (PB0) = OUTPUT a
   External pin 6 (PB1) = OUTPUT b
   External pin 7 (PB2) = Clock
   External pin 8       = Vcc

   V 1.0  -  First Version
   Not the best.  PWM is FAST for PB0/PB1 but much slower on PB4.
   like 30khz vs 500 hz I think.   I might rewrite this because of that.
   But visually, and if your using LEDs, it looks fine

   I would say not ready for prime time.  Could do better with bit flipping.
   
   Rob Stave (Rob the fiddler) ccby 2021
*/

//  ATTiny overview
//                      +-\/-+
//               Reset 1|    |8  VCC
//    (pin3) none  PB3 2|    |7  PB2 (pin2) clock input
// (pin4)output C  PB4 3|    |6  PB1 (pin1) output B
//                 GND 4|    |5  PB0 (pin0) output A
//                      ------


volatile byte a1Value = 0;
volatile byte a2Value = 80;
volatile byte a3Value = 160;


const int clockInt = 0;  // digital pin 2 is now interrupt 0

// the setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00011011;  //set output bits


  noInterrupts(); // disable all interrupts
  attachInterrupt(clockInt, clockCounter, RISING);

  // increase the resolution of analog out to 15k
  // This is at the expense of making delay not work.
  // Fine for this sketch

  TCCR0B = TCCR0B & 0b11111000 | 0b001;


  interrupts(); // enable all interrupts

}

void clockCounter()      // called by interrupt
{
  a1Value++;
  a2Value++;
  a3Value++;
}

void loop() {
  boolean triggerArmed = true;
  boolean clockValue;

  while (true) {
    analogWrite(0, a1Value);
    analogWrite(1, a2Value);
    analogWrite(4, a3Value);
  }

}
