/*
 * ACS-85-0015
 * ATTiny85 Dual LFO Two Ranges
 *
 * This sketch implements two LFOs (Low-Frequency Oscillators) taken from ACS-85-0004.
 * Each LFO operates independently, providing two frequency ranges: LF1 and LF2.
 *
 * External Pinout:
 * - Pin 1: Reset (not used)
 * - Pin 2 (PB3): Input 0 (Frequency 1)
 * - Pin 3 (PB4): Input 1 (Frequency 2)
 * - Pin 4: GND
 * - Pin 5 (PB0): Output 0
 * - Pin 6 (PB1): Output 1
 * - Pin 7 (PB2): Not used
 * - Pin 8: VCC
 *
 * Version 1.0 - Initial version
 * Note: This sketch is designed for the ATTiny85 but can be adapted for Arduino Uno with minor tweaks.
 * Author: Rob Stave (Rob the Fiddler), CC BY 2015
 *
 * TODO: Optimize by removing nested loops and scaling the counter directly.
 *
 * ATTiny85 Overview
 *                       +-\/+-
 *                Reset 1|    |8  VCC
 *  (pin3) in 0 A3  PB3 2|    |7  PB2 (pin2) none
 *  (pin4) in 1 A2  PB4 3|    |6  PB1 (pin1) out 1
 *                  GND 4|    |5  PB0 (pin0) out 0
 *                       ------
 */

/*
 * Note: In the switch, an extra nested loop of 50 is used.
 * Calculation:
 * 8 MHz / 2 / 100 / 50 = 800 Hz Interrupt Frequency
 */

#define VCO1_L1_HIGH 25  // (800/2)/25 = 16 Hz
#define VCO1_L1_LOW 400  // (800/2)/400 =1 Hz

#define VCO2_L2_HIGH 100 // 4 Hz
#define VCO2_L2_LOW 2000 // 0.2 Hz

// Counters for the frequencies
volatile int16_t oscFreq1 = 0;
volatile int16_t oscCounter1 = 0;
volatile int16_t oscFreq2 = 0;
volatile int16_t oscCounter2 = 0;

volatile int16_t lfoCounter = 0;

void setup() {
  DDRB = B00000011;  // Set output bits

  // Initialize timer1
  noInterrupts();           // Disable all interrupts

  TCCR1 = 0;                // Stop the timer
  TCNT1 = 0;                // Zero the timer
  OCR1A = 99;               // Set the compare value
  OCR1C = 99;
  TIMSK = _BV(OCIE1A);      // Interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, CTC mode, prescaler clk/2

  interrupts();             // Enable all interrupts
}

ISR(TIMER1_COMPA_vect) {
  if (lfoCounter > 50) {
    lfoCounter = 0;

    if (oscCounter1 > oscFreq1) {
      oscCounter1 = 0;
      PORTB ^= (_BV(PB0));
    }
    oscCounter1++;

    if (oscCounter2 > oscFreq2) {
      oscCounter2 = 0;
      PORTB ^= (_BV(PB1));
    }
    oscCounter2++;
  }
  lfoCounter++;
}

void loop() {
  int osc1_t = analogRead(A3);
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_L1_LOW, VCO1_L1_HIGH);

  int osc2_t = analogRead(A2);
  oscFreq2 = map(osc2_t, 0, 1023, VCO2_L2_LOW, VCO2_L2_HIGH);
}
