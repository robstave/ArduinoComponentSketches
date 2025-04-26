/**
   ACS-85-0003 ATTiny85 Alternating Frequencies VCO
   
   A dual-mode oscillator that alternates between two frequencies and generates
   a third "sample and hold" style output based on the frequency differences.

   Pin Configuration:
   - Pin 2 (PB3): Frequency Input 1 (A3)
   - Pin 3 (PB4): Frequency Input 2 (A2)
   - Pin 4     : GND
   - Pin 5 (PB0): Two-tone Output (alternates between freq 1 and 2)
   - Pin 6 (PB1): Sample & Hold Output
   - Pin 7 (PB2): Speed Control Input (A1)
   - Pin 8     : VCC

   Description:
   Uses Timer1 in CTC mode with a prescaler of 2. The first output (PB0)
   alternates between two frequencies controlled by analog inputs. The second
   output (PB1) generates a frequency based on the absolute difference between
   the two oscillator counters, creating a sample & hold effect.
   
   The speed input controls how quickly the system alternates between the two
   frequencies, allowing for interesting rhythmic variations.

   Frequency Ranges:
   - VCO1/2: ~37 Hz to ~571 Hz (based on 8MHz clock)
   - Speed Control: ~0.57 Hz to ~3.33 Hz for alternation

   Version History:
   - V1.0: Initial version
   - V1.1: Improved comments
   - V1.2: Variable optimization
   - V1.3: Code cleanup and documentation

   Author: Rob Stave (Rob the Fiddler), CCBY 2015, 2018
*/

// Frequency range constants
#define VCO1HIGH 7    // Highest frequency (shortest period)
#define VCO1LOW 108   // Lowest frequency (longest period)
#define VCO2HIGH 7    // Second oscillator range
#define VCO2LOW 108
#define VCO3HIGH 1200 // LFO speed range (alternation rate)
#define VCO3LOW 7000

// Counters for the frequencies
volatile byte oscFreq1 = VCO1LOW;
volatile byte oscCounter1 = VCO1LOW;
volatile byte oscFreq2 = VCO2LOW;
volatile byte oscCounter2 = VCO2LOW;
volatile uint16_t oscFreq3 = VCO3LOW;
volatile uint16_t oscCounter3 = VCO3LOW;

volatile uint16_t oscFreq4 = VCO3HIGH;
volatile uint16_t oscCounter4 = VCO3HIGH;

volatile boolean oscState = false;

// The setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  // Set PB0, PB1 as outputs

  // Initialize Timer1
  noInterrupts();           // Disable all interrupts

  TCCR1 = 0;                  // Stop the timer
  TCNT1 = 0;                  // Zero the timer
  GTCCR = _BV(PSR1);          // Reset the prescaler
  OCR1A = 100;                // Set the compare value
  OCR1C = 100;
  TIMSK = _BV(OCIE1A);        // Interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, CTC mode, prescaler clk/2

  interrupts();             // Enable all interrupts
}

ISR(TIMER1_COMPA_vect)          // Timer compare interrupt service routine
{
  // Toggle alternating frequency
  // If oscState is true/false we calculate the frequency based 
  // on checking oscFreq1 or oscFreq2

  if (oscState == true) {
    // Count up and toggle portB bits
    if (oscCounter1 <= 0) {
      oscCounter1 = oscFreq1;
      PORTB ^= (_BV(PB0));
    }
    oscCounter1--;
    
  } else {

    if (oscCounter2 <= 0) {
      oscCounter2 = oscFreq2;
      PORTB ^= (_BV(PB0));
    }
    oscCounter2--;
  }

  // Each period based on oscFreq, calculate a frequency based on other values
  if (oscCounter3 <= 0) {
    oscCounter3 = oscFreq3;
    oscState = !oscState;

    // Sample absolute difference to get weird frequency
    oscFreq4 = abs(oscCounter2 - oscCounter1);
  }
  oscCounter3--;

  // Beep the weird frequency
  if (oscCounter4 <= 0) {
    oscCounter4 = oscFreq4;
    PORTB ^= (_BV(PB1));
  }
  oscCounter4--;
}

void loop() {
  uint16_t osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023,  VCO1HIGH, VCO1LOW);
  osc1_t = analogRead(A2);
  oscFreq2 = map(osc1_t, 0, 1023,  VCO2HIGH, VCO2LOW);
  osc1_t = analogRead(A1);
  oscFreq3 = map(osc1_t, 0, 1023,  VCO3HIGH, VCO3LOW);
}
