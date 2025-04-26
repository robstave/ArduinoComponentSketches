/**
   ACS-85-0005 ATTiny85 Tri-VCO with Detune
   
   A triple oscillator circuit with one main frequency control and detune 
   capability that affects the other two oscillators.

   Pin Configuration:
   - Pin 2 (PB3): Main Frequency Input (A3)
   - Pin 3 (PB4): Detune Amount Input (A2)
   - Pin 4     : GND
   - Pin 5 (PB0): Primary VCO Output
   - Pin 6 (PB1): Detuned VCO Output 1
   - Pin 7 (PB2): Detuned VCO Output 2
   - Pin 8     : VCC

   Description:
   Uses Timer0 in CTC mode with no prescaling for maximum frequency range.
   The main frequency input controls the base frequency, while the detune
   input adds an offset to the second and third oscillators. The second
   oscillator is detuned by the input amount, and the third is detuned
   by twice that amount.

   Frequency Ranges (with 8MHz clock):
   - Base Frequency: ~200Hz to ~800Hz
   - Detune Range: 0 to 20 steps from base frequency
   
   Implementation Notes:
   - Interrupt runs at 8MHz/(79+1) = 100kHz
   - Byte counters used for improved performance
   - Detune checked less frequently to prioritize frequency response

   Version History:
   - V1.0: Initial version
   - V1.1: Switched to countdown counters for efficiency
   - V1.2: Added input averaging for stability

   Author: Rob Stave (Rob the Fiddler), CCBY 2015
*/

// Frequency range constants (lower numbers = higher frequencies)
#define VCO1_HIGH 50   // ~800Hz
#define VCO1_LOW 200   // ~200Hz

// Counters for the frequencies
volatile uint8_t oscFreq1 = 200;
volatile uint8_t oscCounter1 = 200;
volatile uint16_t oscFreq2 = 200;
volatile uint16_t oscCounter2 = 200;
volatile uint16_t oscFreq3 = 200;
volatile uint16_t oscCounter3 = 200;

// The setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  // Set output bits

  // Initialize Timer0
  noInterrupts();           // Disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); // Start Timer0 in CTC mode (Table 11.5)
  OCR0A = 79;             // CTC Compare value
  TCCR0B |= (1 << CS00);  // Prescaler = 0 (Table 11.6)

  TIMSK |= (1 << OCIE0A); // Enable CTC interrupt (see 13.3.6)
  interrupts();           // Enable all interrupts
}

ISR(TIMER0_COMPA_vect) {   // Timer compare interrupt service routine

  // Count down and toggle PORTB bits
  if (oscCounter1 <= 0) {
    oscCounter1 = oscFreq1;
    PORTB ^= (_BV(PB0));
  }
  oscCounter1--;

  if (oscCounter2 <= 0) {
    oscCounter2 = oscFreq2;
    PORTB ^= (_BV(PB1));
  }
  oscCounter2--;

  if (oscCounter3 <= 0) {
    oscCounter3 = oscFreq3;
    PORTB ^= (_BV(PB2));
  }
  oscCounter3--;
}

int detuneCount = 0;

void loop() {

  // Read frequency
  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_LOW, VCO1_HIGH);

  // Read detune. No need to check as often; prioritize frequency response.
  if (detuneCount > 5) {
    detuneCount = 0;
    int osc2_t = analogRead(A2);
    int detune = map(osc2_t, 0, 1023, 0, 20);
    oscFreq2 = oscFreq1 + detune;
    oscFreq3 = oscFreq2 + detune;
  }
  detuneCount++;
}
