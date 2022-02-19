/*

   ACS-85-0120
   Phase Distortion V1  Sawtooth


   External pin 1       = Reset (not used)
   External pin 2 (PB3) = Input freq
   External pin 3 (PB4) = Input CV
   External pin 4       = GND
   External pin 5 (PB0) = Input Wave Envelope
   External pin 6 (PB1) = Output (PWM)
   External pin 7 (PB2) =
   External pin 8       = Vcc


  Phase distortion synthesis is an interesting alternative to classic substractive synthesis.
  Rather than a VCO/DCO with a harmonically rich waveform that you pass through a filter
  ( subtracting harmonics ) to achieve a timber, it uses usually a simpler waveform that
  is distorted either by tweaking the phase in a wavetable or shaping the wave to effectively
  add harmonics.

  It turns out that there are even ways to achieve cutoffs, resonance ( in a sense )
  and filter sweeps.  This is then feed into your typical VCAs with all the envelopes.

  The nice thing is that it all can be done with just a microcontroller.
  No super difficult DSP or analog filtering needed.
  The ATTINY is still not exactly a good fit for this.
  Still, it makes for an interesting exploration.

  The first sketch is just me looking at the WIKI and seeing if I can do part of that.
  This is almost exactly like the first 2 ( or 3 ) steps in
  https://en.wikipedia.org/wiki/Phase_distortion_synthesis without the windowing.

  The Bool on pin zero applies a dampening on the last 20 values of the waveform.
  Its Subtle, but without it, that sharp cut at wavetable value 255 causes lots
  of overtones.

   V 1.0  -  First Version

   VERY simple version of phase distortion.  There seem to be a lot of versions to this
   seemingly broad definition.  In This case, the normal DDS/Tuning word is used for
   the waveform period.  The word is then used with a multiplier to achieve the phasing.

   When the normal accumulator resets to 0, the phase is restarted.

  This is a pretty simple demo, and the frequency math can do with some work.

  An excellent explaination is buried in this pdf

  https://thesnowfields.com/manuals/An%20Insider%27s%20Guide%20to%20Casio%20CZ%20Synthesizers.pdf


  Other references:
  There is some code from Jan Ostman
  https://github.com/4-3is4-me/CZ1

  that is supposed to be a version of the CZ1.  Im gonna say Im not sure how that
  math works out. It does not match up with any kinda phase thing that I can tell, but
  it might have the resonance bit where you feed back the frequence to enhance a peak.
  In any case, this sketch is not that.

  Rob Stave (Rob the fiddler) CCBY 2022

*/

volatile unsigned int Acc;
volatile unsigned int PhaseAcc;

volatile unsigned int Note = 857;
volatile unsigned int PhaseNote = 857;

int envelopePin = 0;  // LED connected to digital pin 13
volatile int doEnv = 0;


void setup()
{
  SetupDDS();
}

void SetupDDS()
{
  // Enable 64 MHz PLL and use as source for Timer1
  // PLLCSR = 1<<PCKE | 1<<PLLE;
  // Below is supposed to be better....not sure why
  PLLCSR |= (1 << PLLE); // PLL enable for asynchronous mode

  while ((PLLCSR & (1 << PLOCK)) == 0x00)
  {
    // Do nothing until plock bit is set
  }
  PLLCSR |= (1 << PCKE); // Sets the timer/clock1 source to RC oscillator

  // Set up Timer/Counter1 for PWM output
  TIMSK = 0;                                    // Timer interrupts OFF
  TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10; // PWM A, clear on match, 1:1 prescale
  pinMode(1, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(envelopePin, INPUT_PULLUP);

  
  // Set up Timer/Counter0 for 20kHz interrupt to output samples.
  TCCR0A = 3 << WGM00;             // Fast PWM
  TCCR0B = 1 << WGM02 | 2 << CS00; // 1/8 prescale
  TIMSK = 1 << OCIE0A;             // Enable compare match, disable overflow
  OCR0A = 60;                      // Divide by 61
}

#define VCO1_HIGH 4000
#define VCO1_LOW 400

void loop()
{

  byte count = 0;
  while (true) {
    //Read freq
    int osc1_t = analogRead(A3);
    Note = map(osc1_t, 0, 1023, VCO1_LOW, VCO1_HIGH);

    float phaseAdj = Note * (float(analogRead(A2)) / 1100.0);

    // add to the phase accumulator to increase the frequency of the repeated waveform.
    PhaseNote = Note + int(phaseAdj);
    
    
    count++;
    // This pin only needs to be read like every 10 loops
    if (count % 10 == 0 ) {
      doEnv = digitalRead(envelopePin);
    }
  }
}

ISR(TIMER0_COMPA_vect)
{

  // Detect rollover of regular accumulator
  unsigned int last = Acc;
  Acc = Acc + Note;
  if (last >= Acc)
  {
    // Reset phase if accumulator rolls over
    PhaseAcc = 0;
  }

  PhaseAcc = PhaseAcc + PhaseNote;
  byte value = PhaseAcc >> 8;

  // Check if we want to apply the windowing feature
  // This just attenuates values past wavetable index 230 ( of 255)
  if (doEnv == 0 ) {
    OCR1A = value;
    return;
  }


  // attenuate value based on location of the accumulator ( wave) pointer
  byte window = Acc >> 8;

  if (window < 230) {
    OCR1A = value;
    return;
  }

  if (window > 250) {
    OCR1A = (value >> 4);
    return;
  }
  if (window > 242) {
    OCR1A = (value >> 3);
    return;
  }

  if (window > 235) {
    OCR1A = (value >> 2);
    return;
  }


  OCR1A = (value >> 1);



}
