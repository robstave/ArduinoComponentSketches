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

   Things that can be improved:
   - freq is linear...better if exp
   - analog read is not stellar

   Rob Stave (Rob the fiddler) CCBY 2022

*/

// Set to 0 for sine and 128 for cosine
volatile byte sinOffset = 128;


volatile unsigned int Acc;
volatile unsigned int Note;

volatile byte phaseTransform[256];

const byte sinetable[256] PROGMEM = {

  0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11, 12, 14, 15, 16, 18, 20, 21, 23, 25, 27, 29, 31,

  33, 35, 37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76, 78, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 115, 118, 121, 124,

  127, 130, 133, 136, 139, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 178, 181, 184, 187, 190, 192, 195, 198, 200, 203, 205, 208, 210, 212, 215, 217, 219, 221, 223, 225, 227, 229, 231, 233, 234, 236, 238, 239, 240,

  242, 243, 244, 245, 247, 248, 249, 249, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 249, 249, 248, 247, 245, 244, 243, 242, 240, 239, 238, 236, 234, 233, 231, 229, 227, 225, 223,

  221, 219, 217, 215, 212, 210, 208, 205, 203, 200, 198, 195, 192, 190, 187, 184, 181, 178, 176, 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 139, 136, 133, 130, 127, 124, 121, 118, 115, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81, 78,

  76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39, 37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 16, 15, 14, 12, 11, 10, 9, 7, 6, 5, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0, 0

};

const byte costable[256] PROGMEM = {

  254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 249, 249, 248, 247, 245, 244, 243, 242, 240, 239, 238, 236, 234, 233, 231, 229, 227, 225, 223,

  221, 219, 217, 215, 212, 210, 208, 205, 203, 200, 198, 195, 192, 190, 187, 184, 181, 178, 176, 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 139, 136, 133, 130, 127, 124, 121, 118, 115, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81, 78,

  76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39, 37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 16, 15, 14, 12, 11, 10, 9, 7, 6, 5, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0, 0,

  0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11, 12, 14, 15, 16, 18, 20, 21, 23, 25, 27, 29, 31,

  33, 35, 37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 62, 64, 67, 70, 73, 76, 78, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 115, 118, 121, 124,

  127, 130, 133, 136, 139, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 178, 181, 184, 187, 190, 192, 195, 198, 200, 203, 205, 208, 210, 212, 215, 217, 219, 221, 223, 225, 227, 229, 231, 233, 234, 236, 238, 239, 240,

  242, 243, 244, 245, 247, 248, 249, 249, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 254

};


void setup() {
  SetupDDS ();
}

void SetupDDS () {
  // Enable 64 MHz PLL and use as source for Timer1
  //PLLCSR = 1 << PCKE | 1 << PLLE;

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
  pinMode(2, INPUT);

  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;                     // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00;         // 1/8 prescale
  TIMSK = 1 << OCIE0A;                     // Enable compare match, disable overflow
  OCR0A = 60;                              // Divide by 61
}

#define VCO1_HIGH 4000
#define VCO1_LOW 400

void doPhaseTable () {
  byte xValue = map(analogRead(A2), 0, 1023, 0,  255);
  byte yValue = map(analogRead(A1), 0, 1023, 0,  255);

  byte wavePointer = 0;

  // we are basically rebuilding the wavetable every time
  for (int i = 0; i <= 255; i++) {
    if ( i < xValue ) {
      wavePointer = map(i, 0, xValue, 0,  yValue);
    } else {
      wavePointer = map(i, xValue, 255, yValue, 255 );
    }
    phaseTransform[i] = wavePointer;
  }
}


void loop() {

  byte loopCount = 3;
  while (true) {
    //Read freq
    Note = map(analogRead(A3), 0, 1023, VCO1_LOW,  VCO1_HIGH);

    // build wave table in loop...but not every loop.  Id rather
    // read the frequency
    if (loopCount % 3 == 0 ) {
      doPhaseTable ();
    }
    loopCount++;
  }
}

ISR(TIMER0_COMPA_vect) {

  Acc = Acc + Note;
  byte ramp = Acc >> 8;

  // ramp is just counting up
  // samples[ramp] is the transform

  OCR1A = phaseTransform[ramp];

  // if you want a sine wave, comment the above one line out and uncomment the one you want
  //byte  value = pgm_read_word_near(sinetable + phaseTransform[ramp]);
  //byte  value = pgm_read_word_near(costable + phaseTransform[ramp]);

  //OCR1A = value;

}
