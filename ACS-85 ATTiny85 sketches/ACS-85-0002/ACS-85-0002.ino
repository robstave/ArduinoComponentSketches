/**
   ACS-85-0002 ATTiny85 Dual VCO with Mixed Output

   A dual oscillator circuit with two independent frequency inputs and three outputs.
   The third output provides an interesting mix of the two oscillators.

   Pin Configuration:
   - Pin 2 (PB3): Frequency Input 1
   - Pin 3 (PB4): Frequency Input 2
   - Pin 4     : GND
   - Pin 5 (PB0): Oscillator 1 Output
   - Pin 6 (PB1): Oscillator 2 Output 
   - Pin 7 (PB2): Mixed Output (alternates between combining both oscillators)
   - Pin 8     : VCC

   Description:
   Uses Timer1 in CTC mode with a prescaler of 2. The timer frequency affects the
   resolution of the oscillators - higher frequencies allow for finer control but
   limit processing time available in the interrupt.

   The mixed output (PB2) alternates between combining both oscillators and silence,
   creating an amplitude modulation effect controlled by MIX_LFO.

   Frequency ranges are mapped from analog inputs (0-1023) to timer values (5-118).
   Note that frequency response is non-linear due to the timer-based implementation.

   Version History:
   - V1.0: Initial version
   - V1.1: Improved comments
   - V1.2: Added input averaging for stability
   - V1.3: Code refactoring

   Author: Rob Stave (Rob the Fiddler), CCBY 2015
*/

// Frequency range constants 
#define VCO1HIGH 5
#define VCO1LOW 118
#define VCO2HIGH 5
#define VCO2LOW 118
#define MIX_LFO 3000  // Period of mixed output modulation

// Counters for the frequencies
byte oscFreq1 = 0;
byte oscCounter1 = 0;
byte oscFreq2 = 0;
byte oscCounter2 = 0;

boolean oscState = false;
int oscCounter3 = 0;

// Used for sample averaging
int f1Sample[4] = {0, 0, 0, 0};
int f2Sample[4] = {0, 0, 0, 0};
byte counter = 0;

// The setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  // Configure PB0, PB1, PB2 as outputs

  // Initialize Timer1
  noInterrupts();           // Disable all interrupts

  TCCR1 = 0;                // Stop the timer
  TCNT1 = 0;                // Zero the timer

  OCR1A = 50;               // Set the compare value
  OCR1C = 50;               // Set the compare value (redundant but safe)

  TIMSK = _BV(OCIE1A);      // Interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, CTC mode, prescaler clk/2

  interrupts();             // Enable all interrupts
}

ISR(TIMER1_COMPA_vect) {    // Timer compare interrupt service routine

  // Count up and toggle portB bits
  if (oscCounter1 > oscFreq1) {
    oscCounter1 = 0;
    PORTB ^= (_BV(PB0));

    // Bonus: toggle pin 2 also if state is true
    if (oscState == true) {
      PORTB ^= (_BV(PB2));
    }
  }
  oscCounter1++;

  if (oscCounter2 > oscFreq2) {
    oscCounter2 = 0;
    PORTB ^= (_BV(PB1));

    // Bonus: toggle pin 2 also if state is true
    if (oscState == true) {
      PORTB ^= (_BV(PB2));
    }
  }
  oscCounter2++;

  // Toggle alternating frequency
  // So basically, we are setting a flag for on/off.
  // If the state is on, the counters will sum together giving a ring effect.
  if (oscCounter3 > MIX_LFO) {
    oscCounter3 = 0;
    oscState = !oscState;
  }
  oscCounter3++;
}

void readVco1freq() {
  int osc1_t = 0;
  f1Sample[counter] = analogRead(A3);
  osc1_t = (f1Sample[0] + f1Sample[1] + f1Sample[2] + f1Sample[3]) >> 2;
  oscFreq1 = map(osc1_t, 0, 1023, VCO1LOW, VCO1HIGH);
}

void readVco2freq() {
  int osc1_t = 0;
  f2Sample[counter] = analogRead(A2);
  osc1_t = (f2Sample[0] + f2Sample[1] + f2Sample[2] + f2Sample[3]) >> 2;
  oscFreq2 = map(osc1_t, 0, 1023, VCO2LOW, VCO2HIGH);
}

void tickCounter() {
  counter++;
  if (counter > 3) {
    counter = 0;
  }
}

void loop() {
  readVco1freq();
  readVco2freq();
  tickCounter();
}
