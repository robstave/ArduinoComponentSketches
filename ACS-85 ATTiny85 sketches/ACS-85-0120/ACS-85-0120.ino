/*
  
   ACS-85-0120
   Phase Distortion V1  Sawtooth


   External pin 1       = Reset (not used)
   External pin 2 (PB3) = Input freq
   External pin 3 (PB4) = Input CV
   External pin 4       = GND
   External pin 5 (PB0) = 
   External pin 6 (PB1) = Output (PWM)
   External pin 7 (PB2) = 
   External pin 8       = Vcc

   V 1.0  -  First Version

   VERY simple version of phase distortion.  There seem to be a lot of versions to this
   seemingly broad definition.  In This case, the normal DDS/Tuning word is used for 
   the waveform period.  The word is then used with a multiplier to achieve the phasing.

   When the normal accumulator resets to 0, the phase is restarted.

  This is a pretty simple demo, and the frequency math can do with some work.



  There is some code from Jan Ostman
  https://github.com/4-3is4-me/CZ1

  that is supposed to be a version of the CZ1.  Im gonna say Im not sure how that 
  math works out.

 
*/

volatile unsigned int Acc;
volatile unsigned int PhaseAcc;

volatile unsigned int Note = 857;
volatile unsigned int PhaseNote = 857;

void setup() {
  SetupDDS ();
}

void SetupDDS () {
  // Enable 64 MHz PLL and use as source for Timer1
  PLLCSR = 1<<PCKE | 1<<PLLE;     

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                               // Timer interrupts OFF
  TCCR1 = 1<<PWM1A | 2<<COM1A0 | 1<<CS10;  // PWM A, clear on match, 1:1 prescale
  pinMode(1, OUTPUT);                       
  pinMode(3, INPUT);                      
  pinMode(4, INPUT);  

  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3<<WGM00;                       // Fast PWM
  TCCR0B = 1<<WGM02 | 2<<CS00;             // 1/8 prescale
  TIMSK = 1<<OCIE0A;                       // Enable compare match, disable overflow
  OCR0A = 60;                              // Divide by 61
}

#define VCO1_HIGH 4000
#define VCO1_LOW 400

void loop() {

  //Read freq
  int osc1_t = analogRead(A3);
  Note = map(osc1_t, 0, 1023, VCO1_LOW,  VCO1_HIGH);


  float phaseAdj = Note * ( float(analogRead(A2)) /1100.0);

  // add to the phase accumulator to increase the frequency of the repeated waveform.
  PhaseNote = Note + int(phaseAdj);
}

ISR(TIMER0_COMPA_vect) {


  // Detect rollover of regular accumulator
  unsigned int last = Acc;
  Acc = Acc + Note;
  if (last >= Acc ) {
    // Reset phase if accumulator rolls over
    PhaseAcc = 0;
  }

  PhaseAcc = PhaseAcc + PhaseNote;
  OCR1A = PhaseAcc >> 8;
}
