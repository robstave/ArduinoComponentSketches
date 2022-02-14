/*

   ACS-85-0121
   Phase Distortion V1 chainsaw

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = Input freq
   External pin 3 (PB4) = Input CV
   External pin 4       = GND
   External pin 5 (PB0) =
   External pin 6 (PB1) = Output (PWM)
   External pin 7 (PB2) =
   External pin 8       = Vcc

   V 1.0  -  First Version

   VERY simple version of phase distortion.
   In This case, the normal DDS/Tuning word is used for
   the waveform period.  There is no stretching of the waveform, just
   more of a morphing of a descending ramp to an acending one.

   For this sketch, the phase map is the waveform...but you could easily map that one for one
   to a wavetable

  This is a pretty simple demo, and the frequency math can do with some improvement

     Things that can be improved:
   - freq is linear...better if exp
   - analog read is not stellar

  Rob Stave (Rob the fiddler) CCBY 2022

*/

volatile unsigned int Acc;

volatile unsigned int Note = 857;
volatile byte FoldingValue = 128;

void setup() {
  SetupDDS ();
}

void SetupDDS () {
  // Enable 64 MHz PLL and use as source for Timer1
  // PLLCSR = 1<<PCKE | 1<<PLLE;
  // Below is supposed to be better....not sure why
  PLLCSR |= (1 << PLLE);  // PLL enable for asynchronous mode

  while ((PLLCSR & (1 << PLOCK)) == 0x00)
  {
    // Do nothing until plock bit is set
  }
  PLLCSR |= (1 << PCKE); // Sets the timer/clock1 source to RC oscillator

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                               // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10; // PWM A, clear on match, 1:1 prescale
  pinMode(1, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;                     // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00;         // 1/8 prescale
  TIMSK = 1 << OCIE0A;                     // Enable compare match, disable overflow
  OCR0A = 60;                              // Divide by 61
}

#define VCO1_HIGH 4000
#define VCO1_LOW 400

void loop() {

  byte loopCount = 0;
  while (true) {
    //Read freq
    Note = map(analogRead(A3), 0, 1023, VCO1_LOW,  VCO1_HIGH);
   
    // no need to read cv as often as freq
    if (loopCount % 2 == 0 ) {
      FoldingValue = map(analogRead(A2), 0, 1023, 0,  255);
    }
    loopCount++;
  }
}

ISR(TIMER0_COMPA_vect) {

  Acc = Acc + Note;

  byte ramp = Acc >> 8;

  if (ramp <= FoldingValue ) {
    OCR1A = ramp;
  } else {
    OCR1A = 255 - ramp + FoldingValue;
  }

}
