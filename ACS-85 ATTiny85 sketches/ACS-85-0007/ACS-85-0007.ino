/**
   ACS-85-0007 ATTiny85 Square Wave with Variable Frequency Ramp
   
   A square wave generator that creates three outputs with different frequency
   variation patterns, producing unique timbral characteristics.

   Pin Configuration:
   - Pin 2 (PB3): Frequency Input (A3)
   - Pin 3 (PB4): Not used
   - Pin 4     : GND
   - Pin 5 (PB0): Output 0 - Small Variance (period + mod 8)
   - Pin 6 (PB1): Output 1 - Medium Variance (period + mod 16)
   - Pin 7 (PB2): Output 2 - Large Variance (period + mod 32)
   - Pin 8     : VCC

   Description:
   Uses Timer0 in CTC mode. Each output adds different amounts of variance
   to the base period. The variance is achieved by masking different bit widths:
   - Output 0: Base period + (counter mod 8)  [3-bit mask]
   - Output 1: Base period + (counter mod 16) [4-bit mask]
   - Output 2: Base period + (counter mod 32) [5-bit mask]

   Frequency Response:
   The effective frequency decreases as the period increases:
   - 40  => ~1250 Hz
   - 45  => ~1111 Hz
   - 215 => ~232 Hz
   - 222 => ~225 Hz

   Note: Counter is byte-sized and ramp is max 5 bits (32), 
   so VCO1_LOW should not exceed 223.

   Version History:
   - V1.0: Initial version
   - V1.1: Improved period calculation
   - V1.2: Enhanced documentation

   Author: Rob Stave (Rob the Fiddler), CCBY 2015
*/

// Frequency range constants
#define VCO1_HIGH 40    // Higher frequency (~1250 Hz)
#define VCO1_LOW 223    // Lower frequency (~225 Hz)

// Counters for the frequencies
volatile uint8_t oscFreq1 = VCO1_LOW;

volatile uint8_t oscCounter1 = VCO1_LOW;
volatile uint8_t oscCounter2 = VCO1_LOW;
volatile uint8_t oscCounter3 = VCO1_LOW;

volatile uint8_t counter1 = 0;
volatile uint8_t counter2 = 0;
volatile uint8_t counter3 = 0;

// The setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000111;  // Set output bits

  // Initialize Timer0
  noInterrupts();           // Disable all interrupts

  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01); // Start Timer0 in CTC mode (Table 11.5)

  OCR0A = 79; // CTC Compare value

  TCCR0B |= (1 << CS00); // Prescaler = 0 (Table 11.6)

  TIMSK |= (1 << OCIE0A); // Enable CTC interrupt (see 13.3.6)
  interrupts();           // Enable all interrupts
}

ISR(TIMER0_COMPA_vect)          // Timer compare interrupt service routine
{

  // Count up and toggle portB bits
  if (oscCounter1 <= 0) {
    counter1++;
    oscCounter1 = oscFreq1 + (counter1 & B00000111);
    PORTB ^= (_BV(PB0));
  }
  oscCounter1--;

  if (oscCounter2 <= 0) {
    counter2++;
    oscCounter2 = oscFreq1 + (counter2 & B00001111);
    PORTB ^= (_BV(PB1));
  }
  oscCounter2--;

  if (oscCounter3 <= 0) {
    counter3++;
    oscCounter3 = oscFreq1 + (counter3 & B00011111);
    PORTB ^= (_BV(PB2));
  }
  oscCounter3--;

}

void loop() {

  // Read frequency
  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_LOW, VCO1_HIGH);
}
