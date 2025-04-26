/**
   ACS-85-0004 ATTiny85 Dual VCO with Selectable Range
   
   A versatile dual oscillator with six selectable frequency ranges from LFO to audio rates.
   Each oscillator has independent frequency control within the selected range.

   Pin Configuration:
   - Pin 2 (PB3): Frequency Input 1 (A3)
   - Pin 3 (PB4): Frequency Input 2 (A2)
   - Pin 4     : GND
   - Pin 5 (PB0): Oscillator 1 Output
   - Pin 6 (PB1): Oscillator 2 Output
   - Pin 7 (PB2): Range Selection Input (A1)
   - Pin 8     : VCC

   Description:
   Uses Timer1 in CTC mode. The range selector input divides the frequency spectrum 
   into six ranges, from very low frequency (LFO) to high frequency audio rates.
   For LFO ranges, an additional counter provides lower frequencies.

   Frequency Ranges (approximate, with 8MHz clock):
   - High Freq 1 (HF1): 50Hz - 4kHz
   - High Freq 2 (HF2): 166Hz - 666Hz
   - Mid Freq 1 (MF1): 62.5Hz - 222Hz
   - Mid Freq 2 (MF2): 38.5Hz - 133Hz
   - Low Freq 1 (LF1): 1Hz - 16Hz
   - Low Freq 2 (LF2): 0.2Hz - 4Hz

   Implementation Notes:
   - Uses different timer configurations for audio vs LFO ranges
   - High/Mid frequencies use direct timer toggling
   - LFO ranges use an additional counter for lower frequencies
   - Maximum frequency limited by interrupt overhead

   Version History:
   - V1.0: Initial version
   - V1.1: Code optimization
   - V1.2: Improved range selection
   - V1.3: Enhanced documentation

   Author: Rob Stave (Rob the Fiddler), CCBY 2015
*/

// Frequency range definitions for each band
// High Frequency Band 1 (4kHz - 50Hz)
#define VCO1_H1_HIGH 5
#define VCO1_H1_LOW 80
#define VCO2_H1_HIGH 5
#define VCO2_H1_LOW 80

// High Frequency Band 2 (666Hz - 166Hz)
#define VCO1_H2_HIGH 30
#define VCO1_H2_LOW 120
#define VCO2_H2_HIGH 30
#define VCO2_H2_LOW 120

// Mid Frequency Band 1 (222Hz - 62.5Hz)
#define VCO1_M1_HIGH 90
#define VCO1_M1_LOW 320
#define VCO2_M1_HIGH 90
#define VCO2_M1_LOW 320

// Mid Frequency Band 2 (133Hz - 38.5Hz)
#define VCO1_M2_HIGH 150
#define VCO1_M2_LOW 520
#define VCO2_M2_HIGH 150
#define VCO2_M2_LOW 520

// Low Frequency Band 1 (16Hz - 1Hz)
#define VCO1_L1_HIGH 25
#define VCO1_L1_LOW 400
#define VCO2_L1_HIGH 25
#define VCO2_L1_LOW 400

// Low Frequency Band 2 (4Hz - 0.2Hz)
#define VCO1_L2_HIGH 100
#define VCO1_L2_LOW 2000
#define VCO2_L2_HIGH 100
#define VCO2_L2_LOW 2000

// Counters for the frequencies
volatile uint16_t oscFreq1 = 0;
volatile uint16_t oscCounter1 = 0;
volatile uint16_t oscFreq2 = 0;
volatile uint16_t oscCounter2 = 0;

volatile boolean isLFO = false;
volatile uint16_t lfoCounter = 0;

volatile uint16_t range = 0;  // From 0 to 5
volatile uint16_t loopCount = 0;

// The setup function runs once when you press reset or power the board
void setup() {

  DDRB = B00000011;  // Set output bits

  // Initialize timer1
  noInterrupts();           // Disable all interrupts

  TCCR1 = 0;                  // Stop the timer
  TCNT1 = 0;                  // Zero the timer
  OCR1A = 99;                // Set the compare value
  OCR1C = 99;
  TIMSK = _BV(OCIE1A);        // Interrupt on Compare Match A

  TCCR1 = _BV(CTC1) | _BV(CS11); // Start timer, CTC mode, prescaler clk/2

  interrupts();             // Enable all interrupts

}

ISR(TIMER1_COMPA_vect)          // Timer compare interrupt service routine
{

  if (isLFO == true) {
    // Count up and toggle portB bits
    if (lfoCounter > 50) {
      lfoCounter = 0;

      // Count up and toggle portB bits
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

    return;
  }

  // Count and toggle portB bits
  if (oscCounter1 == 0) {
    oscCounter1 = oscFreq1;
    PORTB ^= (_BV(PB0));
  }
  oscCounter1--;

  if (oscCounter2 == 0) {
    oscCounter2 = oscFreq2;
    PORTB ^= (_BV(PB1));
  }
  oscCounter2--;

}

void loop() {

  // No need to monitor range that often
  if (loopCount > 20) {
    loopCount = 0;
    int range_t = analogRead(A1);
    range = map(range_t, 0, 1023, 0,  60);
  }
  loopCount++;

  int osc1_t = analogRead(A3);
  int osc2_t = analogRead(A2);

  if (range < 10) {

    isLFO = true;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_L2_LOW,  VCO1_L2_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_L2_LOW,  VCO2_L2_HIGH);
    return;
  }

  if (range < 20) {
    isLFO = true;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_L1_LOW,  VCO1_L1_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_L1_LOW,  VCO2_L1_HIGH);
    return;
  }

  if (range < 30) {
    isLFO = false;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_M2_LOW,  VCO1_M2_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_M2_LOW,  VCO2_M2_HIGH);
    return;

  }

  if (range < 40) {
    isLFO = false;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_M1_LOW,  VCO1_M1_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_M1_LOW,  VCO2_M1_HIGH);
    return;
  }

  if (range < 50) {
    isLFO = false;
    oscFreq1 = map(osc1_t, 0, 1023, VCO1_H2_LOW,  VCO1_H2_HIGH);
    oscFreq2 = map(osc2_t, 0, 1023, VCO2_H2_LOW,  VCO2_H2_HIGH);
    return;
  }

  isLFO = false;
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_H1_LOW,  VCO1_H1_HIGH);
  oscFreq2 = map(osc2_t, 0, 1023, VCO2_H1_LOW,  VCO2_H1_HIGH);
  return;

}
