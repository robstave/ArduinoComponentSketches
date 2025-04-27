/**
 * ACS-85-0601
 * ATTiny85 3 Channel Sequencer
 *
 * Simple drum beats or 3-pin sequencer.
 *
 * Patterns can be programmed by changing the values in the arrays.
 *
 * External Pin Layout:
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = Input 0 frequency
 * External pin 3 (PB4) = Input 1 scene select
 * External pin 4       = GND
 * External pin 5 (PB0) = Output 0
 * External pin 6 (PB1) = Output 1
 * External pin 7 (PB2) = Output 2
 * External pin 8       = Vcc
 *
 * Version History:
 * V1.0 - First Version
 * Any further extensions to this will be treated as a sequencer.
 *
 * Author: Rob Stave (Rob the Fiddler) ccby 2015
 *
 *
 * ATTiny85 Pinout Overview:
 *
 *                         +--\/--+
 *                  Reset 1|      |8  VCC
 *   (pin3) freq A3  PB3  2|      |7  PB2 (pin2) Output 2
 *  (pin4) scene A2  PB4  3|      |6  PB1 (pin1) Output 1
 *                   GND  4|      |5  PB0 (pin0) Output 0
 *                          ------
 */

// Define ranges for the potentiometer. A smaller number means a shorter timer.
#define VCO1_L1_HIGH 200
#define VCO1_L1_LOW 3000

// Use this to tweak the feel of the potentiometer. Set to 0 for an audio pot.
#define MAP_VALUES_AS_LINEAR 0

#if MAP_VALUES_AS_LINEAR == 1
#define STATE0 10
#define STATE1 20
#define STATE2 30
#define STATE3 40
#else
#define STATE0 5
#define STATE1 10
#define STATE2 20
#define STATE3 40
#endif

// Counters for the frequencies
volatile int oscCounter1 = 0;
volatile int oscFreq1 = 0;

int patternCount = 0;
volatile int pattern = 0;

// Define patterns
#define PATTERN_0_SIZE 16
byte pattern_0[PATTERN_0_SIZE] = {3, 0, 1, 0, 7, 0, 1, 0, 3, 0, 1, 0, 7, 0, 1, 0};

#define PATTERN_1_SIZE 16
byte pattern_1[PATTERN_1_SIZE] = {2, 1, 6, 1, 2, 1, 6, 1, 2, 1, 6, 1, 2, 1, 6, 1};

#define PATTERN_2_SIZE 16
byte pattern_2[PATTERN_2_SIZE] = {3, 0, 7, 0, 7, 0, 3, 0, 3, 0, 1, 0, 1, 0, 7, 0};

#define PATTERN_3_SIZE 16
byte pattern_3[PATTERN_3_SIZE] = {3, 0, 1, 0, 5, 0, 1, 0, 3, 0, 3, 0, 5, 0, 1, 0};

/**
 * Setup function runs once when you press reset or power the board.
 */
void setup()
{
  DDRB = B00000111; // Set PB0, PB1, PB2 as output

  // Initialize Timer1
  noInterrupts(); // Disable all interrupts

  TCCR1 = 0; // Stop the timer
  TCNT1 = 0; // Reset the timer counter
  OCR1A = 99; // Set the compare value for Timer1
  OCR1C = 99; // Set the top value for Timer1
  TIMSK = _BV(OCIE1A); // Enable interrupt on Compare Match A

  // Configure Timer1: CTC mode, prescaler clk/2
  TCCR1 = _BV(CTC1) | _BV(CS11) | _BV(CS12);

  interrupts(); // Enable all interrupts
}

/**
 * Get the current value from the selected pattern.
 */
byte getPatternValue()
{
  byte result = B00000000;

  if (pattern == 0)
  {
    if (patternCount >= PATTERN_0_SIZE)
    {
      patternCount = 0;
    }
    result = pattern_0[patternCount];
  }
  else if (pattern == 1)
  {
    if (patternCount >= PATTERN_1_SIZE)
    {
      patternCount = 0;
    }
    result = pattern_1[patternCount];
  }
  else if (pattern == 2)
  {
    if (patternCount >= PATTERN_2_SIZE)
    {
      patternCount = 0;
    }
    result = pattern_2[patternCount];
  }
  else if (pattern == 3)
  {
    if (patternCount >= PATTERN_3_SIZE)
    {
      patternCount = 0;
    }
    result = pattern_3[patternCount];
  }

  patternCount++;

  byte output = result & B00000111;
  return output;
}

/**
 * Timer1 Compare Match A interrupt service routine.
 */
ISR(TIMER1_COMPA_vect)
{
  if (oscCounter1 >= oscFreq1)
  {
    oscCounter1 = 0;
    PORTB = getPatternValue();
  }

  oscCounter1++;
}

/**
 * Map the analog input to a pattern index.
 */
int mapPattern(int sample)
{
  if (sample <= STATE0)
  {
    return 0;
  }
  else if (sample <= STATE1)
  {
    return 1;
  }
  else if (sample <= STATE2)
  {
    return 2;
  }
  else if (sample <= STATE3)
  {
    return 3;
  }
  return 0;
}

/**
 * Main loop function.
 */
void loop()
{
  int osc1_t = analogRead(A3); // Read LFO frequency input
  oscFreq1 = map(osc1_t, 0, 1023, VCO1_L1_LOW, VCO1_L1_HIGH);

  int pattern_t = analogRead(A2); // Read scene select input
  pattern = mapPattern(map(pattern_t, 0, 1023, 0, 40));
}
