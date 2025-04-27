/**
 * ACS-85-0605
 * ATTiny85 1-bit Waveform Selector
 *
 * External Pin Layout:
 *
 * External pin 1       = Reset (not used)
 * External pin 2 (PB3) = S1
 * External pin 3 (PB4) = S2
 * External pin 4       = GND
 * External pin 5 (PB0) = Output
 * External pin 6 (PB1) = S0
 * External pin 7 (PB2) = Clock
 * External pin 8       = Vcc
 *
 * Version History:
 * V1.0 - First Version
 *
 * Note: This sketch has been written specifically for ATTINY85 and not Arduino Uno.
 *
 * Author: Rob Stave (Rob the Fiddler) ccby 2015
 *
 * ATTiny85 Pinout Overview:
 *
 *                    +--\/--+
 *             Reset 1|      |8  VCC
 *   (pin3) S2  PB3  2|      |7  PB2 (pin2/int0) Clock
 *   (pin4) S1  PB4  3|      |6  PB1 (pin1) S0
 *              GND  4|      |5  PB0 (pin0) Output
 *                     ------
 */

const int int0 = 0;  // interrupt 0

volatile byte pattern;
int patternCount = 0;

#define PATTERN_0_SIZE 2
boolean pattern_0[PATTERN_0_SIZE] = {LOW, HIGH};

#define PATTERN_1_SIZE 6
boolean pattern_1[PATTERN_1_SIZE] = {LOW, HIGH, LOW, LOW, HIGH, HIGH};

#define PATTERN_2_SIZE 8
boolean pattern_2[PATTERN_2_SIZE] = {LOW, HIGH, LOW, LOW, LOW, HIGH, HIGH, HIGH};

#define PATTERN_3_SIZE 12
boolean pattern_3[PATTERN_3_SIZE] = {LOW, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, LOW, HIGH, HIGH, HIGH};

#define PATTERN_4_SIZE 10
boolean pattern_4[PATTERN_4_SIZE] = {LOW, HIGH, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};

#define PATTERN_5_SIZE 14
boolean pattern_5[PATTERN_5_SIZE] = {LOW, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};

#define PATTERN_6_SIZE 16
boolean pattern_6[PATTERN_6_SIZE] = {LOW, HIGH, LOW, LOW, LOW, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};

#define PATTERN_7_SIZE 20
boolean pattern_7[PATTERN_7_SIZE] = {LOW, HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, LOW, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};

/**
 * Setup function runs once when you press reset or power the board.
 */
void setup()
{
  PORTB = 0x00;       // Enable all pull-downs
  DDRB = B00000001;   // Set PORTB to inputs except PB0

  attachInterrupt(int0, clockCounter, CHANGE);
  // clockInt is our interrupt, clockCounter function is called when
  // invoked on either clock edge
}

/**
 * Get the current value from the selected pattern.
 */
boolean getPatternValue()
{
  boolean result = LOW;

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
  else if (pattern == 4)
  {
    if (patternCount >= PATTERN_4_SIZE)
    {
      patternCount = 0;
    }
    result = pattern_4[patternCount];
  }
  else if (pattern == 5)
  {
    if (patternCount >= PATTERN_5_SIZE)
    {
      patternCount = 0;
    }
    result = pattern_5[patternCount];
  }
  else if (pattern == 6)
  {
    if (patternCount >= PATTERN_6_SIZE)
    {
      patternCount = 0;
    }
    result = pattern_6[patternCount];
  }
  else if (pattern == 7)
  {
    if (patternCount >= PATTERN_7_SIZE)
    {
      patternCount = 0;
    }
    result = pattern_7[patternCount];
  }

  patternCount++;
  return result;
}

/**
 * Interrupt service routine.
 * Sample the counter and determine the elapsed time.
 */
void clockCounter()
{
  boolean v = getPatternValue();
  if (v == HIGH)
  {
    PORTB |= _BV(PB0);
  }
  else
  {
    PORTB &= ~_BV(PB0);
  }
}

/**
 * Main loop function.
 */
void loop()
{
  byte inputPortB = PINB;
  boolean s0 = bitRead(inputPortB, 1);
  boolean s1 = bitRead(inputPortB, 3);
  boolean s2 = bitRead(inputPortB, 4);

  byte output = B00000000;
  bitWrite(output, 0, s0);
  bitWrite(output, 1, s1);
  bitWrite(output, 2, s2);

  pattern = output;
}
