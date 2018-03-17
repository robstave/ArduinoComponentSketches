/**
   ACS-85-xxx
   Simple VCA


   External pin 1       = Reset (not used)
   External pin 2 (PB3) = Volume
   External pin 3 (PB4) =
   External pin 4       = GND
   External pin 5 (PB0) = output (PWM)
   External pin 6 (PB1) =
   External pin 7 (PB2) = Clock
   External pin 8       = Vcc

   V 1.0  -  First Version

   Improvements
    maybe make the map log-ish
    Im not reading the value of the clock pin, just flippling so it may not be
    super good for LFO cases.  We should sample a few times first I suppose
    at a minimum or maybe every time.

   Note: This sketch has been written specifically for ATTINY85 and not Arduino uno
   Rob Stave (Rob the fiddler) CCBY 2018
*/


//  ATTiny overview
//                    +-\/-+
//             Reset 1|    |8  VCC
// (pin3) volume PB3 2|    |7  PB2 (pin2/int0) CLOCK
// (pin4)   none PB4 3|    |6  PB1 (pin1) none
//               GND 4|    |5  PB0 (pin0) out
//                    ------


const int int0 = 0; // interrupt 0
volatile uint8_t volume;

volatile uint8_t state = HIGH;


// the setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00010000;  //set output bits

  // initialize timer0
  noInterrupts();           // disable all interrupts
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1<<PCKE | 1<<PLLE;     

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                     // Timer interrupts OFF
  TCCR1 = 1<<CS10;               // 1:1 prescale
  GTCCR = 1<<PWM1B | 2<<COM1B0;  // PWM B, clear on match


  pinMode(2, INPUT_PULLUP);

  interrupts(); // enable all interrupts

  //  clockCounter function is called when invoked on changing  clock edge
  attachInterrupt(int0, clockCounter, CHANGE);

}

void clockCounter()      // called by interrupt
{

  state = !state;
  if (state == HIGH) {
    OCR1B  = volume;
  } else {
    OCR1B  = 0;
  }
}

void loop() {

  int vol = 0;
  while (true) {
    vol = analogRead(A3);
    volume = map(vol, 0, 1023, 0, 255);
  }

}