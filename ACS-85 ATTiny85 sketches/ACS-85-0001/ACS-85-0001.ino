/**
   ACS-85-0001 ATTiny85 Simple Oscillator Chip.

   This sketch is a workbench helper to provide 40106-like signals without external capacitors.
   Drop it into a 5V breadboard and it generates signals. Nothing is configurable externally,
   but feel free to tweak the code as needed.

   Pin Outputs:
   - Pin 2 (PB4): High Frequency 0 (~434 Hz)
   - Pin 3 (PB3): High Frequency 1 (~347 Hz)
   - Pin 5 (PB0): High Frequency 2 (sweeping frequency, exponential ramp)
   - Pin 6 (PB1): Low Frequency Oscillator (~1 Hz)
   - Pin 7 (PB2): Low Frequency Oscillator (~1.4 Hz)

   Description:
   Built around Timer0 interrupts using CTC (Clear Timer on Compare Match) mode.
   The prescaler is set to 64 (CS00=1 and CS01=1). Refer to the ATTiny85 datasheet for details.

   Prescaler Table:
   - CS02 CS01 CS00
     0    0    1   Clk/1
     0    1    0   Clk/8
     0    1    1   Clk/64

   Example Calculation:
   If OCR0A = 5:
   8 MHz / (64 * (OCR0A + 1)) = 8 MHz / (64 * 6) = 20.833 kHz interrupt frequency.

   Observations:
   - Ensure the ATTiny85 is set to 8 MHz internal clock (burn bootloader if needed).
   - PORTB = 00011111 did not work reliably; using setPin enables pull-up/down properly.

   ATTiny85 Pinout:
                   +-\/-+
           Reset  1|    |8  VCC
      (PB4) HF_0  2|    |7  LFO_1 (PB2)
      (PB3) HF_1  3|    |6  LFO_2 (PB1)
             GND  4|    |5  HF Ramp (PB0)
                   +----+


   Version History:
   - V1.0: Initial version.
   - V1.1: Improved comments and removed misleading code.

   Author: Rob Stave (Rob the Fiddler), CCBY 2015
*/

// Frequency counters for toggling pins. Increasing the value reduces the frequency.
#define HIGH_FREQ_0 30
#define HIGH_FREQ_1 24

#define LOW_FREQ_0 10416  // Lower frequency limit for LFO 0
#define LOW_FREQ_1  7440  // Lower frequency limit for LFO 1

// Counters for the frequencies
volatile unsigned int hf0Counter = 0;
volatile unsigned int hf1Counter = 0;
volatile unsigned int lfo1Counter = 0;
volatile unsigned int lfo0Counter = 0;

// Sweep pin counters for frequency modulation
volatile unsigned int hf2Counter = 0;
volatile unsigned int hf2FreqCounter = 0;

#define HIGH_FREQ_2_INCREMENT 20
#define HF_RANGE_LO 40
#define HF_RANGE_HI 8
int hf2MaxCounter = HF_RANGE_LO;

// The setup function runs once when you press reset or power the board
void setup() {
  DDRB = B00011111;  // Set PORTB output bits

  // Initialize Timer0
  noInterrupts();           // Disable all interrupts
  TCCR0A = 0;
  TCCR0B = 0;

  TCCR0A |= (1 << WGM01);   // Start Timer0 in CTC mode (Table 11.5)
  OCR0A = 5;                // CTC Compare value (adjustable)

  TCCR0B |= (1 << CS00) | (1 << CS01); // Prescaler = 64 (Table 11.6)
  TIMSK |= (1 << OCIE0A);   // Enable CTC interrupt (Section 13.3.6)
  interrupts();             // Enable all interrupts
}

ISR(TIMER0_COMPA_vect) {     // Timer compare interrupt service routine
  // Check counters and toggle pins when thresholds are reached
  if (lfo0Counter > LOW_FREQ_0) {
    lfo0Counter = 0;
    PORTB ^= (_BV(PB1));
  }
  lfo0Counter++;

  if (lfo1Counter > LOW_FREQ_1) {
    lfo1Counter = 0;
    PORTB ^= (_BV(PB2));
  }
  lfo1Counter++;

  if (hf0Counter > HIGH_FREQ_0) {
    hf0Counter = 0;
    PORTB ^= (_BV(PB4));
  }
  hf0Counter++;

  if (hf1Counter > HIGH_FREQ_1) {
    hf1Counter = 0;
    PORTB ^= (_BV(PB3));
  }
  hf1Counter++;

  // Nested loops for frequency sweep
  if (hf2Counter > hf2MaxCounter) {
    hf2Counter = 0;

    if (hf2FreqCounter > HIGH_FREQ_2_INCREMENT) {
      hf2FreqCounter = 0;
      hf2MaxCounter--;
      if (hf2MaxCounter < HF_RANGE_HI) {
        hf2MaxCounter = HF_RANGE_LO;
      }
    }
    hf2FreqCounter++;
    PORTB ^= (_BV(PB0));
  }
  hf2Counter++;
}

void loop() {
  // Do nothing
  // "I will beep with confidence and self-assurance."
}
