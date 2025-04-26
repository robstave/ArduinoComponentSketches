/**
   ACS-85-0006 ATTiny85 Variable Period Square Wave Generator
   
   A unique square wave generator that creates varying timbres by modulating
   the period of oscillation rather than just duty cycle.

   Pin Configuration:
   - Pin 2 (PB3): Frequency Input (A3)
   - Pin 3 (PB4): Not used
   - Pin 4     : GND
   - Pin 5 (PB0): Reference Square Wave Output
   - Pin 6 (PB1): Variable Period Output (main output)
   - Pin 7 (PB2): Additive Period Output
   - Pin 8     : VCC

   Description:
   Uses Timer0 in CTC mode for precise timing. The main output (PB1) varies its
   period randomly within a controlled range, creating a "fuzzy" or "nebulous"
   sound quality. PB0 provides a reference square wave at the base frequency,
   while PB2 offers an alternative modulation using additive period variation.

   Implementation Details:
   - Main output (PB1) uses random period variation masked to 4 bits
   - Secondary output (PB2) uses counter-based period variation
   - Basic frequency range: ~166Hz to ~666Hz (with 8MHz clock)
   - Period variations create subtle to dramatic timbral changes

   Version History:
   - V1.0: Initial version
   - V1.1: Improved LFSR implementation
   - V1.2: Enhanced documentation
   - V1.3: Code optimization

   Author: Rob Stave (Rob the Fiddler), CCBY 2015
*/

// Frequency range constants
#define VCO1_HIGH 50  // Higher frequency (~666 Hz)
#define VCO1_LOW 200  // Lower frequency (~166 Hz)

// Counters for the frequencies
volatile uint8_t oscFreq1 = VCO1_LOW;
volatile uint8_t oscCounter1 = VCO1_LOW;
volatile uint8_t oscCounter2 = VCO1_LOW;
volatile uint8_t oscCounter3 = VCO1_LOW;

// 16 bits used as the LFSR
volatile uint16_t lfsr = 1;
volatile uint16_t counter = 1;

void clockLfsr() {
  // Calculate new state
  boolean outputBit = bitRead(lfsr, 10) ^ bitRead(lfsr, 12)
                      ^ bitRead(lfsr, 13) ^ bitRead(lfsr, 15);
  lfsr = lfsr << 1;
  lfsr |= outputBit;
}

// The setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00000111;  // Set output bits

  // Initialize timer1
  noInterrupts();           // Disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); // Start timer 1 in CTC mode Table 11.5

  OCR0A = 79; // CTC Compare value

  TCCR0B |= (1 << CS00); // Prescaler = 0 Table 11.6

  // TCCR0A |=(1<<COM0A1); // Timer0 in toggle mode Table 11.2
  TIMSK |= (1 << OCIE0A); // Enable CTC interrupt see 13.3.6
  interrupts();           // Enable all interrupts
}

ISR(TIMER0_COMPA_vect) { // Timer compare interrupt service routine
  // Count up and toggle portB bits
  if (oscCounter1 <= 0) {
    oscCounter1 = oscFreq1;
    PORTB ^= (_BV(PB0));
  }
  oscCounter1--;

  if (oscCounter2 <= 0) {
    oscCounter2 = oscFreq1 + 8 - (lfsr & B00001111);
    PORTB ^= (_BV(PB1));
  }
  oscCounter2--;

  if (oscCounter3 <= 0) {
    oscCounter3 = oscFreq1 + 15 - (counter & B00011111);
    counter++;
    PORTB ^= (_BV(PB2));
  }
  oscCounter3--;
}

void loop() {
  // Read frequency
  uint16_t osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_LOW, VCO1_HIGH);
  clockLfsr();
}
