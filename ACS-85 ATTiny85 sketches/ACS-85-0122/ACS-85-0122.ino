/*

   ACS-85-0122
   Phase Distortion xy

   External pin 1       = Reset (not used)
   External pin 2 (PB3) = Input freq
   External pin 3 (PB4) = Input x
   External pin 4       = GND
   External pin 5 (PB0) =
   External pin 6 (PB1) = Output (PWM)
   External pin 7 (PB2) = Input y
   External pin 8       = Vcc

   V 1.0  -  First Version

   VERY simple version of phase distortion.
   In This case, the normal DDS/Tuning word is used for the waveform period.
   
   For this sketch, the phase mapping ( Skewed accumulator ) is the function of one point 
   defined by the pots as X and Y and two map functions that connect it from
   0,0 to x,y to 255,255.  Still "phase distortion" as I understand it, but in this case, its
   not all that audio interesting.  Looks cool on a scope.

   you can just take the value being sent to the pwm and map it to a sine or other function.

  This is a pretty simple demo, and the frequency math can do with some math

   Rob Stave (Rob the fiddler) CCBY 2022

*/

volatile unsigned int Acc;
volatile unsigned int Note;

volatile byte samples[255];

void setup() {
  SetupDDS ();
}

void SetupDDS () {
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1 << PCKE | 1 << PLLE;

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                               // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10; // PWM A, clear on match, 1:1 prescale
  pinMode(1, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(2, INPUT);

  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;                     // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00;         // 1/8 prescale
  TIMSK = 1 << OCIE0A;                     // Enable compare match, disable overflow
  OCR0A = 60;                              // Divide by 61
}

#define VCO1_HIGH 4000
#define VCO1_LOW 400

void loop() {

  byte xValue = 0;
  byte yValue = 0;

  while (true) {
    //Read freq
    Note = map(analogRead(A3), 0, 1023, VCO1_LOW,  VCO1_HIGH);
    xValue = map(analogRead(A2), 0, 1023, 0,  255);
    yValue = map(analogRead(A1), 0, 1023, 0,  255);

    byte wavePointer = 0;

    // we are basically rebuilind the wavetable every time
    for (int i = 0; i <= 255; i++) {

      if ( i < xValue ) {
        wavePointer = map(i, 0, xValue, 0,  yValue);
      } else {
        wavePointer = map(i, xValue, 255, yValue, 255 );
      }
      samples[i] = wavePointer;
    }
  }
}

ISR(TIMER0_COMPA_vect) {

  Acc = Acc + Note;
  byte ramp = Acc >> 8;
  OCR1A = samples[ramp];
}
